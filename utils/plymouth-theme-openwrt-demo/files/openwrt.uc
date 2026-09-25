import * as pvg from 'plutovg';
import * as psvg from 'plutosvg';
import { open, lsdir } from 'fs';

const PI = 3.14159265358979;

const CYAN = { r: 0.0, g: 0.710, b: 0.886, a: 1.0 };
const NAVY = { r: 0.0, g: 0.169, b: 0.286, a: 1.0 };
const DEEP = { r: 0.0, g: 0.082, b: 0.149, a: 1.0 };
const WHITE = { r: 1.0, g: 1.0, b: 1.0, a: 1.0 };
const WARM = { r: 1.0, g: 0.83, b: 0.45, a: 1.0 };

const HUB_FILL = { r: 0.80, g: 0.93, b: 1.0, a: 0.90 };
const LEAF_FILL = { r: 0.55, g: 0.78, b: 0.92, a: 0.58 };

const R_CORE = 0.78;
const R_HUB = 0.50;
const R_LEAF = 0.30;

const CORE_X = 0.50;
const CORE_Y = 0.732;
const RING_R = 0.103;
const FAN_R = 0.062;
const ASPECT = 1.25;
const FAN_ANGLE = 0.55;
const RING_VARY = 0.24;
const FAN_VARY = 0.30;
const BRANCH_MIN = 4;
const BRANCH_MAX = 5;
const CLIENT_MIN = 2;
const CLIENT_MAX = 3;
const ANGLE_JITTER = 0.14;
const PEER_BOW = 0.30;

const HOP_FRAMES = 11.0;
const HOP_DWELL = 3.0;
const LAUNCH_BACK = 0.055;
const LAUNCH_HOLD = 0.18;
const LAUNCH_CRUISE = 0.50;
const FLOW_GAP = 18;
const FLOW_CAP = 22;
const TRAIL = 3;

const LOGO_HEIGHT = 0.26;
const BAND_TOP = 0.58;
const ROW_Y = 0.965;

const PULSE_LIFE = 22.0;
const PULSE_MAX = 14;
const ICON_DIM = { r: 0.34, g: 0.48, b: 0.60, a: 1.0 };
const ADDON_DIR = '/usr/share/plymouth/pictograms';
const RC_DIR = '/etc/rc.d';
const REVEAL_SPACING = 1.15;

let screen;
let backdrop;
let unit = 1.0;
let icon_size = 1.0;
let packet_radius = 1.0;
let glow_radius = 1.0;
let pulse_r0 = 1.0;
let pulse_r1 = 1.0;
let nodes = [];
let links = [];
let peers = [];
let clients = [];
let hubs = [];
let core;
let reveal = [];
let revealed = 0;
let reveal_at = 0.0;
let endpoints = {};
let next_endpoint = 0;
let on_bus = false;
let dismantle = false;
let logo_box;

let packets = [];
let pulses = [];
let stage_list = [];
let stage_boxes = [];
let icons = {};
let flow_timer = 0;
let primed = false;
let rng_state = 1;

function seed_pick() {
	let fh = open('/dev/urandom', 'r');
	let raw, n;

	if (!fh)
		return 1;

	raw = fh.read(4);
	fh.close();

	if (!raw || length(raw) < 4)
		return 1;

	n = 0;

	for (let i = 0; i < 4; i++)
		n = (n * 251 + ord(raw, i)) % 2147483647;

	return n + 1;
}

function rnd() {
	rng_state = (rng_state * 1103515245 + 12345) % 2147483648;

	return rng_state / 2147483648.0;
}

function rnd_int(lo, hi) {
	return lo + int(rnd() * (hi - lo + 1));
}

function fsin(x) {
	let x2;

	while (x > PI)
		x -= 2.0 * PI;
	while (x < -PI)
		x += 2.0 * PI;

	x2 = x * x;

	return x * (1.0 - x2 / 6.0 * (1.0 - x2 / 20.0 *
	       (1.0 - x2 / 42.0 * (1.0 - x2 / 72.0))));
}

function fcos(x) {
	return fsin(x + PI * 0.5);
}

function node_add(x, y, r, kind, parent) {
	push(nodes, { x: x, y: y, r: r, kind: kind, parent: parent,
		      cx: x, cy: y });

	return length(nodes) - 1;
}

