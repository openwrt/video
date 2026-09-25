#!/usr/bin/env ucode

import * as pvg from 'plutovg';

const DEFAULT_FRAMES = 120;
const VERSION = '@VERSION@';

function usage(code) {
	print(`plymouth-theme-bench ${VERSION}\n`);
	print("usage: plymouth-theme-bench [theme dir] [width] [height] " +
	      "[frames] [png]\n");
	exit(code);
}

switch (ARGV[0]) {
case '--version':
case '-version':
case 'version':
case '-v':
case '-V':
	usage(0);
case '--help':
case '-help':
case '-h':
case '-?':
	usage(0);
}

let theme_dir = ARGV[0] ?? '/usr/share/plymouth/themes/openwrt-demo';
let width = int(ARGV[1] ?? 1920);
let height = int(ARGV[2] ?? 1080);
let frames = int(ARGV[3] ?? DEFAULT_FRAMES);
let png_out = ARGV[4];

let damage = [];
let started = 0.0;

function now() {
	let t = clock(true);

	return t[0] + t[1] / 1000000000.0;
}

function rect_merge(list) {
	let out = [];
	let keep;

	for (let i = 0; i < length(list); i++) {
		keep = true;

		for (let j = 0; j < length(out); j++) {
			if (list[i].x >= out[j].x && list[i].y >= out[j].y &&
			    list[i].x + list[i].w <= out[j].x + out[j].w &&
			    list[i].y + list[i].h <= out[j].y + out[j].h) {
				keep = false;
				break;
			}
		}

		if (keep)
			push(out, list[i]);
	}

	return out;
}

global.plymouth = {
	screens: function() {
		return [ { width: width, height: height, scale: 1 } ];
	},

	damage: function(x, y, w, h) {
		push(damage, { x: x, y: y, w: w, h: h });

		return true;
	},

	mode: function() {
		return 'boot-up';
	},

	theme_dir: function() {
		return theme_dir;
	},

	elapsed: function() {
		return now() - started;
	},

	log: function(msg) {
		warn(`theme: ${msg}\n`);
	}
};

started = now();

let script = theme_dir + '/openwrt.uc';
let entry = loadfile(script);
let theme;

if (!entry) {
	warn(`cannot load ${script}\n`);
	exit(1);
}

theme = entry();

if (type(theme) != 'object' || type(theme.paint) != 'function') {
	warn(`${script} returned no theme object\n`);
	exit(1);
}

let surface = pvg.surface(width, height);
let canvas = pvg.canvas(surface);
let setup_start = now();

theme.setup('boot-up');

let setup_ms = (now() - setup_start) * 1000.0;
let full = rect_merge(damage);
let first_start = now();

for (let i = 0; i < length(full); i++)
	theme.paint(canvas, full[i].x, full[i].y, full[i].w, full[i].h);

let first_ms = (now() - first_start) * 1000.0;

let frame_total = 0.0;
let paint_total = 0.0;
let pixels = 0;
let t0, t1, rects;

const EVENT_EVERY = 20;

if (type(theme.event) == 'function')
	theme.event('ubus-connected', null);

for (let f = 0; f < frames; f++) {
	damage = [];

	if (type(theme.event) == 'function' && f % EVENT_EVERY == 0) {
		theme.event('ubus', { type: 'ubus.object.add',
				      data: '{"path":"hostapd.phy0"}' });
		theme.event('progress', { duration: 20.0,
					  fraction: f / (frames * 1.0) });
	}

	t0 = now();
	theme.frame(f / 30.0);
	t1 = now();
	frame_total += t1 - t0;

	rects = rect_merge(damage);
	t0 = now();

	for (let i = 0; i < length(rects); i++) {
		theme.paint(canvas, rects[i].x, rects[i].y, rects[i].w, rects[i].h);
		pixels += rects[i].w * rects[i].h;
	}

	paint_total += now() - t0;
}

printf("theme      %s\n", theme_dir);
printf("geometry   %dx%d, %d frames\n", width, height, frames);
printf("setup      %8.2f ms  (scene build and static render)\n", setup_ms);
printf("first draw %8.2f ms  (full screen)\n", first_ms);
printf("frame()    %8.3f ms/frame\n", frame_total * 1000.0 / frames);
printf("paint()    %8.3f ms/frame\n", paint_total * 1000.0 / frames);
printf("total      %8.3f ms/frame  %7.1f fps\n",
       (frame_total + paint_total) * 1000.0 / frames,
       frames / (frame_total + paint_total));
printf("repainted  %8.2f %% of the screen per frame\n",
       100.0 * pixels / frames / (width * height));

if (png_out) {
	surface.write_png(png_out);
	printf("wrote      %s\n", png_out);
}
