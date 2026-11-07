#
# Copyright (C) 2026 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt6translations
PKG_RELEASE:=1
PKG_HASH:=f36d545e6681b146fd79b3ebb74ef275e88694cf81eae8323327cae3bfc490a1

PKG_MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
PKG_LICENSE:=LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
# the tarball only ships these two of the license texts
PKG_LICENSE_FILES:= \
  LICENSES/LicenseRef-Qt-Commercial.txt \
  LICENSES/GPL-3.0-only.txt

PKG_BUILD_DEPENDS:=qt6tools/host qt6tools

QT_MODULE:=qttranslations

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include ../cmake.mk

define Package/qt6translations
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Qt6Translations
  URL:=https://www.qt.io
endef

define Package/qt6translations/install
	$(call Build/Install/Translations,$(1),*)
endef

$(eval $(call BuildPackage,qt6translations))
