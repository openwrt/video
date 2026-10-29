#
# Copyright (C) 2026 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt6svg
PKG_RELEASE:=1
PKG_HASH:=ec359d930c95935ea48af58b100c2f5d0d275968ec8ca1e0e76629b7159215fc

PKG_MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
PKG_LICENSE:=LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
# Qt's own licenses, followed by the license of the xsvg-derived code in QtSvg
PKG_LICENSE_FILES:= \
  LICENSES/LicenseRef-Qt-Commercial.txt \
  LICENSES/LGPL-3.0-only.txt \
  LICENSES/GPL-2.0-only.txt \
  LICENSES/GPL-3.0-only.txt \
  LICENSES/HPND-sell-variant.txt

PKG_BUILD_DEPENDS:=qt6base qt6shadertools/host
HOST_BUILD_DEPENDS:=qt6base/host

QT_MODULE:=qtsvg

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/nls.mk
include $(INCLUDE_DIR)/cmake.mk
include ../cmake.mk

define Package/qt6svg
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Qt6SVG
  DEPENDS+=+qt6base-widgets
  URL:=https://www.qt.io
endef

Build/InstallDev = $(Build/InstallDev/Qt)

define Package/qt6svg/install
	$(call Build/Install/Libs,$(1),libQt6Svg)
	$(call Build/Install/Libs,$(1),libQt6SvgWidgets)
	$(call Build/Install/Plugins,$(1),imageformats,libqsvg)
endef

$(eval $(call BuildPackage,qt6svg))
$(eval $(call HostBuild))
