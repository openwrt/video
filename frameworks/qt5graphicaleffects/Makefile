#
# Copyright (C) 2016 OpenWrt
# Author: Mirko Vogt <mirko@openwrt.org>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5graphicaleffects
PKG_VERSION:=5.9.1
PKG_RELEASE:=1
PKG_HASH:=5f86c38e36d9553049cfbe56d4869e73cf8ab920905402cbcafb56dd8a1a59ff

PKG_SYS_NAME:=qtgraphicaleffects-opensource-src-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_SYS_NAME).tar.xz
PKG_SOURCE_URL:=http://download.qt-project.org/official_releases/qt/$(basename $(PKG_VERSION))/$(PKG_VERSION)/submodules

PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SYS_NAME)
PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
-include $(STAGING_DIR)/host/mk/qmake.mk

define Package/qt5graphicaleffects
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=qt5graphicaleffects
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko@openwrt.org>
  DEPENDS:=+qt5quick2-quick
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
