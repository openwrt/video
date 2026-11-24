#!/bin/sh

# shellcheck shell=busybox

# vulkaninfo prints version information only once a Vulkan driver is loaded
case "$PKG_NAME" in
vulkaninfo)
	exit 0
	;;

*)
	echo "Untested package: $PKG_NAME" >&2
	exit 1
	;;
esac
