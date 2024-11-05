include $(TOPDIR)/rules.mk

PKG_NAME:=graphene
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_DATE:=2024-08-19
PKG_SOURCE_VERSION:=d63bd69609a43558e758dad74bd1a2d480d335ca
PKG_SOURCE_URL:=https://github.com/ebassi/graphene
PKG_MIRROR_HASH:=464fa9572b2c92c1cdc7a460d03771021767e4139ff1ae0d181c141ac3d47391

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
