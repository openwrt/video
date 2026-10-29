#
# Copyright (C) 2026 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt6svg
PKG_RELEASE:=1
PKG_HASH:=db76aa3358cbbe6fce7da576ff4669cb9801920188c750d3b12783bbe97026e2

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
include ../cmake.mk

PKG_CONFIG_DEPENDS:=CONFIG_PACKAGE_qt6svg-widgets

# libQt6SvgWidgets is built whenever Widgets is found
CMAKE_OPTIONS += \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Widgets=$(if $(CONFIG_PACKAGE_qt6svg-widgets),OFF,ON)

CMAKE_HOST_OPTIONS += \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Widgets=ON

define Package/qt6svg/Default
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Qt6SVG
  URL:=https://www.qt.io
endef

define Package/qt6svg
  $(call Package/qt6svg/Default)
  DEPENDS+=+qt6base-gui
endef

define Package/qt6svg-widgets
  $(call Package/qt6svg/Default)
  TITLE+=widgets
  DEPENDS+=+qt6svg +qt6base-widgets
endef

Build/InstallDev = $(Build/InstallDev/Qt)

define Package/qt6svg/install
	$(call Build/Install/Libs,$(1),libQt6Svg)
	$(call Build/Install/Plugins,$(1),imageformats,libqsvg)
	$(call Build/Install/Plugins,$(1),iconengines,libqsvgicon)
endef

define Package/qt6svg-widgets/install
	$(call Build/Install/Libs,$(1),libQt6SvgWidgets)
endef

$(eval $(call BuildPackage,qt6svg))
$(eval $(call BuildPackage,qt6svg-widgets))
$(eval $(call HostBuild))