function curve_set(idx) {
	let n = nodes[idx];
	let p = nodes[n.parent];

	n.cx = (n.x + p.x) * 0.5;
	n.cy = (n.y + p.y) * 0.5;
}

function curve_at(idx, t) {
	let n = nodes[idx];
	let p = nodes[n.parent];
	let u = 1.0 - t;
	let a = u * u;
	let b = 2.0 * u * t;
	let c = t * t;

	return {
		x: a * p.x + b * n.cx + c * n.x,
		y: a * p.y + b * n.cy + c * n.y
	};
}

function link_add(a, b, kind) {
	push(links, { a: a, b: b, kind: kind });
}

function peer_link() {
	let i, a, b, mx, my;

	if (length(hubs) < 3)
		return;

	i = rnd_int(1, length(hubs) - 2);
	a = hubs[i];
	b = hubs[i + 1];
	mx = (nodes[a].x + nodes[b].x) * 0.5;
	my = (nodes[a].y + nodes[b].y) * 0.5;

	push(peers, {
		a: a,
		b: b,
		kind: 'peer',
		cx: mx + (mx - nodes[core].x) * PEER_BOW,
		cy: my + (my - nodes[core].y) * PEER_BOW
	});
}

function branch_grow(angle) {
	let ring = screen.height * RING_R *
		   (1.0 - RING_VARY * 0.5 + RING_VARY * rnd());
	let fan = screen.height * FAN_R;
	let bx = nodes[core].x + ring * ASPECT * fcos(angle);
	let by = nodes[core].y + ring * fsin(angle);
	let hub = node_add(bx, by, unit * R_HUB, 'hub', core);
	let count = rnd_int(CLIENT_MIN, CLIENT_MAX);
	let reach, phi, idx;

	link_add(core, hub, 'trunk');
	curve_set(hub);
	push(hubs, hub);

	for (let k = 0; k < count; k++) {
		phi = angle + (k - (count - 1.0) * 0.5) * FAN_ANGLE;
		reach = fan * (1.0 - FAN_VARY * 0.5 + FAN_VARY * rnd());
		idx = node_add(bx + reach * ASPECT * fcos(phi),
			       by + reach * fsin(phi),
			       unit * R_LEAF, 'leaf', hub);
		link_add(hub, idx, 'edge');
		curve_set(idx);
		push(clients, idx);
	}
}

function topo_build() {
	let w = screen.width;
	let h = screen.height;
	let count = rnd_int(BRANCH_MIN, BRANCH_MAX);

	nodes = [];
	links = [];
	peers = [];
	clients = [];
	hubs = [];

	core = node_add(w * CORE_X, h * CORE_Y, unit * R_CORE, 'router', null);
	push(hubs, core);

	for (let i = 0; i < count; i++)
		branch_grow(-PI * 0.5 + PI * (2.0 * i + 1.0) / count +
			    (rnd() - 0.5) * ANGLE_JITTER);

	peer_link();

	if (rnd() < 0.5)
		peer_link();

	reveal = [ core ];

	for (let i = 1; i < length(hubs); i++)
		push(reveal, hubs[i]);

	for (let i = 0; i < length(clients); i++)
		push(reveal, clients[i]);

	for (let i = 0; i < length(peers); i++)
		push(reveal, -1 - i);
}

function ease_bullet(t) {
	let a = LAUNCH_HOLD;
	let b = LAUNCH_CRUISE;
	let pb, vb, s;

	if (t <= 0.0)
		return 0.0;
	if (t >= 1.0)
		return 1.0;
	if (t < a)
		return -LAUNCH_BACK * fsin(PI * t / a);

	pb = 1.0 / (1.0 + 2.0 * (1.0 - b) / (b - a));
	vb = 2.0 * pb / (b - a);
	s = (t - a) / (b - a);

	if (t < b)
		return pb * s * s;

	return pb + vb * (t - b);
}

function ancestors(idx) {
	let chain = [];
	let at = idx;

	while (at != null) {
		push(chain, at);
		at = nodes[at].parent;
	}

	return chain;
}

