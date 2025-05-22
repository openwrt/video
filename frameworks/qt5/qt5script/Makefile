#
# Copyright (C) 2020 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5script
PKG_RELEASE:=1
PKG_HASH:=0b98baa6eece7d0e7860590dd67faf452499096f62f33d16556fd38ac236eec2

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include ../qmake.mk

define Package/qt5script/Default
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Qt5script
#  DEPENDS:=qt5script +qt5base-core
  DEPENDS:=+qt5base-core
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
endef

# define Package/qt5script
#   $(call Package/qt5script/Default)
#   DEPENDS:=+qt5base
#   MENU:=1
# endef

define Package/qt5script-script
  $(call Package/qt5script/Default)
  TITLE+=script
endef

define Package/qt5script-scriptTools
  $(call Package/qt5script/Default)
  TITLE+=scriptTools
  DEPENDS+=+qt5base-gui +qt5base-widgets +qt5script-script
endef

define Package/qt5script-examples
  $(call Package/qt5script/Default)
  TITLE+=examples
  DEPENDS+=+qt5script-scriptTools @BROKEN
endef

define Build/InstallDev
	$(call Build/Install/HostFiles,$(1))
	$(call Build/Install/Headers,$(1))
	$(call Build/Install/Libs,$(1),*)
endef

define Package/qt5script-script/install
	$(call Build/Install/Libs,$(1),libQt5Script)
endef

define Package/qt5script-scriptTools/install
	$(call Build/Install/Libs,$(1),libQt5ScriptTools)
endef

define Package/qt5script-examples/install
	$(call Build/Install/Examples,$(1))
endef

#$(eval $(call BuildPackage,qt5script))
$(eval $(call BuildPackage,qt5script-script))
$(eval $(call BuildPackage,qt5script-scriptTools))
$(eval $(call BuildPackage,qt5script-examples))
