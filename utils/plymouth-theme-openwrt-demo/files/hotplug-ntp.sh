[ "$ACTION" = "stratum" ] || exit 0
[ -x /usr/bin/plymouth ] || exit 0

/usr/bin/plymouth update --status="service:sysntpd" 2>/dev/null