function path_between(a, b) {
	let up = ancestors(a);
	let down = ancestors(b);
	let meet = null;
	let path = [];

	for (let i = 0; i < length(up) && meet == null; i++) {
		for (let j = 0; j < length(down); j++) {
			if (up[i] != down[j])
				continue;

			meet = up[i];
			break;
		}
	}

	if (meet == null)
		return null;

	for (let i = 0; i < length(up); i++) {
		push(path, up[i]);

		if (up[i] == meet)
			break;
	}

	for (let i = length(down) - 1; i >= 0; i--) {
		if (down[i] == meet)
			continue;

		push(path, down[i]);
	}

	return (length(path) > 1) ? path : null;
}

function packet_add(path, kind, delay, colour) {
	push(packets, {
		path: path,
		kind: kind,
		colour: colour,
		hop: 0,
		age: -delay,
		dwell: 0.0,
		box: null
	});
}

function flow_inward() {
	let src = clients[rnd_int(0, length(clients) - 1)];
	let path = path_between(src, core);

	if (path)
		packet_add(path, 'request', 0.0, WHITE);
}

function flow_lan() {
	let a = clients[rnd_int(0, length(clients) - 1)];
	let b = clients[rnd_int(0, length(clients) - 1)];
	let path;

	if (a == b)
		return;

	path = path_between(a, b);

	if (path)
		packet_add(path, 'request', 0.0, WHITE);
}

function flow_stream() {
	let dst = clients[rnd_int(0, length(clients) - 1)];
	let path = path_between(core, dst);

	if (!path)
		return;

	for (let i = 0; i < 5; i++)
		packet_add(path, 'stream', i * 5.0, CYAN);
}

function flow_multicast() {
	let count = rnd_int(3, 4);
	let dst, path;

	for (let i = 0; i < count; i++) {
		dst = clients[rnd_int(0, length(clients) - 1)];
		path = path_between(core, dst);

		if (path)
			packet_add(path, 'multicast', i * 2.0, WARM);
	}
}

function flow_spawn() {
	let pick = rnd();

	if (!length(clients))
		return;

	if (length(packets) > FLOW_CAP)
		return;

	if (pick < 0.45)
		flow_inward();
	else if (pick < 0.70)
		flow_lan();
	else if (pick < 0.90)
		flow_stream();
	else
		flow_multicast();
}

function packet_at(p, back) {
	let age = p.age - back;
	let hop = p.hop;
	let from, to, t;

	while (age < 0.0 && hop > 0) {
		hop--;
		age += HOP_FRAMES;
	}

	if (hop > length(p.path) - 2)
		hop = length(p.path) - 2;
	if (hop < 0)
		hop = 0;

	from = p.path[hop];
	to = p.path[hop + 1];
	t = age / HOP_FRAMES;

	if (t < 0.0)
		t = 0.0;
	if (t > 1.0)
		t = 1.0;

	t = ease_bullet(t);

	if (nodes[to].parent == from)
		return curve_at(to, t);

	if (nodes[from].parent == to)
		return curve_at(from, 1.0 - t);

	return {
		x: nodes[from].x + (nodes[to].x - nodes[from].x) * t,
		y: nodes[from].y + (nodes[to].y - nodes[from].y) * t
	};
}

function packet_box(p) {
	let head = packet_at(p, 0);
	let tail = packet_at(p, TRAIL);
	let pad = glow_radius + 3.0;
	let x0 = ((head.x < tail.x) ? head.x : tail.x) - pad;
	let y0 = ((head.y < tail.y) ? head.y : tail.y) - pad;
	let x1 = ((head.x > tail.x) ? head.x : tail.x) + pad;
	let y1 = ((head.y > tail.y) ? head.y : tail.y) + pad;

	return {
		x: int(x0),
		y: int(y0),
		w: int(x1 - x0) + 1,
		h: int(y1 - y0) + 1
	};
}

function packet_reply(p) {
	let back = [];

	for (let i = length(p.path) - 1; i >= 0; i--)
		push(back, p.path[i]);

	packet_add(back, 'response', 0.0, CYAN);
}

function pulse_spawn(idx) {
	if (!length(nodes))
		return;

	push(pulses, { n: idx ?? rnd_int(0, length(nodes) - 1), age: 0.0 });

	if (length(pulses) > PULSE_MAX)
		shift(pulses);
}

function pulse_box(p) {
	let n = nodes[p.n];
	let r = pulse_r1 + 3.0;

	return {
		x: int(n.x - r),
		y: int(n.y - r),
		w: int(r * 2.0) + 1,
		h: int(r * 2.0) + 1
	};
}

