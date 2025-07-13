include $(TOPDIR)/rules.mk

PKG_NAME:=wpebackend-fdo
PKG_VERSION:=1.16.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://wpewebkit.org/releases
PKG_HASH:=beddf321232d5bd08106c179dbc600f8ce88eb3620b4a59a6329063b78f64635

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-2-clause
PKG_LICENSE_FILES:=COPYING

PKG_BUILD_DEPENDS:=mesa

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/wpebackend-fdo
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=WPE's backend based on a freedesktop.org stack
  URL:=https://wpewebkit.org/
  DEPENDS:=+glib2 +libepoxy +libstdcpp +libwayland +libwpe
endef

define Package/wpebackend-fdo/description
 An Open Source WebKit port for Linux-based embedded devices designed
 with flexibility and hardware acceleration in mind, leveraging common 3D
 graphics APIs for best performance.
endef

MESON_ARGS += \
	-Dbuild_docs=false

define Package/wpebackend-fdo/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/wpe-fdo-1.0/wpe
	$(CP) $(PKG_INSTALL_DIR)/usr/include/wpe-fdo-1.0/wpe/* $(1)/usr/include/wpe-fdo-1.0/wpe
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

$(eval $(call BuildPackage,wpebackend-fdo))
