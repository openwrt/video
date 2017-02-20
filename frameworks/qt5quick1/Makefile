#
# Copyright (C) 2015 OpenWrt
# Author: Mirko Vogt <mirko@openwrt.org>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5quick1
PKG_VERSION:=5.4.2
PKG_RELEASE:=1
PKG_MD5SUM:=8275c3ca1f962905baf1f19310e1b6e4

PKG_SYS_NAME:=qtquick1-opensource-src-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_SYS_NAME).tar.xz
PKG_SOURCE_URL:=http://download.qt-project.org/official_releases/qt/$(basename $(PKG_VERSION))/$(PKG_VERSION)/submodules

PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SYS_NAME)
PKG_BUILD_PARALLEL:=1
PKG_BUILD_DEPENDS:=qt5base
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
-include $(STAGING_DIR)/host/mk/qmake.mk

define Package/qt5quick1/Default
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Qt5quick1
  DEPENDS:=qt5quick1 +qt5base-core +qt5base-gui +qt5base-network +qt5base-sql +qt5base-widgets +qt5script-script
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko@openwrt.org>
endef

# define Package/qt5quick1
#   $(call Package/qt5quick1/Default)
#   DEPENDS:=+qt5base +qt5script
#   MENU:=1
# endef

define Package/qt5quick1-declarative
  $(call Package/qt5quick1/Default)
  TITLE+=declarative
endef

#TODO: fine grane
define Package/qt5quick1-imports
  $(call Package/qt5quick1/Default)
  TITLE+=imports
  DEPENDS+=+qt5quick1-declarative
endef

define Package/qt5quick1-examples
  $(call Package/qt5quick1/Default)
  TITLE+=examples
  DEPENDS+=+qt5quick1-declarative
endef

define Build/InstallDev
	$(call Build/Install/HostFiles,$(1))
	$(call Build/Install/Headers,$(1))
	$(call Build/Install/Libs,$(1),*)
endef

define Package/qt5quick1-declarative/install
	$(call Build/Install/Libs,$(1),*)
endef

define Package/qt5quick1-imports/install
	$(INSTALL_DIR) \
		$(1)/usr/lib/qt5

	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/qt5/* \
		$(1)/usr/lib/qt5/
endef

define Package/qt5quick1-examples/install
	$(call Build/Install/Examples,$(1))
endef

# $(eval $(call BuildPackage,qt5quick1))
$(eval $(call BuildPackage,qt5quick1-declarative))
$(eval $(call BuildPackage,qt5quick1-imports))
$(eval $(call BuildPackage,qt5quick1-examples))
