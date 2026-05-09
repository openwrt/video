include $(TOPDIR)/rules.mk

PKG_NAME:=graphene
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_DATE:=2025-12-19
PKG_SOURCE_VERSION:=98173e59a3d80d3dd5ad6e4eaab919b4649ac7e5
PKG_SOURCE_URL:=https://github.com/ebassi/graphene
PKG_MIRROR_HASH:=841b66ddff456352f19e617ca0bedf70950834cbe230f3632038aa007e2dbb57

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE.txt

PKG_INSTALL:=1
PKG_FORTIFY_SOURCE:=0

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

MESON_ARGS += \
	-Dgtk_doc=false \
	-Dgobject_types=true \
	-Dintrospection=disabled \
	-Dtests=false \
	-Dinstalled_tests=false

define Package/libgraphene
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  DEPENDS:=+glib2
  TITLE:=Graphene
  URL:=https://github.com/ebassi/graphene
endef

define Package/libgraphene/description
 A thin layer of types for graphic libraries
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/graphene-1.0
	$(CP) $(PKG_INSTALL_DIR)/usr/include/graphene-1.0/*.h $(1)/usr/include/graphene-1.0
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/graphene-1.0/include/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/graphene-1.0/include/*.h $(1)/usr/lib/graphene-1.0/include
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

define Package/libgraphene/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,libgraphene))