function key_safe(key) {
	return key && index(key, '/') < 0 && index(key, '..') < 0;
}

function icon_get(key) {
	let doc;

	if (exists(icons, key))
		return icons[key];

	doc = psvg.document_from_file(plymouth.theme_dir() +
				      '/pictograms/' + key + '.svg');

	if (!doc)
		doc = psvg.document_from_file(ADDON_DIR + '/' + key + '.svg');

	icons[key] = doc;

	return doc;
}

function icon_draw(cv, doc, cx, cy, colour) {
	let scale = icon_size / doc.height();

	cv.save();
	cv.translate(cx - doc.width() * scale * 0.5, cy - icon_size * 0.5);
	cv.scale(scale, scale);
	doc.render(cv, null, colour);
	cv.restore();
}

function stage_index(key) {
	for (let i = 0; i < length(stage_list); i++)
		if (stage_list[i].key == key)
			return i;

	return -1;
}

function stage_layout() {
	let pitch = unit * 4.2;
	let x0 = screen.width * 0.5 - pitch * length(stage_list) * 0.5;

	stage_boxes = [];

	for (let i = 0; i < length(stage_list); i++)
		push(stage_boxes, { x: x0 + pitch * (i + 0.5),
				    y: screen.height * ROW_Y });
}

function stage_row_damage() {
	plymouth.damage(0, int(screen.height * ROW_Y - icon_size),
			screen.width, int(icon_size * 2.0));
}

function stage_reach(key) {
	let i;

	if (!key_safe(key))
		return;

	i = stage_index(key);

	if (i >= 0) {
		if (stage_list[i].on)
			return;

		stage_list[i].on = true;
	} else {
		if (!icon_get(key))
			return;

		push(stage_list, { key: key, on: true });
		stage_layout();
	}

	stage_row_damage();

	if (length(hubs))
		pulse_spawn(hubs[rnd_int(0, length(hubs) - 1)]);
}

function stage_add(key) {
	if (!key_safe(key) || stage_index(key) >= 0)
		return;

	if (!icon_get(key))
		return;

	push(stage_list, { key: key, on: false });
}

function stages_init() {
	stage_list = [];
	stage_layout();
}

/* The enabled set only exists once the overlay is mounted, so this runs
 * when the bus comes up rather than at setup time.
 */
function services_scan() {
	let entries, found, m;

	if (dismantle)
		return;

	entries = lsdir(RC_DIR) ?? [];
	found = [];

	for (let i = 0; i < length(entries); i++) {
		m = match(entries[i], /^S([0-9]+)(.+)$/);

		if (!m)
			continue;

		push(found, { order: int(m[1]), name: m[2] });
	}

	sort(found, (a, b) => a.order - b.order);

	for (let i = 0; i < length(found); i++)
		stage_add(found[i].name);

	stage_layout();
	stage_row_damage();
}

function link_draw(cv, l) {
	let trunk = (l.kind == 'trunk');
	let peer = (l.kind == 'peer');

	cv.set_line_width(unit * (trunk ? 0.17 : (peer ? 0.10 : 0.11)));
	cv.set_color({ r: CYAN.r, g: CYAN.g, b: CYAN.b,
		       a: trunk ? 0.50 : (peer ? 0.16 : 0.30) });
	cv.move_to(nodes[l.a].x, nodes[l.a].y);

	if (peer)
		cv.quad_to(l.cx, l.cy, nodes[l.b].x, nodes[l.b].y);
	else
		cv.line_to(nodes[l.b].x, nodes[l.b].y);

	cv.stroke();
}

function node_draw(cv, n) {
	if (n.kind == 'router') {
		cv.set_color({ r: CYAN.r, g: CYAN.g, b: CYAN.b, a: 0.24 });
		cv.circle(n.x, n.y, n.r * 2.4);
		cv.fill();
	}

	cv.set_color((n.kind == 'leaf') ? LEAF_FILL : HUB_FILL);
	cv.circle(n.x, n.y, n.r);
	cv.fill();
}

function step_shown(step) {
	for (let i = 0; i < revealed; i++)
		if (reveal[i] == step)
			return true;

	return false;
}

