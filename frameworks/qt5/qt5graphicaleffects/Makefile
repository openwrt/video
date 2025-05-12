#
# Copyright (C) 2020 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5graphicaleffects
PKG_VERSION:=5.15.16
PKG_RELEASE:=1
PKG_HASH:=d185813737f38352d81b3a405a9243aca1af7bfebe37aba22a4099a7031f640a

PKG_SYS_NAME:=qtgraphicaleffects-everywhere-src-$(PKG_VERSION)
PKG_SOURCE:=$(subst -src-,-opensource-src-,$(PKG_SYS_NAME)).tar.xz
PKG_SOURCE_URL:=https://download.qt.io/official_releases/qt/$(basename $(PKG_VERSION))/$(PKG_VERSION)/submodules

PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SYS_NAME)
PKG_BUILD_PARALLEL:=1
PKG_BUILD_DEPENDS:=qt5base
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
-include $(STAGING_DIR)/host/mk/qmake5.mk

define Package/qt5graphicaleffects
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=qt5graphicaleffects
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
  DEPENDS:=+qt5quick2-quick @BROKEN # depends on GL support, but currently we only support software rendering
endef

define Build/InstallDev
endef

define Package/qt5graphicaleffects/install
	$(INSTALL_DIR) \
	  $(1)/usr/lib/qt5/qml/QtGraphicalEffects

	$(CP) \
	  $(PKG_INSTALL_DIR)/usr/lib/qt5/qml/QtGraphicalEffects/* \
	  $(1)/usr/lib/qt5/qml/QtGraphicalEffects/
endef

$(eval $(call BuildPackage,qt5graphicaleffects))
