#
# Copyright (C) 2020 OpenWrt.org
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

# for target builds (STAGING_DIR)
QT_INSTALL_PREFIX:=/usr
QT_INSTALL_CONFIGURATION:=/etc/qt5
QT_INSTALL_LIBS:=$(QT_INSTALL_PREFIX)/lib
QT_INSTALL_DATA:=$(QT_INSTALL_PREFIX)/share/qt5
QT_INSTALL_HEADERS:=$(QT_INSTALL_PREFIX)/include/qt5
QT_INSTALL_CMAKES:=$(QT_INSTALL_PREFIX)/lib/cmake
QT_INSTALL_PKGCONFIGS:=$(QT_INSTALL_PREFIX)/lib/pkgconfig
QT_INSTALL_BINS:=$(QT_INSTALL_PREFIX)/bin
QT_INSTALL_DOCS:=$(QT_INSTALL_DATA)/doc
QT_INSTALL_TRANSLATIONS:=$(QT_INSTALL_DATA)/translations
QT_INSTALL_ARCHDATA:=$(QT_INSTALL_LIBS)/qt5
QT_INSTALL_LIBEXECS:=$(QT_INSTALL_ARCHDATA)
QT_INSTALL_TESTS:=$(QT_INSTALL_ARCHDATA)/tests
QT_INSTALL_PLUGINS:=$(QT_INSTALL_ARCHDATA)/plugins
QT_INSTALL_IMPORTS:=$(QT_INSTALL_ARCHDATA)/imports
QT_INSTALL_QML:=$(QT_INSTALL_ARCHDATA)/qml
QT_INSTALL_EXAMPLES:=$(QT_INSTALL_ARCHDATA)/examples
QT_INSTALL_DEMOS:=$(QT_INSTALL_EXAMPLES)
# for host builds defined in target project files (STAGING_DIR)/host
QT_HOST_PREFIX:=$(STAGING_DIR)/host
QT_HOST_DATA:=$(QT_HOST_PREFIX)/share/qt5
QT_HOST_BINS:=$(QT_HOST_PREFIX)/bin/qt5
QT_HOST_LIBS:=$(QT_HOST_PREFIX)/lib
# for host builds defined in host project files (STAGING_DIR_HOST)
QT_HOSTPKG_PREFIX:=$(STAGING_DIR_HOST)
QT_HOSTPKG_CONFIGURATION:=$(STAGING_DIR_HOST)/etc/qt5
QT_HOSTPKG_LIBS:=$(QT_HOSTPKG_PREFIX)/lib
QT_HOSTPKG_DATA:=$(QT_HOSTPKG_PREFIX)/share/qt5
QT_HOSTPKG_HEADERS:=$(QT_HOSTPKG_PREFIX)/include/qt5
QT_HOSTPKG_CMAKES:=$(QT_HOSTPKG_PREFIX)/lib/cmake
QT_HOSTPKG_PKGCONFIGS:=$(QT_HOSTPKG_PREFIX)/lib/pkgconfig
QT_HOSTPKG_BINS:=$(QT_HOSTPKG_PREFIX)/bin/qt5
QT_HOSTPKG_DOCS:=$(QT_HOSTPKG_DATA)/doc
QT_HOSTPKG_TRANSLATIONS:=$(QT_HOSTPKG_DATA)/translations
QT_HOSTPKG_ARCHDATA:=$(QT_HOSTPKG_LIBS)/qt5
QT_HOSTPKG_LIBEXECS:=$(QT_HOSTPKG_ARCHDATA)/libexec
QT_HOSTPKG_TESTS:=$(QT_HOSTPKG_ARCHDATA)/tests
QT_HOSTPKG_PLUGINS:=$(QT_HOSTPKG_ARCHDATA)/plugins
QT_HOSTPKG_IMPORTS:=$(QT_HOSTPKG_ARCHDATA)/imports
QT_HOSTPKG_QML:=$(QT_HOSTPKG_ARCHDATA)/qml
QT_HOSTPKG_EXAMPLES:=$(QT_HOSTPKG_ARCHDATA)/examples
QT_HOSTPKG_DEMOS:=$(QT_HOSTPKG_EXAMPLES)

ifeq (qt, $(firstword $(subst 5, ,$(PKG_NAME))))
# PKG defaults for official Qt modules

PKG_VERSION?=5.15.16

PKG_SOURCE_URL?=https://download.qt.io/official_releases/qt/$(basename $(PKG_VERSION))/$(PKG_VERSION)/submodules
PKG_SYS_NAME?=$(subst -,,$(subst $(firstword $(subst ., ,$(PKG_VERSION))),,$(PKG_NAME)))
PKG_SYS_NAME_FULL?=$(PKG_SYS_NAME)-everywhere-src-$(PKG_VERSION)
PKG_SOURCE?=$(subst -src-,-opensource-src-,$(PKG_SYS_NAME_FULL)).tar.xz

PKG_LICENSE?=LGPL-3.0-or-commercial

PKG_BUILD_DIR?=$(BUILD_DIR)/$(PKG_SYS_NAME_FULL)
HOST_BUILD_DIR?=$(BUILD_DIR)/host/$(PKG_SYS_NAME_FULL)

PKG_BUILD_PARALLEL?=1
HOST_BUILD_PARALLEL?=1
PKG_BUILD_FLAGS?=no-mips16

PKG_INSTALL?=1
endif

define Build/Install/Headers
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_HEADERS)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_HEADERS)/* \
		$(1)/$(QT_INSTALL_HEADERS)/
endef

define Build/Install/Libs
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_LIBS)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_LIBS)/$(2).so* \
		$(1)/$(QT_INSTALL_LIBS)/
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

define Build/Install/Translations
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_TRANSLATIONS)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_TRANSLATIONS)/$(2).qm \
		$(1)/$(QT_INSTALL_TRANSLATIONS)/
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