function span_box(ax, ay, bx, by, pad) {
	let x0 = ((ax < bx) ? ax : bx) - pad;
	let y0 = ((ay < by) ? ay : by) - pad;
	let x1 = ((ax > bx) ? ax : bx) + pad;
	let y1 = ((ay > by) ? ay : by) + pad;

	return {
		x: int(x0),
		y: int(y0),
		w: int(x1 - x0) + 1,
		h: int(y1 - y0) + 1
	};
}

function span_damage(ax, ay, bx, by, pad) {
	let box = span_box(ax, ay, bx, by, pad);

	plymouth.damage(box.x, box.y, box.w, box.h);
}

function reveal_peer(idx) {
	let cv = pvg.canvas(backdrop);
	let l = peers[idx];

	link_draw(cv, l);
	span_damage(nodes[l.a].x, nodes[l.a].y, nodes[l.b].x, nodes[l.b].y,
		    unit * 4.0);
}

function reveal_node(idx) {
	let cv = pvg.canvas(backdrop);
	let n = nodes[idx];
	let p = (n.parent != null) ? nodes[n.parent] : n;

	for (let i = 0; i < length(links); i++)
		if (links[i].b == idx)
			link_draw(cv, links[i]);

	node_draw(cv, n);
	span_damage(n.x, n.y, p.x, p.y, n.r * 3.0 + unit);
	pulse_spawn(idx);
}

function reveal_step() {
	let step;

	if (revealed >= length(reveal))
		return;

	step = reveal[revealed++];

	if (step < 0)
		return reveal_peer(-1 - step);

	reveal_node(step);
}

function logo_draw(cv) {
	let doc = psvg.document_from_file(plymouth.theme_dir() + '/logo.svg');
	let scale, lw, lh;

	if (!doc) {
		plymouth.log('logo.svg missing');
		return;
	}

	lh = screen.height * LOGO_HEIGHT;
	scale = lh / doc.height();
	lw = doc.width() * scale;
	logo_box = {
		x: (screen.width - lw) / 2.0,
		y: screen.height * BAND_TOP - lh - screen.height * 0.04,
		w: lw,
		h: lh
	};

	cv.save();
	cv.translate(logo_box.x, logo_box.y);
	cv.scale(scale, scale);
	doc.render(cv);
	cv.restore();
}

function ground_draw(cv) {
	cv.set_radial_gradient(screen.width * 0.5, screen.height * 0.40,
			       screen.width * 0.80,
			       screen.width * 0.5, screen.height * 0.40, 0.0,
			       'pad',
			       [ { offset: 0.0, color: NAVY },
				 { offset: 1.0, color: DEEP } ]);
	cv.fill_rect(0, 0, screen.width, screen.height);
}

function backdrop_draw() {
	let cv = pvg.canvas(backdrop);

	ground_draw(cv);
	logo_draw(cv);
}

function overlaps(box, x, y, w, h) {
	if (!box)
		return false;

	return !(box.x > x + w || box.x + box.w < x ||
		 box.y > y + h || box.y + box.h < y);
}

/* Taking a node off the backdrop means putting back what was under it.
 */
function backdrop_patch(x, y, w, h) {
	let cv = pvg.canvas(backdrop);
	let n;

	cv.save();
	cv.clip_rect(x, y, w, h);
	ground_draw(cv);

	if (overlaps(logo_box, x, y, w, h))
		logo_draw(cv);

	for (let i = 0; i < length(links); i++)
		if (step_shown(links[i].b))
			link_draw(cv, links[i]);

	for (let i = 0; i < length(peers); i++)
		if (step_shown(-1 - i))
			link_draw(cv, peers[i]);

	for (let i = 0; i < length(nodes); i++) {
		n = nodes[i];

		if (step_shown(i))
			node_draw(cv, n);
	}

	cv.restore();
}

function hide_step() {
	let step, n, p, box;

	if (revealed <= 0)
		return;

	step = reveal[--revealed];

	if (step < 0) {
		p = peers[-1 - step];
		box = span_box(nodes[p.a].x, nodes[p.a].y,
			       nodes[p.b].x, nodes[p.b].y, unit * 4.0);
	} else {
		n = nodes[step];
		p = (n.parent != null) ? nodes[n.parent] : n;
		box = span_box(n.x, n.y, p.x, p.y, n.r * 3.0 + unit);
	}

	backdrop_patch(box.x, box.y, box.w, box.h);
	plymouth.damage(box.x, box.y, box.w, box.h);

	if (step >= 0)
		pulse_spawn(step);
}

