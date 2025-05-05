#
# Copyright (C) 2020 OpenWrt.org
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

# This file contains Qt specific paths and Install definitions,
# which are agnostic to whether projects use cmake or qmake.
# Its main purpose is to provide helper functions for packaging
# software using Qt.

include $(TOPDIR)/rules.mk

QT_INSTALL_PREFIX:=$(CONFIGURE_PREFIX)
QT_INSTALL_CONFIGURATION:=/etc/qt6
QT_INSTALL_SYSCONF:=/etc/qt6
QT_INSTALL_LIBS:=$(QT_INSTALL_PREFIX)/lib
QT_INSTALL_DATA:=$(QT_INSTALL_PREFIX)/share/qt6
QT_INSTALL_HEADERS:=$(QT_INSTALL_PREFIX)/include
QT_INSTALL_CMAKES:=$(QT_INSTALL_LIBS)/cmake
QT_INSTALL_PKGCONFIGS:=$(QT_INSTALL_LIBS)/pkgconfig
QT_INSTALL_BINS:=$(QT_INSTALL_PREFIX)/bin/qt6
QT_INSTALL_DOCS:=$(QT_INSTALL_DATA)/doc
QT_INSTALL_TRANSLATIONS:=$(QT_INSTALL_DATA)/translations
QT_INSTALL_ARCHDATA:=$(QT_INSTALL_LIBS)/qt6
QT_INSTALL_MKSPECS:=$(QT_INSTALL_DATA)/mkspecs
QT_INSTALL_LIBEXECS:=$(QT_INSTALL_LIBS)exec/qt6
QT_INSTALL_TESTS:=$(QT_INSTALL_ARCHDATA)/tests
QT_INSTALL_PLUGINS:=$(QT_INSTALL_ARCHDATA)/plugins
QT_INSTALL_METATYPES:=$(QT_INSTALL_ARCHDATA)/metatypes
QT_INSTALL_IMPORTS:=$(QT_INSTALL_ARCHDATA)/imports
QT_INSTALL_QML:=$(QT_INSTALL_ARCHDATA)/qml
QT_INSTALL_EXAMPLES:=$(QT_INSTALL_ARCHDATA)/examples
QT_INSTALL_DEMOS:=$(QT_INSTALL_EXAMPLES)

QT_HOST_PREFIX:=$(STAGING_DIR_HOSTPKG)
QT_HOST_CONFIGURATION:=$(STAGING_DIR_HOSTPKG)/etc/qt6
QT_HOST_SYSCONF:=$(STAGING_DIR_HOSTPKG)/etc/qt6
QT_HOST_LIBS:=$(QT_HOST_PREFIX)/lib
QT_HOST_DATA:=$(QT_HOST_PREFIX)/share/qt6
QT_HOST_HEADERS:=$(QT_HOST_PREFIX)/include
QT_HOST_CMAKES:=$(QT_HOST_LIBS)/cmake
QT_HOST_PKGCONFIGS:=$(QT_HOST_LIBS)/pkgconfig
QT_HOST_BINS:=$(QT_HOST_PREFIX)/bin/qt6
QT_HOST_DOCS:=$(QT_HOST_DATA)/doc
QT_HOST_TRANSLATIONS:=$(QT_HOST_DATA)/translations
QT_HOST_ARCHDATA:=$(QT_HOST_LIBS)/qt6
QT_HOST_MKSPECS:=$(QT_HOST_DATA)/mkspecs
QT_HOST_LIBEXECS:=$(QT_HOST_LIBS)exec/qt6
QT_HOST_TESTS:=$(QT_HOST_ARCHDATA)/tests
QT_HOST_PLUGINS:=$(QT_HOST_ARCHDATA)/plugins
QT_HOST_METATYPES:=$(QT_HOST_ARCHDATA)/metatypes
QT_HOST_IMPORTS:=$(QT_HOST_ARCHDATA)/imports
QT_HOST_QML:=$(QT_HOST_ARCHDATA)/qml
QT_HOST_EXAMPLES:=$(QT_HOST_ARCHDATA)/examples
QT_HOST_DEMOS:=$(QT_HOST_EXAMPLES)

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

	#$(INSTALL_DIR) \
	#	$(1)/$(QT_INSTALL_PLUGINS)/$(2)

	#$(CP) \
	#	$(PKG_INSTALL_DIR)/$(QT_INSTALL_PLUGINS)/$(2)/$(3).so* \
	#	$(1)/$(QT_INSTALL_PLUGINS)/$(2)/
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

define Build/Install/QMLplugin
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_QML)/$(2)

	$(CP) \
	  $(PKG_INSTALL_DIR)/$(QT_INSTALL_QML)/$(2)/{plugins.qmltypes,qmldir,$(3).{so,qml,js}} \
	  $(1)/$(QT_INSTALL_QML)/$(2)/ \
	  || true
endef

###

define Build/InstallDev/Qt
	$(call Build/Install/Headers,$(1)) || true
	$(call Build/Install/Cmakes,$(1)) || true
	$(call Build/Install/Pkgconfigs,$(1)) || true
	$(call Build/Install/Libs_shared,$(1),*) || true
	$(call Build/Install/Libs_static,$(1),*) || true
	$(call Build/Install/Archdata,$(1)) || true
	$(call Build/Install/Data,$(1)) || true
endef
