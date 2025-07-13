include $(TOPDIR)/rules.mk

PKG_NAME:=pango
PKG_VERSION:=1.56.3
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=@GNOME/$(PKG_NAME)/$(basename $(PKG_VERSION))
PKG_HASH:=2606252bc25cd8d24e1b7f7e92c3a272b37acd6734347b73b47a482834ba2491

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=LGPL-2.0-or-later
PKG_LICENSE_FILES:=COPYING
PKG_CPE_ID:=cpe:/a:gnome:pango

PKG_BUILD_DEPENDS:=glib2/host
PKG_INSTALL:=1

PKG_FORTIFY_SOURCE:=0

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include $(INCLUDE_DIR)/meson.mk

define Package/libpango
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=libpango
  URL:=https://www.pango.org/
  DEPENDS:=+glib2 +fribidi +harfbuzz +libcairo $(ICONV_DEPENDS) $(INTL_DEPENDS)
endef

MESON_ARGS += \
	-Dintrospection=disabled \
	-Dgtk_doc=false \
	-Dsysprof=disabled

define package/libpango/decription
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/include/pango-1.0/pango
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/include/pango-1.0/pango/*.h $(1)/usr/include/pango-1.0/pango/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig/

endef

define Package/libpango/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libpango))
