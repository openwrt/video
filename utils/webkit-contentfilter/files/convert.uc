'use strict';

const HOST_ANCHOR = '^[^:]+://+([^:/]+\\.)?';
const SEPARATOR = '[^-.%a-zA-Z0-9_]';
const SEPARATOR_OR_END = '([^-.%a-zA-Z0-9_].*)?$';
const REGEX_SPECIAL = '.+?()[]{}\\$|';

const DEFAULT_TYPES = [
	'child-document', 'image', 'style-sheet', 'script', 'font', 'media',
	'fetch', 'websocket', 'ping', 'other', 'svg-document'
];

const TYPE_OPTIONS = {
	script: [ 'script' ],
	image: [ 'image' ],
	stylesheet: [ 'style-sheet' ],
	css: [ 'style-sheet' ],
	font: [ 'font' ],
	media: [ 'media' ],
	object: [ 'other' ],
	xmlhttprequest: [ 'fetch' ],
	xhr: [ 'fetch' ],
	subdocument: [ 'child-document' ],
	frame: [ 'child-document' ],
	websocket: [ 'websocket' ],
	ping: [ 'ping' ],
	beacon: [ 'ping' ],
	other: [ 'other' ],
	popup: [ 'popup' ]
};

const PROCEDURAL = regexp(':(has-text|matches-css|matches-css-before|matches-css-after|matches-path|matches-attr|matches-prop|matches-media|min-text-length|upward|xpath|remove|remove-attr|remove-class|style|watch-attr|others|contains|if|if-not|nth-ancestor|-abp-[a-z-]+)\\(');

function pattern_regex(pattern) {
	let n = length(pattern);
	let re = '';
	let i = 0;
	let end;

	if (substr(pattern, 0, 2) == '||') {
		re = HOST_ANCHOR;
		i = 2;
	}
	else if (substr(pattern, 0, 1) == '|') {
		re = '^';
		i = 1;
	}

	if (n > i && substr(pattern, n - 1, 1) == '|') {
		end = '$';
		n--;
	}

	for (; i < n; i++) {
		let c = substr(pattern, i, 1);

		if (ord(c) > 127)
			return null;

		if (c == '*')
			re += '.*';
		else if (c == '^' && i == n - 1 && !end)
			re += SEPARATOR_OR_END;
		else if (c == '^')
			re += SEPARATOR;
		else if (index(REGEX_SPECIAL, c) >= 0)
			re += '\\' + c;
		else
			re += c;
	}

	re += end ?? '';

	return length(re) ? re : '.*';
}

function domain_entry(domain) {
	domain = lc(trim(domain));

	if (!length(domain) || match(domain, /[^a-z0-9.-]/))
		return null;

	return '*' + domain;
}

function domain_lists(spec, sep) {
	let lists = { include: [], exclude: [], dropped: 0 };

	for (let d in split(spec, sep)) {
		let negated = (substr(d, 0, 1) == '~');
		let entry = domain_entry(negated ? substr(d, 1) : d);

		if (!entry) {
			lists.dropped++;
			continue;
		}

		push(negated ? lists.exclude : lists.include, entry);
	}

	return lists;
}

function option_types(state, name, negated) {
	let types = TYPE_OPTIONS[name];

	if (!types)
		return false;

	if (!negated) {
		state.types ??= [];
		push(state.types, ...types);
		return true;
	}

	state.excluded_types ??= [];
	push(state.excluded_types, ...types);
	return true;
}

function options_parse(spec) {
	let state = { case_sensitive: false, important: false };

	for (let opt in split(spec, ',')) {
		let negated = (substr(opt, 0, 1) == '~');
		let name = lc(negated ? substr(opt, 1) : opt);
		let value;
		let eq = index(name, '=');

		if (eq >= 0) {
			value = substr(opt, (negated ? 1 : 0) + eq + 1);
			name = substr(name, 0, eq);
		}

		if (name == 'third-party' || name == '3p')
			state.load_type = negated ? 'first-party' : 'third-party';
		else if (name == 'first-party' || name == '1p')
			state.load_type = negated ? 'third-party' : 'first-party';
		else if ((name == 'domain' || name == 'from') && value != null)
			state.domains = domain_lists(value, '|');
		else if (name == 'match-case')
			state.case_sensitive = true;
		else if (name == 'important')
			state.important = true;
		else if (name == 'document' || name == 'doc')
			state.document = true;
		else if (name == 'elemhide' || name == 'ehide')
			state.elemhide = true;
		else if (name == 'generichide' || name == 'ghide')
			state.generichide = true;
		else if (!option_types(state, name, negated))
			return null;
	}

	return state;
}

