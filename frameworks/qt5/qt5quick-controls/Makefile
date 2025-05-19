#
# Copyright (C) 2020 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5quick-controls2
PKG_RELEASE:=1
PKG_HASH:=6bdc134c7f86dcf0e416d89d96d0d1d1415ea465bceb1f360fb5badf2adcaa16

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include ../qmake.mk

define Package/qt5quick-controls2
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=qt5quick-controls2
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
  DEPENDS:=+qt5quick-quick
endef

define Build/InstallDev
	$(call Build/Install/HostFiles,$(1))
	$(call Build/Install/Headers,$(1))
	$(call Build/Install/Cmakes,$(1))
	$(call Build/Install/Libs,$(1),*)
endef

define Package/qt5quick-controls2/install
	$(call Build/Install/Libs,$(1),libQt5QuickTemplates2)
	$(call Build/Install/Libs,$(1),libQt5QuickControls2)

	$(INSTALL_DIR) \
	  $(1)/usr/lib/qt5/qml/QtQuick

	$(CP) \
	  $(PKG_INSTALL_DIR)/usr/lib/qt5/qml/QtQuick/* \
	  $(1)/usr/lib/qt5/qml/QtQuick/
endef

define Package/qt5quick-templates2/install
	$(call Build/Install/Libs,$(1),libQt5QuickTemplates2)
endef

$(eval $(call BuildPackage,qt5quick-controls2))
