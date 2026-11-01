#!/usr/bin/ucode
'use strict';

import { cursor } from 'uci';
import { glob, open, readfile, writefile, unlink, rename, mkdir, lsdir,
	 symlink, access, error as fs_error } from 'fs';
import * as uloop from 'uloop';
import * as uclient from 'uclient';
import { convert } from 'webkit-contentfilter.convert';

const CONFIG = 'webkit-contentfilter';
const PUBLISH_DIR = '/var/run/webkit-contentfilter';
const DEFAULT_STORAGE = '/var/lib/webkit-contentfilter';
const COMPILER = '/usr/libexec/webkit-contentfilter-compile';
/* file name WebKit's content filter store gives a compiled list */
const LIST_PREFIX = 'ContentRuleList-';
const RECHECK_INTERVAL_MS = 24 * 60 * 60 * 1000;
/* maxRuleCount in WebKit's ContentExtensionParser.cpp */
const WEBKIT_MAX_RULES = 150000;

let ca_files = glob('/etc/ssl/certs/*.crt');
let fetch = index(ARGV, '--fetch') >= 0;
let recheck_timer;

function dir_create(path) {
	let current = '';

	for (let part in split(path, '/')) {
		if (part == '')
			continue;
		current += '/' + part;
		mkdir(current, 0755);
	}
}

function config_load() {
	let uci = cursor();
	let config = {
		storage: uci.get(CONFIG, 'global', 'storage') ?? DEFAULT_STORAGE,
		sources: []
	};

	uci.foreach(CONFIG, 'source', function(s) {
		if (s.enabled == '0' || !s.url)
			return;
		push(config.sources, { name: s['.name'], url: s.url });
	});

	return config;
}

function meta_load(path) {
	try {
		return json(readfile(path) ?? '{}');
	}
	catch (e) {
		return {};
	}
}

function rules_write(path, rules) {
	let f = open(path, 'w');

	if (!f)
		return false;

	f.write('[');
	for (let i = 0; i < length(rules); i++)
		f.write((i ? ',\n' : '\n') + sprintf('%J', rules[i]));
	f.write('\n]\n');
	f.close();

	return true;
}

function list_compiled(config, name) {
	return `${config.storage}/${LIST_PREFIX}${name}`;
}

/* web views reload a list when its link is created, so always replace it */
function list_publish(config, name) {
	let link = `${PUBLISH_DIR}/${LIST_PREFIX}${name}`;
	let tmp = `${PUBLISH_DIR}/.${LIST_PREFIX}${name}`;

	unlink(tmp);
	if (symlink(list_compiled(config, name), tmp) && rename(tmp, link))
		return;

	warn(`${name}: publishing ${link} failed: ${fs_error()}\n`);
	unlink(tmp);
}

function list_drop(config, name) {
	unlink(`${PUBLISH_DIR}/${LIST_PREFIX}${name}`);
	unlink(list_compiled(config, name));
	unlink(`${config.storage}/${name}.meta`);
}

function compiler_run(args, done) {
	let proc = uloop.process(COMPILER, args, null, (code) => done(code == 0));

	if (!proc)
		done(false);
}

function list_compile(config, source, rules, headers, done) {
	let path = `${config.storage}/${source.name}.json`;

	if (!rules_write(path, rules)) {
		warn(`${source.name}: writing ${path} failed: ${fs_error()}\n`);
		unlink(path);
		return done();
	}

	compiler_run([ config.storage, source.name, path ], (ok) => {
		unlink(path);

		if (!ok) {
			warn(`${source.name}: WebKit could not compile the list\n`);
			return done();
		}

		writefile(`${config.storage}/${source.name}.meta`, sprintf('%J\n', {
			url: source.url,
			etag: headers.etag,
			last_modified: headers['last-modified']
		}));
		list_publish(config, source.name);
		done();
	});
}

