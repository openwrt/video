#
# Copyright (C) 2020 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5graphicaleffects
PKG_RELEASE:=1
PKG_HASH:=d185813737f38352d81b3a405a9243aca1af7bfebe37aba22a4099a7031f640a

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include ../qmake.mk

define Package/qt5graphicaleffects
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=qt5graphicaleffects
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
  DEPENDS:=+qt5quick-quick @BROKEN # depends on GL support, but currently we only support software rendering
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