function trigger_types(opts) {
	let types = opts.types ?? DEFAULT_TYPES;

	if (opts.excluded_types)
		types = filter(types, t => index(opts.excluded_types, t) < 0);

	return uniq(types);
}

function trigger_new(url_filter, opts) {
	let trigger = { 'url-filter': url_filter };

	if (opts?.case_sensitive)
		trigger['url-filter-is-case-sensitive'] = true;

	if (opts?.load_type)
		trigger['load-type'] = [ opts.load_type ];

	return trigger;
}

function rule_new(trigger, action) {
	return { trigger, action };
}

/* A trigger takes one domain condition: excluded domains get their own rule. */
function network_rules(re, opts, action_type) {
	let types = trigger_types(opts);
	let domains = opts.domains;
	let rules = [];
	let trigger;

	if (!length(types))
		return rules;

	trigger = trigger_new(re, opts);
	trigger['resource-type'] = types;

	if (domains && length(domains.include))
		trigger['if-domain'] = domains.include;
	else if (domains && length(domains.exclude))
		trigger['unless-domain'] = domains.exclude;
	else if (domains)
		return rules;

	push(rules, rule_new(trigger, { type: action_type }));

	if (action_type != 'block' || !domains || !length(domains.include) ||
	    !length(domains.exclude))
		return rules;

	trigger = trigger_new(re, opts);
	trigger['resource-type'] = types;
	trigger['if-domain'] = domains.exclude;
	push(rules, rule_new(trigger, { type: 'ignore-previous-rules' }));

	return rules;
}

function badfilter_key(line) {
	return replace(replace(line, /[$,]badfilter$/, ''), /,badfilter,/, ',');
}

function network_parse(out, line) {
	let exception = (substr(line, 0, 2) == '@@');
	let body = exception ? substr(line, 2) : line;
	let pattern = body;
	let opts = { case_sensitive: false, important: false };
	let dollar = rindex(body, '$');
	let re;

	if (dollar >= 0 && !match(substr(body, dollar), /^\$\/$/)) {
		opts = options_parse(substr(body, dollar + 1));
		pattern = substr(body, 0, dollar);
	}

	if (!opts)
		return out.stats.unsupported_option++;

	if (opts.domains?.dropped && !length(opts.domains.include) &&
	    !length(opts.domains.exclude))
		return out.stats.unsupported_domain++;

	if (match(pattern, /^\/.*\/$/))
		return out.stats.regex++;

	re = pattern_regex(pattern);
	if (!re)
		return out.stats.non_ascii++;

	if (!exception) {
		if (opts.elemhide || opts.generichide)
			return out.stats.unsupported_option++;

		if (opts.document) {
			opts.types ??= [];
			push(opts.types, 'top-document');
		}

		push(opts.important ? out.important : out.block,
		     ...network_rules(re, opts, 'block'));
		return out.stats.network++;
	}

	if (opts.document) {
		let trigger = trigger_new('.*', null);
		trigger['if-top-url'] = [ re ];
		push(out.document_exception, rule_new(trigger,
			{ type: 'ignore-previous-rules' }));
		return out.stats.network_exception++;
	}

	if (opts.elemhide || opts.generichide) {
		let trigger = trigger_new(re, opts);
		trigger['resource-type'] = [ 'top-document', 'child-document' ];
		push(opts.elemhide ? out.elemhide : out.generichide,
		     rule_new(trigger, { type: 'ignore-previous-rules' }));
		return out.stats.cosmetic_exception++;
	}

	push(out.exception, ...network_rules(re, opts, 'ignore-previous-rules'));
	return out.stats.network_exception++;
}

