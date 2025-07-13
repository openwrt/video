include $(TOPDIR)/rules.mk

PKG_NAME:=gdk-pixbuf
PKG_VERSION:=2.42.12
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=@GNOME/$(PKG_NAME)/$(basename $(PKG_VERSION))
PKG_HASH:=b9505b3445b9a7e48ced34760c3bcb73e966df3ac94c95a148cb669ab748e3c7

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=LGPL-2.1-or-later
PKG_LICENSE_FILES:=COPYING
PKG_CPE_ID:=cpe:/a:gnome:gdk-pixbuf

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

MESON_ARGS += \
	-Dgtk_doc=false \
	-Dinstalled_tests=false \
	-Dman=false \
	-Dtests=false

define Package/libgdk-pixbuf
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  DEPENDS:=+giflib +libpng +libtiff +libjpeg +shared-mime-info
  TITLE:=GdkPixbuf: Image loading library
  URL:=http://www.gtk.org/
endef

define Package/libgdk-pixbuf/description
  GdkPixbuf is a library that loads image data in various formats and stores it as linear buffers in memory.
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/gdk-pixbuf-2.0/gdk-pixbuf
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/include/gdk-pixbuf-2.0/gdk-pixbuf/*.h \
		$(1)/usr/include/gdk-pixbuf-2.0/gdk-pixbuf/

	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/*.{so*,a} \
		$(1)/usr/lib/

	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc \
		$(1)/usr/lib/pkgconfig
endef

define Package/libgdk-pixbuf/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/

	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/lib/
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,libgdk-pixbuf))
