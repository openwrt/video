#
# Copyright (C) 2020 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# Author: Tito Brasolin <tito.brasolin@gmail.com>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5serialport
PKG_RELEASE:=1
PKG_HASH:=e139718f7ace082a69fc65762178ce959d7827a8e87be9773555a6c649154ebc

PKG_BUILD_DEPENDS:=qt5base

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include ../qmake.mk

MAKE_PATH=src

define Package/qt5serialport
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=qt5serialport
  URL:=http://qt.io
  DEPENDS:=+qt5base-core
endef

define Build/InstallDev
	$(call Build/Install/HostFiles,$(1))
	$(call Build/Install/Headers,$(1))
	$(call Build/Install/Libs,$(1),*)
endef

define Package/qt5serialport/install
	$(call Build/Install/Libs,$(1),libQt5SerialPort)
endef

$(eval $(call BuildPackage,qt5serialport))
