[ -x /usr/sbin/plymouthd ] || return 0

# common.sh sorts before this file, so this replaces its v() rather than
# being replaced by it
v() {
	_v "$(date) upgrade: $@"
	logger -p info -t upgrade "$@"
	/usr/bin/plymouth update --status="$*" 2>/dev/null
}

# Sourced by sysupgrade itself as well, where nothing is decided yet.
case "$0" in
*/stage2)
	# install_bin pulls in what a plugin or module links against;
	# install_file takes files only, not directories
	RAMFS_COPY_BIN="$RAMFS_COPY_BIN /usr/bin/plymouth /usr/sbin/plymouthd \
		$(find /usr/lib/plymouth /usr/lib/ucode -name '*.so')"
	RAMFS_COPY_DATA="$RAMFS_COPY_DATA \
		$(find /usr/share/plymouth /etc/plymouth ! -type d)"

	/usr/sbin/plymouth-evict

	/usr/bin/plymouth --ping 2>/dev/null || return 0
	/usr/bin/plymouth change-mode --system-upgrade
	/usr/bin/plymouth reactivate
	/usr/bin/plymouth show-splash
	;;
*/do_stage2)
	# stage2 stopped the daemon along with everything else on the old root
	/usr/sbin/plymouthd --mode=shutdown --graphical-boot || return 0
	/usr/bin/plymouth change-mode --system-upgrade
	/usr/bin/plymouth show-splash
	;;
esac
