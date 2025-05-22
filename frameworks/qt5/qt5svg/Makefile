#
# Copyright (C) 2020 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5svg
PKG_RELEASE:=1
PKG_HASH:=cc4e320084bc2882dc6e55229045ce99469946deab8c075aae06413e27100bc1

PKG_BUILD_DEPENDS:=qt5base

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include ../qmake.mk

MAKE_PATH=src

define Package/qt5svg
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=qt5svg
  URL:=http://qt.io
  DEPENDS:=+qt5base-widgets
endef

define Build/InstallDev
	$(call Build/Install/HostFiles,$(1))
	$(call Build/Install/Headers,$(1))
	$(call Build/Install/Libs,$(1),*)
endef

define Package/qt5svg/install
	$(call Build/Install/Libs,$(1),libQt5Svg)
	$(call Build/Install/Plugins,$(1),imageformats,libqsvg)
	$(call Build/Install/Plugins,$(1),iconengines,libqsvgicon)
endef

$(eval $(call BuildPackage,qt5svg))
