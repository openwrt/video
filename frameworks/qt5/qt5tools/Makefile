#
# Copyright (C) 2020 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5tools
PKG_RELEASE:=1
PKG_HASH:=1cab11887faca54af59f4995ee435c9ad98d194e9e6889c846692c8b6815fc1c

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/nls.mk
include ../qmake.mk

HOST_BUILD_DEPENDS:=qt5base/host

define Package/qt5tools
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=qt5tools
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
  DEPENDS:=+qt5base-core
  BUILDONLY:=1
endef

define Host/Install
	$(call Host/Install/Default)

	$(INSTALL_DIR) \
	  $(STAGING_DIR_HOST)/lib \
	  $(STAGING_DIR_HOST)/bin/qt5

	$(CP) \
	  $(HOST_INSTALL_DIR)/lib/* \
	  $(STAGING_DIR_HOST)/lib/

	$(CP) \
	  $(HOST_INSTALL_DIR)/bin/qt5/* \
	  $(STAGING_DIR_HOST)/bin/qt5/

	ln -sf $(STAGING_DIR_HOST)/bin/qt5/lrelease \
	  $(STAGING_DIR)/host/bin/qt5/
	ln -sf $(STAGING_DIR_HOST)/bin/qt5/lupdate \
	  $(STAGING_DIR)/host/bin/qt5/

endef

$(eval $(call BuildPackage,qt5tools))
$(eval $(call HostBuild))
