#
# Copyright (C) 2015 OpenWrt
# Author: Mirko Vogt <mirko@openwrt.org>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5quick2-controls2
PKG_VERSION:=5.9.1
PKG_RELEASE:=1
PKG_HASH:=203be0321112d1513b722fce463f9e30d7d00befb33349901d9517f4f13408ff

PKG_SYS_NAME:=qtquickcontrols2-opensource-src-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_SYS_NAME).tar.xz
PKG_SOURCE_URL:=http://download.qt-project.org/official_releases/qt/$(basename $(PKG_VERSION))/$(PKG_VERSION)/submodules

PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SYS_NAME)
PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
-include $(STAGING_DIR)/host/mk/qmake.mk

define Package/qt5quick2-controls2
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=qt5quick2-controls2
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko@openwrt.org>
  DEPENDS:=+qt5quick2-quick
endef

define Build/InstallDev
	$(call Build/Install/HostFiles,$(1))
	$(call Build/Install/Headers,$(1))
	$(call Build/Install/Libs,$(1),*)
endef

define Package/qt5quick2-controls2/install
	$(call Build/Install/Libs,$(1),libQt5QuickTemplates2)
	$(call Build/Install/Libs,$(1),libQt5QuickControls2)

	$(INSTALL_DIR) \
	  $(1)/usr/lib/qt5/qml/QtQuick

	$(CP) \
	  $(PKG_INSTALL_DIR)/usr/lib/qt5/qml/QtQuick/* \
	  $(1)/usr/lib/qt5/qml/QtQuick/
endef

define Package/qt5quick2-templates2/install
	$(call Build/Install/Libs,$(1),libQt5QuickTemplates2)
endef

$(eval $(call BuildPackage,qt5quick2-controls2))
