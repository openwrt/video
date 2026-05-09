include $(TOPDIR)/rules.mk

PKG_NAME:=harfbuzz
PKG_VERSION:=14.2.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://github.com/harfbuzz/harfbuzz/releases/download/$(PKG_VERSION)/
PKG_HASH:=94017020f96d025bb66ae91574e4cf334bcad23e8175a8a40565b3721bc2eaff

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=LGPL-2.1
PKG_LICENSE_FILES:=COPYING

PKG_INSTALL:=1

PKG_BUILD_DEPENDS:=glib2/host

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

MESON_ARGS += \
	-Dtests=disabled \
	-Ddocs=disabled \
	-Dbenchmark=disabled

define Package/harfbuzz
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=hafbuzz
  URL:=http://www.harfbuzz.org/
  DEPENDS:=+glib2 +icu +libcairo +libfreetype
endef

define Package/harfbuzz/description
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig/
endef

define Package/harfbuzz/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,harfbuzz))
