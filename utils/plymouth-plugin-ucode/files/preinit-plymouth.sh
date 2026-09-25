plymouth_preinit_devnodes() {
	local node name major minor

	for node in /sys/class/drm/*/dev; do
		[ -e "$node" ] || continue

		name="${node%/dev}"
		name="${name##*/}"

		case "$name" in
		*-*) continue ;;
		esac

		[ -e "/dev/dri/$name" ] && continue

		IFS=: read major minor < "$node"
		mkdir -p /dev/dri
		mknod "/dev/dri/$name" c "$major" "$minor" 2>/dev/null
	done
}

plymouth_preinit_start() {
	local card found

	[ -x /usr/sbin/plymouthd ] || return 0

	plymouth_preinit_devnodes

	for card in /dev/dri/card*; do
		[ -e "$card" ] && found="$card"
	done

	[ -n "$found" ] || return 0

	/usr/sbin/plymouthd --mode=boot --ignore-serial-consoles \
		--kernel-command-line="$(cat /proc/cmdline) splash" || return 0

	/usr/bin/plymouth show-splash
}

plymouth_preinit_failsafe() {
	/usr/bin/plymouth quit 2>/dev/null
}

boot_hook_add preinit_main plymouth_preinit_start
boot_hook_add failsafe plymouth_preinit_failsafe
