#!/bin/sh
# Version check override for the generic package tests.
#
# Nothing these packages install reports the package version. The three
# programs in libwpewebkit are helper processes the library spawns
# (WPEGPUProcess, WPENetworkProcess, WPEWebProcess); they take no arguments
# and print nothing. WPEWebDriver has no --version option, only --help,
# --port, --host, --target and --replace-on-new-session. The one file that
# does carry the version, wpe-webkit-2.0.pc, belongs to Build/InstallDev and
# is not shipped. The library soname encodes the libtool triple, 1.11.3 for
# 2.54.0, not the package version.
#
# So this cannot verify the version, and does not pretend to. It checks that
# the package installed the artefacts it is supposed to, which is the useful
# part the probe was standing in for.

set -e

pkg="$1"
version="$2"

[ -n "$pkg" ] || exit 1
[ -n "$version" ] || exit 1

case "$pkg" in
libwpewebkit)
	# The versioned library, the SONAME symlink pointing at it, the
	# injected bundle, and the three helper processes.
	lib=$(ls /usr/lib/libWPEWebKit-2.0.so.*.*.* 2>/dev/null | head -n 1)
	[ -n "$lib" ] || exit 1
	[ -f "$lib" ] || exit 1
	[ -e /usr/lib/libWPEWebKit-2.0.so.1 ] || exit 1
	[ -f /usr/lib/wpe-webkit-2.0/injected-bundle/libWPEInjectedBundle.so ] || exit 1
	for helper in WPEGPUProcess WPENetworkProcess WPEWebProcess; do
		[ -x "/usr/libexec/wpe-webkit-2.0/$helper" ] || exit 1
	done
	[ -f /usr/share/wpe-webkit-2.0/inspector.gresource ] || exit 1
	;;
wpewebkit-driver)
	[ -x /usr/bin/WPEWebDriver ] || exit 1
	;;
wpewebkit-minibrowser)
	[ -x /usr/libexec/wpe-webkit-2.0/MiniBrowser ] || exit 1
	;;
*)
	# An unknown subpackage means this override is out of date.
	exit 1
	;;
esac

exit 0
