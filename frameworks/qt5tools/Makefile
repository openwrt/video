#
# Copyright (C) 2016 OpenWrt
# Author: Mirko Vogt <mirko@openwrt.org>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5tools
PKG_VERSION:=5.9.1
PKG_RELEASE:=1
PKG_HASH:=c4eb56cf24a75661b8317b566be37396c90357b4f6730ef12b8c97a7079ca0e8,

PKG_SYS_NAME:=qttools-opensource-src-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_SYS_NAME).tar.xz
PKG_SOURCE_URL:=http://download.qt-project.org/official_releases/qt/$(basename $(PKG_VERSION))/$(PKG_VERSION)/submodules

PKG_HOST_ONLY:=1
PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SYS_NAME)
HOST_BUILD_DIR=$(BUILD_DIR)/host/$(PKG_SYS_NAME)
PKG_BUILD_PARALLEL:=1
HOST_BUILD_PARALLEL:=1
PKG_INSTALL:=1
HOST_BUILD_DEPENDS:=qt5base/host

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/nls.mk
-include $(STAGING_DIR)/host/mk/qmake.mk

define Package/qt5tools
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=qt5tools
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko@openwrt.org>
  DEPENDS:=+qt5base
  BUILDONLY:=1
endef

define Host/Install
	$(call Host/Install/Default)

	$(INSTALL_DIR) \
	  $(STAGING_DIR_HOST)/lib \
	  $(STAGING_DIR_HOST)/bin

	$(CP) \
	  $(HOST_INSTALL_DIR)/lib/* \
	  $(STAGING_DIR_HOST)/lib/

	$(CP) \
	  $(HOST_INSTALL_DIR)/bin/* \
	  $(STAGING_DIR_HOST)/bin/
endef

$(eval $(call BuildPackage,qt5tools))
$(eval $(call HostBuild))
