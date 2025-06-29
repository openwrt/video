#
# Copyright (C) 2020 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# Author: Tito Brasolin <tito.brasolin@gmail.com>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5serialbus
PKG_RELEASE:=1
PKG_HASH:=8d600747367532ed563e75b8ace8dc2e4e4fca9280a964281b2a1ad49e0eeab6

PKG_BUILD_DEPENDS:=qt5base

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include ../qmake.mk

MAKE_PATH=src

define Package/qt5serialbus
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=qt5serialbus
  URL:=http://qt.io
  DEPENDS:=+qt5base-network +qt5serialport
endef

define Build/InstallDev
	$(call Build/Install/HostFiles,$(1))
	$(call Build/Install/Headers,$(1))
	$(call Build/Install/Libs,$(1),*)
endef

define Package/qt5serialbus/install
	$(call Build/Install/Libs,$(1),libQt5SerialBus)
endef

$(eval $(call BuildPackage,qt5serialbus))
