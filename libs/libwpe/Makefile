include $(TOPDIR)/rules.mk

PKG_NAME:=libwpe
PKG_VERSION:=1.16.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://wpewebkit.org/releases
PKG_HASH:=960bdd11c3f2cf5bd91569603ed6d2aa42fd4000ed7cac930a804eac367888d7

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-2-clause
PKG_LICENSE_FILES:=COPYING

PKG_BUILD_DEPENDS:=mesa

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/libwpe
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=WPE backend abstraction library
  URL:=https://wpewebkit.org/
  DEPENDS:=+libstdcpp +libxkbcommon
endef

define Package/libwpe/description
 An Open Source WebKit port for Linux-based embedded devices designed
 with flexibility and hardware acceleration in mind, leveraging common 3D
 graphics APIs for best performance.
endef

MESON_ARGS += \
	-Denable-xkb=true \
	-Dbuild-docs=false \
	-Ddefault_library=shared

define Package/libwpe/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/wpe-1.0/wpe
	$(CP) $(PKG_INSTALL_DIR)/usr/include/wpe-1.0/wpe/*.h $(1)/usr/include/wpe-1.0/wpe
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

$(eval $(call BuildPackage,libwpe))
