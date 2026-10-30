#
# Copyright (C) 2026 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

# This file contains Qt specific paths and Install definitions,
# which are agnostic to whether projects use cmake or qmake.
# Its main purpose is to provide helper functions for packaging
# software using Qt.

include $(TOPDIR)/rules.mk

QT_INSTALL_PREFIX:=/usr
QT_INSTALL_CONFIGURATION:=/etc/qt6
QT_INSTALL_SYSCONF:=/etc/qt6
QT_INSTALL_LIBS:=$(QT_INSTALL_PREFIX)/lib
QT_INSTALL_DATA:=$(QT_INSTALL_PREFIX)/share/qt6
QT_INSTALL_HEADERS:=$(QT_INSTALL_PREFIX)/include/qt6
QT_INSTALL_CMAKES:=$(QT_INSTALL_LIBS)/cmake
QT_INSTALL_PKGCONFIGS:=$(QT_INSTALL_LIBS)/pkgconfig
QT_INSTALL_BINS:=$(QT_INSTALL_PREFIX)/bin/qt6
QT_INSTALL_DOCS:=$(QT_INSTALL_DATA)/doc
QT_INSTALL_TRANSLATIONS:=$(QT_INSTALL_DATA)/translations
QT_INSTALL_ARCHDATA:=$(QT_INSTALL_LIBS)/qt6
QT_INSTALL_MKSPECS:=$(QT_INSTALL_ARCHDATA)/mkspecs
QT_INSTALL_LIBEXECS:=$(QT_INSTALL_ARCHDATA)/libexec
QT_INSTALL_TESTS:=$(QT_INSTALL_ARCHDATA)/tests
QT_INSTALL_PLUGINS:=$(QT_INSTALL_ARCHDATA)/plugins
QT_INSTALL_METATYPES:=$(QT_INSTALL_ARCHDATA)/metatypes
QT_INSTALL_IMPORTS:=$(QT_INSTALL_ARCHDATA)/imports
QT_INSTALL_QML:=$(QT_INSTALL_ARCHDATA)/qml
QT_INSTALL_EXAMPLES:=$(QT_INSTALL_ARCHDATA)/examples
QT_INSTALL_DEMOS:=$(QT_INSTALL_EXAMPLES)

QT_HOSTPKG_PREFIX:=$(STAGING_DIR_HOSTPKG)
QT_HOSTPKG_CONFIGURATION:=$(STAGING_DIR_HOSTPKG)/etc/qt6
QT_HOSTPKG_SYSCONF:=$(STAGING_DIR_HOSTPKG)/etc/qt6
QT_HOSTPKG_LIBS:=$(QT_HOSTPKG_PREFIX)/lib
QT_HOSTPKG_DATA:=$(QT_HOSTPKG_PREFIX)/share/qt6
QT_HOSTPKG_HEADERS:=$(QT_HOSTPKG_PREFIX)/include/qt6
QT_HOSTPKG_CMAKES:=$(QT_HOSTPKG_LIBS)/cmake
QT_HOSTPKG_PKGCONFIGS:=$(QT_HOSTPKG_LIBS)/pkgconfig
QT_HOSTPKG_BINS:=$(QT_HOSTPKG_PREFIX)/bin/qt6
QT_HOSTPKG_DOCS:=$(QT_HOSTPKG_DATA)/doc
QT_HOSTPKG_TRANSLATIONS:=$(QT_HOSTPKG_DATA)/translations
QT_HOSTPKG_ARCHDATA:=$(QT_HOSTPKG_LIBS)/qt6
QT_HOSTPKG_MKSPECS:=$(QT_HOSTPKG_ARCHDATA)/mkspecs
QT_HOSTPKG_LIBEXECS:=$(QT_HOSTPKG_ARCHDATA)/libexec
QT_HOSTPKG_TESTS:=$(QT_HOSTPKG_ARCHDATA)/tests
QT_HOSTPKG_PLUGINS:=$(QT_HOSTPKG_ARCHDATA)/plugins
QT_HOSTPKG_METATYPES:=$(QT_HOSTPKG_ARCHDATA)/metatypes
QT_HOSTPKG_IMPORTS:=$(QT_HOSTPKG_ARCHDATA)/imports
QT_HOSTPKG_QML:=$(QT_HOSTPKG_ARCHDATA)/qml
QT_HOSTPKG_EXAMPLES:=$(QT_HOSTPKG_ARCHDATA)/examples
QT_HOSTPKG_DEMOS:=$(QT_HOSTPKG_EXAMPLES)

