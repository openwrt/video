include $(TOPDIR)/rules.mk

PKG_NAME:=libdisplay-info
PKG_VERSION:=0.2.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://gitlab.freedesktop.org/emersion/libdisplay-info/-/archive/$(PKG_VERSION)
PKG_HASH:=f7331fcaf5527251b84c8fb84238d06cd2f458422ce950c80e86c72927aa8c2b

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE

PKG_BUILD_DEPENDS:=hwdata

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/libdisplay-info
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=EDID and DisplayID library
  DEPENDS:=+edid-decode
  URL:=https://gitlab.freedesktop.org/emersion/libdisplay-info
endef

define Package/libdisplay-info/description
  EDID and DisplayID library for use with compositors.
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/libdisplay-info
	$(CP) $(PKG_INSTALL_DIR)/usr/include/libdisplay-info/*.h $(1)/usr/include/libdisplay-info
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

define Package/libdisplay-info/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,libdisplay-info))