function cosmetic_parse(out, domains, separator, selector) {
	let lists;

	if (separator != '##' && separator != '#@#')
		return out.stats.extended_cosmetic++;

	if (match(selector, /^(\+js|\^)/) || match(selector, PROCEDURAL))
		return out.stats.extended_cosmetic++;

	if (match(selector, /[^\x01-\x7f]/))
		return out.stats.non_ascii++;

	lists = domain_lists(domains, ',');

	if (separator == '#@#') {
		if (!length(domains))
			out.cosmetic_exceptions[selector] = null;
		else if (!exists(out.cosmetic_exceptions, selector))
			out.cosmetic_exceptions[selector] = [ ...lists.include ];
		else if (out.cosmetic_exceptions[selector])
			push(out.cosmetic_exceptions[selector], ...lists.include);
		return out.stats.cosmetic_exception++;
	}

	if (!length(domains)) {
		push(out.generic, selector);
		return out.stats.cosmetic++;
	}

	if (!length(lists.include) && !length(lists.exclude))
		return out.stats.unsupported_domain++;

	push(out.specific, { selector, lists });
	return out.stats.cosmetic++;
}

function cosmetic_generic_rule(out, selector) {
	let excluded = out.cosmetic_exceptions[selector];
	let trigger = trigger_new('.*', null);

	if (exists(out.cosmetic_exceptions, selector) && excluded == null)
		return null;

	if (length(excluded))
		trigger['unless-domain'] = uniq(excluded);

	return rule_new(trigger, { type: 'css-display-none', selector });
}

function cosmetic_specific_rule(out, entry) {
	let excluded = out.cosmetic_exceptions[entry.selector];
	let include = filter(entry.lists.include, d => index(excluded ?? [], d) < 0);
	let trigger = trigger_new('.*', null);

	if (exists(out.cosmetic_exceptions, entry.selector) && excluded == null)
		return null;

	if (length(include))
		trigger['if-domain'] = include;
	else if (length(entry.lists.include))
		return null;
	else
		trigger['unless-domain'] = entry.lists.exclude;

	return rule_new(trigger,
		{ type: 'css-display-none', selector: entry.selector });
}

function line_classify(out, badfilters, line) {
	let m;

	if (!length(line) || substr(line, 0, 1) == '!' || substr(line, 0, 1) == '[')
		return;

	m = match(line, /^([a-zA-Z0-9.,~*_-]*)(#@?[?$%]?[?]?#)(.*)$/);
	if (m)
		return cosmetic_parse(out, m[1], m[2], m[3]);

	if (match(line, /\$\$|\$@\$/))
		return out.stats.extended_cosmetic++;

	if (exists(badfilters, badfilter_key(line)))
		return out.stats.badfilter++;

	return network_parse(out, line);
}

/* ignore-previous-rules cancels only earlier rules, so the order is the ABP precedence. */
export function convert(text) {
	let out = {
		block: [], important: [], exception: [], document_exception: [],
		elemhide: [], generichide: [], generic: [], specific: [],
		cosmetic_exceptions: {},
		stats: {
			network: 0, network_exception: 0, cosmetic: 0,
			cosmetic_exception: 0, extended_cosmetic: 0, regex: 0,
			unsupported_option: 0, unsupported_domain: 0, non_ascii: 0,
			badfilter: 0
		}
	};
	let lines = map(split(text, '\n'), l => trim(l));
	let badfilters = {};
	let generic = [];
	let specific = [];

	for (let line in lines)
		if (match(line, /[$,]badfilter$|,badfilter,/))
			badfilters[badfilter_key(line)] = true;

	for (let line in lines)
		line_classify(out, badfilters, line);

	for (let selector in out.generic)
		push(generic, cosmetic_generic_rule(out, selector));

	for (let entry in out.specific)
		push(specific, cosmetic_specific_rule(out, entry));

	return {
		rules: filter([
			...generic, ...out.generichide, ...specific, ...out.elemhide,
			...out.block, ...out.exception, ...out.important,
			...out.document_exception
		], r => r != null),
		stats: out.stats
	};
};
