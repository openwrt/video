# Video packages feed

## Description

This is an OpenWrt package feed containing video / graphics (as in 'higher than just curses') related libraries and applications which are not considered to be so called "core" packages.

## Usage

To use these packages, add the following line to the feeds.conf
in the OpenWrt buildroot:

```
src-git video https://github.com/openwrt/video.git
```

This feed should be included and enabled by default in the OpenWrt buildroot. To install all its package definitions, run:

```
./scripts/feeds update video
./scripts/feeds install -a -p video
```

The video packages should now appear in menuconfig in section 'Video'.