# for host builds defined in qmake based target project files (STAGING_DIR)/host
# defining here instead of qmake.mk to be available to packages without including qmake.mk
#QT_HOST_PREFIX:=$(STAGING_DIR)/host
QT_HOST_PREFIX:=$(QT_HOSTPKG_PREFIX)
#QT_HOST_DATA:=$(QT_HOST_PREFIX)/share/qt6
QT_HOST_DATA:=$(QT_HOSTPKG_DATA)
#QT_HOST_BINS:=$(QT_HOST_PREFIX)/bin/qt6
QT_HOST_BINS:=$(QT_HOSTPKG_BINS)
#QT_HOST_LIBS:=$(QT_HOST_PREFIX)/lib
QT_HOST_LIBS:=$(QT_HOSTPKG_LIBS)
#QT_HOST_LIBEXECS:=$(QT_HOST_LIBS)/qt6/libexec
QT_HOST_LIBEXECS:=$(QT_HOSTPKG_LIBEXECS)

# PKG defaults for official Qt modules, which set QT_MODULE to the name of
# the Qt module (e.g. qtbase) before including this file
ifneq ($(QT_MODULE),)

PKG_VERSION?=6.9.0

PKG_SOURCE_URL?=https://download.qt.io/archive/qt/$(basename $(PKG_VERSION))/$(PKG_VERSION)/submodules
PKG_SYS_NAME?=$(QT_MODULE)
PKG_SYS_NAME_FULL?=$(PKG_SYS_NAME)-everywhere-src-$(PKG_VERSION)
PKG_SOURCE?=$(PKG_SYS_NAME_FULL).tar.xz

PKG_BUILD_DIR?=$(BUILD_DIR)/$(PKG_SYS_NAME_FULL)
HOST_BUILD_DIR?=$(BUILD_DIR_HOST)/$(PKG_SYS_NAME_FULL)

PKG_BUILD_PARALLEL?=1
HOST_BUILD_PARALLEL?=1
PKG_BUILD_FLAGS?=no-mips16

PKG_INSTALL?=1
endif

# Do not use sstrip for Qt6. When sstrip is used, Qt6 plugin loading
# does not work, because of Qt's internal elf parser, see:
# https://bugreports.qt.io/browse/QTBUG-52567
# Use code copied from the gcc package to use stripping instead.
ifneq ($(CONFIG_USE_SSTRIP),)
  STRIP:=$(TARGET_CROSS)strip $(call qstrip,$(CONFIG_STRIP_ARGS))
  RSTRIP:= \
    export CROSS="$(TARGET_CROSS)" \
		$(if $(CONFIG_KERNEL_KALLSYMS),NO_RENAME=1) \
		$(if $(CONFIG_KERNEL_PROFILING),KEEP_SYMBOLS=1); \
    NM="$(TARGET_CROSS)nm" \
    STRIP="$(STRIP)" \
    STRIP_KMOD="$(SCRIPT_DIR)/strip-kmod.sh" \
    $(SCRIPT_DIR)/rstrip.sh
endif