function list_store(config, source, text, headers, done) {
	let result = convert(text);
	let count = length(result.rules);

	warn(`${source.name}: ${count} rules, ${sprintf('%J', result.stats)}\n`);

	if (!count || count > WEBKIT_MAX_RULES) {
		warn(`${source.name}: ${count} rules cannot be compiled by WebKit\n`);
		return done();
	}

	list_compile(config, source, result.rules, headers, done);
}

function request_headers(config, source) {
	let meta = meta_load(`${config.storage}/${source.name}.meta`);
	let headers = {};

	if (meta.url != source.url ||
	    !access(list_compiled(config, source.name), 'r'))
		return headers;

	if (meta.etag)
		headers['If-None-Match'] = meta.etag;
	if (meta.last_modified)
		headers['If-Modified-Since'] = meta.last_modified;

	return headers;
}

function source_fetch(config, source, done) {
	let chunks = [];
	let finished;
	let status;
	let cl;

	let finish = function(err) {
		let text;

		if (finished)
			return;
		finished = true;

		if (err)
			warn(`${source.name}: ${source.url}: ${err}\n`);
		else if (status != 200 && status != 304)
			warn(`${source.name}: ${source.url}: HTTP status ${status}\n`);

		if (err || status != 200) {
			cl = null;
			return done();
		}

		text = join('', chunks);
		chunks = null;
		list_store(config, source, text, cl.get_headers(), done);
		cl = null;
	};

	cl = uclient.new(source.url, null, {
		header_done: function() {
			let st = cl.status();

			if (st.redirect) {
				cl.redirect();
				return;
			}
			status = st.status;
		},
		data_read: function() {
			let data;

			while (length(data = cl.read()) > 0)
				if (status == 200)
					push(chunks, data);
		},
		data_eof: function() {
			finish(null);
		},
		error: function(cb, code) {
			finish(`uclient error ${code}`);
		}
	});

	if (!cl)
		return finish('invalid URL');

	if (!cl.ssl_init({ ca_files, verify: true }))
		return finish('TLS setup failed');

	if (!cl.connect())
		return finish('connection failed');

	if (!cl.request('GET', { headers: request_headers(config, source) }))
		finish('request failed');
}

/* loading recompiles a list of an older WebKit format from its source */
function source_check(config, source, done) {
	if (!access(list_compiled(config, source.name), 'r'))
		return done();

	compiler_run([ config.storage, source.name ], (ok) => {
		if (ok)
			list_publish(config, source.name);
		else
			list_drop(config, source.name);
		done();
	});
}

/* a stopped compile leaves its JSON and WebKit's ContentRuleList-XXXXXX */
function storage_clean(config) {
	let names = map(config.sources, s => `${LIST_PREFIX}${s.name}`);

	for (let source in config.sources)
		unlink(`${config.storage}/${source.name}.json`);

	for (let file in lsdir(config.storage) ?? [])
		if (index(file, LIST_PREFIX) == 0 && index(names, file) < 0)
			unlink(`${config.storage}/${file}`);
}

function stale_unpublish(config) {
	let names = map(config.sources, s => `${LIST_PREFIX}${s.name}`);

	for (let file in lsdir(PUBLISH_DIR) ?? [])
		if (index(names, file) < 0)
			unlink(`${PUBLISH_DIR}/${file}`);
}

function sources_fetch(config, pending) {
	let source = shift(pending);

	if (!source)
		return recheck_timer.set(RECHECK_INTERVAL_MS);

	source_fetch(config, source, () => sources_fetch(config, pending));
}

function sources_check(config, pending) {
	let source = shift(pending);

	if (!source && !fetch)
		return uloop.end();
	if (!source)
		return sources_fetch(config, [ ...config.sources ]);

	source_check(config, source, () => sources_check(config, pending));
}

function sources_update() {
	let config = config_load();

	dir_create(config.storage);
	dir_create(PUBLISH_DIR);
	storage_clean(config);
	stale_unpublish(config);
	sources_check(config, [ ...config.sources ]);
}

uloop.init();
recheck_timer = uloop.timer(-1, sources_update);
sources_update();
uloop.run();
