#!/usr/bin/ucode

import { lsdir, readfile, readlink } from 'fs';
import * as ubus from 'ubus';
import * as uloop from 'uloop';

function proc_holds_card(pid) {
	for (let fd in lsdir(`/proc/${pid}/fd`) ?? [])
		if (match(readlink(`/proc/${pid}/fd/${fd}`) ?? '',
			  /^\/dev\/dri\/card[0-9]+$/))
			return true;

	return false;
}

function holders_find() {
	let found = [];

	for (let pid in lsdir('/proc') ?? []) {
		if (!match(pid, /^[0-9]+$/))
			continue;
		if (trim(readfile(`/proc/${pid}/comm`) ?? '') == 'plymouthd')
			continue;
		if (proc_holds_card(pid))
			push(found, +pid);
	}

	return found;
}

function proc_ppid(pid) {
	let stat = readfile(`/proc/${pid}/stat`) ?? '';

	// the command name may itself contain spaces and parentheses
	return +split(trim(substr(stat, rindex(stat, ')') + 1)), ' ')[1];
}

function instances_list() {
	let reply = ubus.call({ object: 'service', method: 'list' }) ?? {};
	let list = [];

	for (let service, data in reply)
		for (let name, instance in data.instances ?? {})
			if (instance.pid)
				push(list, { service, name, pid: instance.pid });

	return list;
}

function instance_owning(instances, pid) {
	while (pid > 1) {
		for (let instance in instances)
			if (instance.pid == pid)
				return instance;

		pid = proc_ppid(pid);
	}

	return null;
}

let holders = holders_find();

if (!length(holders))
	exit(0);

let instances = instances_list();
let pending = {};

for (let pid in holders) {
	let owner = instance_owning(instances, pid);

	if (owner)
		pending[`${owner.service}/${owner.name}`] = owner;
	if (!owner || owner.pid != pid)
		system([ 'kill', '-TERM', `${pid}` ]);
}

if (!length(pending))
	exit(0);

uloop.init();

// procd applies the service's own term_timeout and respawns what it did not
// stop; it reports exits as notifications of its service object
let procd = ubus.subscriber((req) => {
	if (req.type != 'instance.stop')
		return;

	delete pending[`${req.data.service}/${req.data.instance}`];

	if (!length(pending))
		uloop.end();
});

procd.subscribe('service');

for (let key, owner in pending)
	ubus.call({
		object: 'service',
		method: 'delete',
		data: { name: owner.service, instance: owner.name }
	});

uloop.run();
