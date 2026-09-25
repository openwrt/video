include $(TOPDIR)/rules.mk

PKG_NAME:=plutosvg
PKG_VERSION:=0.0.8
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/sammycage/plutosvg/tar.gz/v$(PKG_VERSION)?
PKG_HASH:=49d5cfe772d3aa10cd4879f2f6e189f5083c08e4c8ea01bf3d5b87c97dfca7d2

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

# plutosvg ships subprojects/plutovg.wrap and declares it as a fallback,
# so a failed dependency check would quietly build and stage a second
# copy of plutovg over the packaged one.
MESON_ARGS += \
	--wrap-mode=nofallback \
	-Dexamples=disabled \
	-Dtests=disabled \
	-Dfreetype=disabled

define Package/plutosvg
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=SVG rendering library
  URL:=https://github.com/sammycage/plutosvg
  DEPENDS:=+plutovg
endef

define Package/plutosvg/description
  Parses SVG documents and renders them through plutovg. Built without
  the optional freetype support, which only serves OpenType SVG glyphs.
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/plutosvg $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libplutosvg.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/plutosvg.pc $(1)/usr/lib/pkgconfig/
endef

define Package/plutosvg/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libplutosvg.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,plutosvg))
