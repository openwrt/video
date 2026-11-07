#
# Copyright (C) 2026 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/cmake.mk

# the bin-path should be determined correctly via cmake or qmake queries,
# but in case scripts call it relying on it in $PATH, add bin/qt6/ to it.
export PATH:=$(STAGING_DIR_HOSTPKG)/bin/qt6:$(PATH)

CMAKE_OPTIONS += \
  -DQT_HOST_PATH="$(STAGING_DIR_HOSTPKG)" \
  -DQT_QMAKE_TARGET_MKSPEC="linux-openwrt-g++"

# The rpath is needed for the host libraries and executables to find shared
# objects from STAGING_DIR_HOSTPKG (e.g. libdouble-conversion) when linking and
# running. It is passed at the end of the link command, so it comes after the
# build tree's lib dir; otherwise the executables run during the build would
# load a previously installed Qt from STAGING_DIR_HOSTPKG.
# qmake based host builds get it from Qt's rpath feature instead.
CMAKE_HOST_OPTIONS += \
  -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_SKIP_RPATH=FALSE \
  -DCMAKE_C_STANDARD_LIBRARIES="-Wl,-rpath,$(STAGING_DIR_HOSTPKG)/lib" \
  -DCMAKE_CXX_STANDARD_LIBRARIES="-Wl,-rpath,$(STAGING_DIR_HOSTPKG)/lib" \
  -DQT_QMAKE_TARGET_MKSPEC="linux-openwrt-host-g++"
