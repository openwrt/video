#
# Copyright (C) 2020 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5virtualkeyboard
PKG_RELEASE:=1
PKG_HASH:=100e676ca5eda629ae8bcf444d9117d388c5f8ca472c0b7aede2afa24eb1971f

PKG_BUILD_DEPENDS:=qt5tools/host

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include ../qmake.mk

define Package/qt5virtualkeyboard
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=qt5virtualkeyboard
  URL:=http://qt.io
  DEPENDS:=+qt5quick-quick +qt5quick-qml-models +qt5svg
endef

MAKE_PATH=src

define Build/InstallDev
	$(call Build/Install/HostFiles,$(1))
	$(call Build/Install/Headers,$(1))
	$(call Build/Install/Libs,$(1),*)
endef

define Package/qt5virtualkeyboard/install
	$(call Build/Install/Libs,$(1),libQt5VirtualKeyboard)

	$(call Build/Install/Plugins,$(1),platforminputcontexts,libqtvirtualkeyboardplugin)
	$(call Build/Install/Plugins,$(1),virtualkeyboard,libqtvirtualkeyboard_openwnn)
	$(call Build/Install/Plugins,$(1),virtualkeyboard,libqtvirtualkeyboard_tcime)
	$(call Build/Install/Plugins,$(1),virtualkeyboard,libqtvirtualkeyboard_pinyin)
	$(call Build/Install/Plugins,$(1),virtualkeyboard,libqtvirtualkeyboard_hangul)
	$(call Build/Install/Plugins,$(1),virtualkeyboard,libqtvirtualkeyboard_thai)

	$(INSTALL_DIR) \
	  $(1)/usr/lib/qt5/qml/QtQuick/VirtualKeyboard

	$(CP) \
	  $(PKG_INSTALL_DIR)/usr/lib/qt5/qml/QtQuick/VirtualKeyboard/* \
	  $(1)/usr/lib/qt5/qml/QtQuick/VirtualKeyboard/

endef

$(eval $(call BuildPackage,qt5virtualkeyboard))
