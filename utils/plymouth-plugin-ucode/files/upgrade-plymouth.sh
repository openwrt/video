[ -x /usr/bin/plymouth ] || return 0

RAMFS_COPY_BIN="$RAMFS_COPY_BIN /usr/bin/plymouth /usr/sbin/plymouthd"
RAMFS_COPY_DATA="$RAMFS_COPY_DATA /usr/lib/plymouth /usr/share/plymouth \
	/etc/plymouth /usr/lib/ucode /usr/lib/libplutovg.so.* \
	/usr/lib/libplutosvg.so.* /usr/lib/libply*.so.*"

/usr/bin/plymouth --ping 2>/dev/null || return 0

/usr/bin/plymouth change-mode --system-upgrade

# common.sh sorts before this file, so this replaces its v() rather than
# being replaced by it
v() {
	_v "$(date) upgrade: $@"
	logger -p info -t upgrade "$@"
	/usr/bin/plymouth update --status="$*" 2>/dev/null
}
