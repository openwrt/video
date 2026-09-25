#!/bin/sh
# Neither binary reports a version: upstream's command parser has no
# --version and prints only a usage summary. Check that both still
# identify themselves and exit cleanly.
/usr/bin/plymouth --help 2>&1 | grep -q '^Splash control client$' || exit 1
/usr/sbin/plymouthd --help 2>&1 | grep -q '^Splash server$' || exit 1

exit 0
