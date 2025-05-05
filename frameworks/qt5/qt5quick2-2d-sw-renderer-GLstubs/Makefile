#
# Copyright (C) 2020 OpenWrt.org
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
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

PKG_NAME:=qt5quick2-2d-sw-renderer-GLstubs
PKG_VERSION:=5.7.0
PKG_RELEASE:=1
PKG_MD5SUM:=41bfcb5ab6d7a820687c4208beeb7057

PKG_SYS_NAME:=qtdeclarative-render2d-opensource-src-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_SYS_NAME).tar.xz
PKG_SOURCE_URL:=https://download.qt.io/archive/qt/$(basename $(PKG_VERSION))/$(PKG_VERSION)/submodules

PKG_BUILD_DIR=$(BUILD_DIR)/qtdeclarative-render2d-opensource-src-$(PKG_VERSION)-GLstubs/$(PKG_SYS_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/qt5quick2-2d-sw-renderer-GLstubs
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Dummy GL stubs
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
endef

TARGET_CFLAGS += \
  -I../3rdparty/include

define Build/Configure
endef

define Build/Compile
	( cd $(PKG_BUILD_DIR)/tools/opengldummy/src ; \
	  $(TARGET_CC) $(TARGET_CFLAGS) -fPIC -shared -o libEGL.so egl.cpp && \
	  $(TARGET_CC) $(TARGET_CFLAGS) -fPIC -shared -o libGLESv2.so gles2.cpp \
	)
endef

define Build/InstallDev
	$(INSTALL_DIR) \
	  $(1)/usr/include \
	  $(1)/usr/lib

	$(CP) \
	  $(PKG_BUILD_DIR)/tools/opengldummy/3rdparty/include/* \
	  $(1)/usr/include/

	$(INSTALL_DATA) \
	  $(PKG_BUILD_DIR)/tools/opengldummy/src/lib*.so* \
	  $(1)/usr/lib/
endef


define Package/qt5quick2-2d-sw-renderer-GLstubs/install
	$(INSTALL_DIR) \
	  $(1)/usr/lib

	$(INSTALL_DATA) \
	  $(PKG_BUILD_DIR)/tools/opengldummy/src/lib*.so* \
	  $(1)/usr/lib/
endef

$(eval $(call BuildPackage,qt5quick2-2d-sw-renderer-GLstubs))
