include $(TOPDIR)/rules.mk

PKG_NAME:=plutovg
PKG_VERSION:=1.3.3
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/sammycage/plutovg/tar.gz/v$(PKG_VERSION)?
PKG_HASH:=2b0d17a6e016f47b86f9c00e2cb82600041b1ea1f7d2a00c2d46ae542cbfed3c

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

MESON_ARGS += \
	--wrap-mode=nofallback \
	-Dexamples=disabled \
	-Dtests=disabled \
	-Dfont-face-cache-load=disabled

define Package/plutovg
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=2D vector graphics library
  URL:=https://github.com/sammycage/plutovg
endef

define Package/plutovg/description
  Software rasteriser for 2D vector graphics: path filling, stroking and
  dashing, solid, gradient and texture paints, clipping, compositing and
  transformations, onto a 32-bit premultiplied ARGB surface that can wrap
  memory the caller already owns. Depends on nothing but libc.
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/plutovg $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libplutovg.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/plutovg.pc $(1)/usr/lib/pkgconfig/
endef

define Package/plutovg/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libplutovg.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,plutovg))
