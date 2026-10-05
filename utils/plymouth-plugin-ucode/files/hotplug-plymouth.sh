[ "$ACTION" = add ] || exit 0

case "$DEVNAME" in
input/event*) ;;
*) exit 0 ;;
esac

/usr/bin/plymouth update --status="input:${DEVNAME#input/}" 2>/dev/null
