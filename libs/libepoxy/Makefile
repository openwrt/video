include $(TOPDIR)/rules.mk

PKG_NAME:=libepoxy
PKG_VERSION:=1.5.10
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/anholt/libepoxy/tar.gz/$(PKG_VERSION)?
PKG_HASH:=a7ced37f4102b745ac86d6a70a9da399cc139ff168ba6b8002b4d8d43c900c15

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/libepoxy
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=Epoxy OpenGL function pointer management
  URL:=https://github.com/anholt/libepoxy
  DEPENDS:=+libmesa
endef

define Package/libepoxy/description
 Epoxy is a library for handling OpenGL function pointer management for you.
endef

MESON_ARGS += \
	-Ddocs=false \
	-Dglx=no \
	-Degl=yes \
	-Dx11=false \
	-Dtests=false

define Package/libepoxy/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/epoxy
	$(CP) $(PKG_INSTALL_DIR)/usr/include/epoxy/*.h $(1)/usr/include/epoxy
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

$(eval $(call BuildPackage,libepoxy))
