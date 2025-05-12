#
# Copyright (C) 2020 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5translations
PKG_VERSION:=5.15.16
PKG_RELEASE:=1
PKG_HASH:=415dbbb82a75dfc9a7be969e743bee54c0e6867be37bce4cf8f03da39f20112a

PKG_SYS_NAME:=qttranslations-everywhere-src-$(PKG_VERSION)
PKG_SOURCE:=$(subst -src-,-opensource-src-,$(PKG_SYS_NAME)).tar.xz
PKG_SOURCE_URL:=https://download.qt.io/official_releases/qt/$(basename $(PKG_VERSION))/$(PKG_VERSION)/submodules

PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SYS_NAME)
PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1
PKG_BUILD_DEPENDS:=qt5tools/host

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include ../common.mk
include ../qmake.mk

define Package/qt5translations
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=qt5translations
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
endef

define Package/qt5translations/install
	$(call Build/Install/Translations,$(1),*)
endef

$(eval $(call BuildPackage,qt5translations))