define Build/Install/Headers
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_HEADERS)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_HEADERS)/* \
		$(1)/$(QT_INSTALL_HEADERS)/
endef

define Build/Install/Cmakes
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_CMAKES)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_CMAKES)/* \
		$(1)/$(QT_INSTALL_CMAKES)/
endef

define Build/Install/Pkgconfigs
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_PKGCONFIGS)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_PKGCONFIGS)/* \
		$(1)/$(QT_INSTALL_PKGCONFIGS)/
endef

define Build/Install/Libs_shared
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_LIBS)/$(dir $(2))

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_LIBS)/$(2).so* \
		$(1)/$(QT_INSTALL_LIBS)/$(dir $(2))/
endef

define Build/Install/Libs_static
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_LIBS)/$(dir $(2))

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_LIBS)/$(2).a \
		$(1)/$(QT_INSTALL_LIBS)/$(dir $(2))/
endef

define Build/Install/Libs
	$(call Build/Install/Libs_shared,$(1),$(2))
endef

define Build/Install/Archdata
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_ARCHDATA)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_ARCHDATA)/* \
		$(1)/$(QT_INSTALL_ARCHDATA)/
endef

define Build/Install/Data
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_DATA)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_DATA)/* \
		$(1)/$(QT_INSTALL_DATA)/
endef

define Build/Install/Mkspecs
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_MKSPECS)/$(dir $(2))

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_MKSPECS)/$(2) \
		$(1)/$(QT_INSTALL_MKSPECS)/$(dir $(2))/
endef

define Build/Install/Metatypes
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_METATYPES)/$(dir $(2))

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_METATYPES)/$(2).json \
		$(1)/$(QT_INSTALL_METATYPES)/$(dir $(2))/
endef

define Build/Install/Translations
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_TRANSLATIONS)/$(dir $(2))

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_TRANSLATIONS)/$(2).qm \
		$(1)/$(QT_INSTALL_TRANSLATIONS)/$(dir $(2))/
endef

define Build/Install/Plugins
	if [ "$(2)" = '*' ]; then \
		$(INSTALL_DIR) \
			$(1)/$(QT_INSTALL_PLUGINS) ; \
		$(CP) \
			$(PKG_INSTALL_DIR)/$(QT_INSTALL_PLUGINS)/$(2) \
			$(1)/$(QT_INSTALL_PLUGINS)/ ; \
	else \
		$(INSTALL_DIR) \
			$(1)/$(QT_INSTALL_PLUGINS)/$(2) ; \
		$(CP) \
			$(PKG_INSTALL_DIR)/$(QT_INSTALL_PLUGINS)/$(2)/$(3).so* \
			$(1)/$(QT_INSTALL_PLUGINS)/$(2)/ ; \
	fi
endef

define Build/Install/Examples
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_EXAMPLES)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_EXAMPLES)/* \
		$(1)/$(QT_INSTALL_EXAMPLES)/

	$(FIND) $(1)/$(QT_INSTALL_EXAMPLES) \
		-type f \( -name '*.cpp' -o -name '*.h' -o -name '*.pro' -o -name '*.pri' \) | \
		$(XARGS) $(RM) -vf
endef

# Not every QML module directory contains all kinds of files, e.g. some only
# contain .qml files. The directory has to exist and contain at least one of
# them, though.
define Build/Install/QMLplugin
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_QML)/$(2)

	files="$$$$($(FIND) $(PKG_INSTALL_DIR)/$(QT_INSTALL_QML)/$(2) \
	  -mindepth 1 -maxdepth 1 \
	  \( -name plugins.qmltypes -o -name qmldir \
	  -o -name '$(3).so' -o -name '$(3).qml' -o -name '$(3).js' \))" && \
	[ -n "$$$$files" ] && \
	$(CP) \
	  $$$$files \
	  $(1)/$(QT_INSTALL_QML)/$(2)/
endef

###

# Copies $(2), a path or glob below the install root, to $(1) if the module
# installs anything there. Failing to copy fails the build.
define Build/InstallDev/IfPresent
	if ls -d $(PKG_INSTALL_DIR)$(2) >/dev/null 2>&1; then \
		$(INSTALL_DIR) $(1)$(dir $(2)) && \
		$(CP) $(PKG_INSTALL_DIR)$(2) $(1)$(dir $(2)); \
	fi
endef

# Not every module installs headers or data, only what it installs gets
# copied. The library dir is copied as a whole: besides the libraries, the
# cmake and pkg-config files and the archdata (mkspecs, plugins, QML modules)
# it contains the .prl files read by qmake and the objects-* dirs referenced
# by the cmake files of static libraries.
define Build/InstallDev/Qt
	$(call Build/InstallDev/IfPresent,$(1),$(QT_INSTALL_HEADERS))
	$(call Build/InstallDev/IfPresent,$(1),$(QT_INSTALL_LIBS))
	$(call Build/InstallDev/IfPresent,$(1),$(QT_INSTALL_DATA))
endef
