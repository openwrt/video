# Writing a ucode plymouth theme

A theme is one ucode program. The plugin compiles it when the splash is
shown, calls the object it returns, and gives it a plutovg canvas over
plymouth's own pixel buffer.

## The theme file

`ScriptFile` in the `.plymouth` file points at it. The program runs once
and must **return** an object; top-level function declarations stay
local to the program and are not visible to the plugin.

    import * as pvg from 'plutovg';

    function setup(mode) { }
    function frame(t) { }
    function paint(cv, x, y, w, h) { }
    function event(name, detail) { }

    return { setup: setup, frame: frame, paint: paint, event: event };

`setup(mode)` runs once, with the mode plymouth was started in
(`boot-up`, `shutdown`, `reboot`, `system-upgrade`, ...).

`frame(elapsed)` runs at `FrameRate`, default 30 Hz. It advances state
and declares what changed; it must not draw.

The splash starts in preinit, before the overlay is mounted and before
there is a bus to watch, so a theme has two phases to fill. The shipped
one spends the first growing its graph, a node at a time, for as long as
the phase lasts; `ubus-connected` completes whatever is left of it at
once and opens the second, where the traffic drawn is the traffic on the
bus.

`paint(canvas, x, y, w, h)` draws one damaged rectangle. The canvas is
clipped to it and is valid only for the duration of the call: keeping a
reference to it and drawing later writes into a buffer plymouth may have
moved on from.

`event(name, detail)` receives what the system is doing, see below.

## Damage is the contract

Nothing reaches the screen unless the theme asks for it. Call
`plymouth.damage(x, y, w, h)` for every region that changed since the
last frame, including the region something moved *away* from. The plugin
turns each call into a `ply_pixel_display_draw_area()` and plymouth
calls `paint()` back for that region.

Damaging the whole screen every frame works and is slow: a full 1080p
repaint of a busy scene costs about 27 ms against 0.06 ms for the
handful of boxes that actually changed. Damage tightly.

## The plymouth object

    plymouth.screens()     array of { width, height, scale }
    plymouth.damage(x, y, w, h)
    plymouth.mode()        the mode string
    plymouth.theme_dir()   where the theme was installed
    plymouth.elapsed()     seconds since the splash appeared
    plymouth.log(msg)      to plymouth's trace output

## Events

`event(name, detail)`:

    setup-time      mode passed to setup() instead
    status          a status line
    output          console output during boot
    message         a message plymouth wants shown
    system-update   an integer percentage, during sysupgrade
    idle            plymouth is handing the display over; frame() stops
                    until it takes the display back
    ubus-connected  the splash reached ubus
    ubus            { type, data, path, present }

`ubus` covers both the live event stream and, with `present` set, the
objects that already existed when the splash connected. Objects that
registered before then are state rather than events and would otherwise
never be seen: on a Rock 5B `service`, `system` and every `hotplug.*`
object exist before preinit runs.

`data` is the raw JSON of the event. For `ubus.object.add` the `path`
field is provided directly, so a theme does not have to parse it.

## Input

While a boot splash is up, the plugin watches every `/dev/input/event*`
node. Those present when the splash starts are opened at once; procd
announces later ones through `/etc/hotplug.d/input`, which hands them to
the plugin as `plymouth update --status=input:eventN`, since procd
replaces `/dev` after preinit and a watch on it would see none of them.
Such updates are not passed on to the theme. The first key, button or touch
press asks plymouthd to deactivate, the same as `plymouth deactivate`:
the splash stays loaded and the display goes to whoever is waiting for
it, a console on a system that runs nothing else. Pointer motion does not
count, and no other mode reacts to input.

## Handing the display over

`ubus call plymouth status` answers `{ "display": true }` while the
splash holds the display. Whenever it lets go, by input, `plymouth
deactivate` or quitting, the plugin hands procd a `plymouth.release`
service event, so a display client that should only start once the
splash is gone waits for it like kmscon does:

    start_service() {
        [ "$(ubus -S call plymouth status 2>/dev/null | \
            jsonfilter -e '@.display')" = true ] && return 0
        ...
    }

    service_triggers() {
        procd_add_raw_trigger plymouth.release 0 /etc/init.d/<name> start
    }

## Pictograms

A theme draws a pictogram per startup stage from its own
`pictograms/<name>.svg`, named after the init script in `/etc/init.d`.
A package that is not part of the theme drops one in as

    /usr/share/plymouth/pictograms/<init script>.svg

and needs nothing else: no registration, and no variable in the init
script.

Which services exist is not knowable before the overlay is mounted, so
the shipped theme reads `/etc/rc.d/S*` once the bus comes up and shows a
stage, in start order, for every enabled service it finds a pictogram
for. Enabled but not yet started is drawn dark; the stage lights when
the service's own ubus object appears. A service that is not installed
is never drawn, and neither is one with no pictogram. Nothing at all is
drawn before that: the first phase is the graph by itself.

A stage also appears under a bare ubus object name, so a service that
rc.d does not start can still be shown by a pictogram named after its
object.

Some services announce nothing on the bus at all. sysntpd is the usual
one: it reports a sync by calling `hotplug.ntp`, which is a ubus method
call rather than a broadcast, so no listener ever sees it. Such a
service reports itself with a status line instead,

    plymouth update --status="service:<init script>"

from wherever it does have a hook, and the theme lights that stage. The
shipped theme installs exactly that as /etc/hotplug.d/ntp/00-plymouth.

## Measuring

`plymouth-theme-bench <theme dir> [width] [height] [frames] [png]` runs
a theme against an offscreen surface through the same contract and
reports setup, frame and paint cost with the share of the screen
repainted per frame. It needs no display, so a theme can be compared
across targets.
