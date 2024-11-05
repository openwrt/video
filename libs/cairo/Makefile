include $(TOPDIR)/rules.mk

PKG_NAME:=cairo
PKG_VERSION:=1.18.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://cairo.freedesktop.org/releases/
PKG_HASH:=a62b9bb42425e844cc3d6ddde043ff39dbabedd1542eba57a2eb79f85889d45a

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=LGPL-2.1-or-later MPL-1.1
PKG_LICENSE_FILES:=COPYING COPYING-LGPL-2.1 COPYING-MPL-1.1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/libcairo
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=cairo
  URL:=https://cairo.freedesktop.org/
  DEPENDS:=+libpng +libfreetype +fontconfig +libmesa +pixman +glib2 +libjpeg +zlib +liblzo
endef

define Package/libcairo/description
  Cairo is a 2D graphics library with support for multiple output devices.
endef

MESON_ARGS += \
	-Ddwrite=disabled \
	-Dfontconfig=enabled \
	-Dfreetype=enabled \
	-Dpng=enabled \
	-Dxcb=disabled \
	-Dxlib=disabled \
	-Dxlib-xcb=disabled \
	-Dzlib=enabled

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/cairo
	$(CP) $(PKG_INSTALL_DIR)/usr/include/cairo/*.h $(1)/usr/include/cairo
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/cairo
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/cairo/*.so $(1)/usr/lib/cairo
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

define Package/libcairo/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/cairo
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/cairo/*.so $(1)/usr/lib/cairo
endef

$(eval $(call BuildPackage,libcairo))
