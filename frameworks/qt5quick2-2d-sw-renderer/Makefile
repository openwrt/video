#
# Copyright (C) 2016 OpenWrt.org
# Author: Mirko Vogt <mirko@openwrt.org>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

### This package shares the same code as `qt5quick2-GLstubs`.
### However unfortunately we can't handle both packages within
### one Makefile, as it would create a circular dependency:
###  `qt5base` depends on `qt5quick2-GLstubs`, but
### `qt5quick2-2d-sw-renderer` depends on `qt5quick2` which
### depends on `qt5base`.
### `qt5quick2-2d-sw-renderer` and `qt5quick2-GLstubs` sharing
### the same Makefile results in `qt5base` depending on `qt5quick2`
### which is meh.

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5quick2-2d-sw-renderer
PKG_VERSION:=5.7.0
PKG_RELEASE:=1
MD5SUM:=41bfcb5ab6d7a820687c4208beeb7057

PKG_SYS_NAME:=qtdeclarative-render2d-opensource-src-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_SYS_NAME).tar.xz
PKG_SOURCE_URL:=http://download.qt-project.org/official_releases/qt/$(basename $(PKG_VERSION))/$(PKG_VERSION)/submodules

PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SYS_NAME)
PKG_BUILD_PARALLEL:=1
PKG_BUILD_DEPENDS:=qt5base qt5quick2

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
-include $(STAGING_DIR)/host/mk/qmake.mk

define Package/qt5quick2-2d-sw-renderer
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Qt5Quick2 2d Software renderer
  DEPENDS:=+qt5quick2-quick
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko@openwrt.org>
endef

define Build/Configure
	$(call Build/Configure/Default,scenegraph-raster)
endef

define Build/InstallDev
	$(call Build/Install/Plugins,$(1),scenegraph,libsoftwarecontext)
endef

define Package/qt5quick2-2d-sw-renderer/install
	$(call Build/Install/Plugins,$(1),scenegraph,libsoftwarecontext)
endef

$(eval $(call BuildPackage,qt5quick2-2d-sw-renderer))