function setup(mode) {
	let list = plymouth.screens();

	if (!length(list)) {
		plymouth.log('no pixel display');
		return;
	}

	screen = list[0];
	rng_state = seed_pick();

	unit = screen.height / 90.0;
	icon_size = unit * 2.5;
	packet_radius = unit * 0.27;
	glow_radius = unit * 0.67;
	pulse_r0 = unit * 0.45;
	pulse_r1 = unit * 2.70;

	dismantle = (mode != 'boot-up');

	backdrop = pvg.surface(screen.width, screen.height);
	topo_build();
	stages_init();
	backdrop_draw();

	if (dismantle)
		while (revealed < length(reveal))
			reveal_step();

	plymouth.damage(0, 0, screen.width, screen.height);
}

function packets_step() {
	let p, was, now, x0, y0, x1, y1;

	for (let i = length(packets) - 1; i >= 0; i--) {
		p = packets[i];
		was = p.box ?? packet_box(p);

		if (p.dwell > 0.0) {
			p.dwell -= 1.0;
		} else {
			p.age += 1.0;

			if (p.age >= HOP_FRAMES) {
				p.age = 0.0;
				p.hop++;
				p.dwell = HOP_DWELL;
			}
		}

		if (p.hop > length(p.path) - 2) {
			plymouth.damage(was.x, was.y, was.w, was.h);
			pulse_spawn(p.path[length(p.path) - 1]);

			if (p.kind == 'request')
				packet_reply(p);

			splice(packets, i, 1);
			continue;
		}

		now = packet_box(p);
		p.box = now;
		p.pts = [];

		for (let k = 0; k <= TRAIL; k++)
			push(p.pts, packet_at(p, k));

		x0 = (was.x < now.x) ? was.x : now.x;
		y0 = (was.y < now.y) ? was.y : now.y;
		x1 = (was.x + was.w > now.x + now.w) ? was.x + was.w
						     : now.x + now.w;
		y1 = (was.y + was.h > now.y + now.h) ? was.y + was.h
						     : now.y + now.h;

		plymouth.damage(x0, y0, x1 - x0, y1 - y0);
	}
}

function frame(t) {
	let now;

	if (!backdrop)
		return;

	if (!primed) {
		primed = true;
		plymouth.damage(0, 0, screen.width, screen.height);
	}

	packets_step();

	if (dismantle) {
		if (t >= reveal_at) {
			reveal_at = t + REVEAL_SPACING;
			hide_step();
		}
	} else if (revealed < length(reveal)) {
		if (t >= reveal_at) {
			reveal_at = t + REVEAL_SPACING;
			reveal_step();
		}
	} else if (on_bus && ++flow_timer >= FLOW_GAP) {
		flow_timer = 0;
		flow_spawn();
	}

	for (let i = length(pulses) - 1; i >= 0; i--) {
		now = pulse_box(pulses[i]);
		plymouth.damage(now.x, now.y, now.w, now.h);
		pulses[i].age += 1.0;

		if (pulses[i].age > PULSE_LIFE)
			splice(pulses, i, 1);
	}
}

function pulses_paint(cv, x, y, w, h) {
	let n, f, r;

	for (let i = 0; i < length(pulses); i++) {
		n = nodes[pulses[i].n];
		f = pulses[i].age / PULSE_LIFE;
		r = pulse_r0 + (pulse_r1 - pulse_r0) * f;

		if (n.x + r < x || n.x - r > x + w)
			continue;
		if (n.y + r < y || n.y - r > y + h)
			continue;

		cv.set_color({ r: CYAN.r, g: CYAN.g, b: CYAN.b,
			       a: 0.55 * (1.0 - f) });
		cv.set_line_width(unit * 0.15);
		cv.circle(n.x, n.y, r);
		cv.stroke();
	}
}

