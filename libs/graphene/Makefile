include $(TOPDIR)/rules.mk

PKG_NAME:=graphene
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_DATE:=2025-01-23
PKG_SOURCE_VERSION:=47b503d6086900057ceefab785546ad524caa735
PKG_SOURCE_URL:=https://github.com/ebassi/graphene
PKG_MIRROR_HASH:=8c150016ca717c6a829c46c8617f7a5a9136a9b9a918b078d97ee0cb7ee6991c

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
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,libgraphene))