function packet_paint(cv, p, x, y, w, h) {
	let c = p.colour;
	let pt, f;

	for (let k = TRAIL; k >= 0; k--) {
		pt = p.pts[k];

		if (pt.x + glow_radius < x || pt.x - glow_radius > x + w)
			continue;
		if (pt.y + glow_radius < y || pt.y - glow_radius > y + h)
			continue;

		if (k > 0) {
			f = 1.0 - k / (TRAIL + 1.0);
			cv.set_color({ r: c.r, g: c.g, b: c.b, a: 0.32 * f });
			cv.circle(pt.x, pt.y, packet_radius * f);
			cv.fill();
			continue;
		}

		cv.set_color({ r: c.r, g: c.g, b: c.b, a: 0.28 });
		cv.circle(pt.x, pt.y, glow_radius);
		cv.fill();
		cv.set_color(c);
		cv.circle(pt.x, pt.y, (p.kind == 'stream')
			  ? packet_radius * 0.7 : packet_radius);
		cv.fill();
	}
}

function stages_paint(cv, x, y, w, h) {
	let b, on, doc;

	for (let i = 0; i < length(stage_boxes); i++) {
		b = stage_boxes[i];
		on = stage_list[i].on;

		if (b.x + icon_size < x || b.x - icon_size > x + w)
			continue;
		if (b.y + icon_size < y || b.y - icon_size > y + h)
			continue;

		if (on) {
			cv.set_color({ r: CYAN.r, g: CYAN.g, b: CYAN.b,
				       a: 0.85 });
			cv.circle(b.x, b.y, icon_size * 0.72);
			cv.fill();
		}

		doc = icons[stage_list[i].key];

		if (doc)
			icon_draw(cv, doc, b.x, b.y, on ? WHITE : ICON_DIM);
	}
}

function paint(cv, x, y, w, h) {
	let b;

	if (!backdrop)
		return;

	cv.set_texture(backdrop, 'plain', 1.0);
	cv.fill_rect(x, y, w, h);

	pulses_paint(cv, x, y, w, h);

	for (let i = 0; i < length(packets); i++) {
		b = packets[i].box;

		if (!packets[i].pts || !b)
			continue;
		if (b.x + b.w < x || b.x > x + w)
			continue;
		if (b.y + b.h < y || b.y > y + h)
			continue;

		packet_paint(cv, packets[i], x, y, w, h);
	}

	stages_paint(cv, x, y, w, h);
}

function object_stage(path) {
	if (path == 'service' || path == 'system' || path == 'container' ||
	    substr(path, 0, 8) == 'hotplug.')
		return 'procd';
	if (path == 'block')
		return 'blockd';
	if (substr(path, 0, 7) == 'network')
		return 'network';
	if (substr(path, 0, 7) == 'dnsmasq')
		return 'dnsmasq';
	if (path == 'dhcp')
		return 'odhcpd';
	if (substr(path, 0, 7) == 'hostapd' || path == 'wpa_supplicant' ||
	    path == 'iwinfo')
		return 'wpad';
	if (path == 'uhttpd' || substr(path, 0, 4) == 'luci')
		return 'uhttpd';

	return null;
}

function endpoint_for(path) {
	let idx;

	if (exists(endpoints, path))
		return endpoints[path];

	if (!length(clients))
		return null;

	idx = clients[next_endpoint % length(clients)];
	next_endpoint++;
	endpoints[path] = idx;

	return idx;
}

function flow_endpoint(path) {
	let idx = endpoint_for(path);
	let p;

	if (idx == null || revealed < length(reveal))
		return;

	p = path_between(idx, core);

	if (p)
		packet_add(p, 'request', 0.0, WHITE);
}

function ubus_event(detail) {
	let payload;

	if (detail.type == 'ubus.object.add') {
		payload = detail.path;

		if (!payload) {
			try {
				payload = json(detail.data ?? '{}')?.path;
			} catch (e) {
				payload = null;
			}
		}

		if (payload) {
			stage_reach(object_stage(payload) ?? payload);
			flow_endpoint(payload);
		}

		return;
	}

	flow_endpoint(detail.path ?? detail.type ?? 'bus');
}

function event(name, detail) {
	if (name == 'ubus-connected') {
		on_bus = true;

		while (revealed < length(reveal))
			reveal_step();

		services_scan();
		return;
	}

	if (name == 'ubus' && type(detail) == 'object')
		return ubus_event(detail);

	if (name == 'status' && substr(detail ?? '', 0, 8) == 'service:')
		stage_reach(substr(detail, 8));

}

return {
	setup: setup,
	frame: frame,
	paint: paint,
	event: event
};
