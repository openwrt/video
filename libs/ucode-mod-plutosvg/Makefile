include $(TOPDIR)/rules.mk

PKG_NAME:=ucode-mod-plutosvg
PKG_RELEASE:=1

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=ISC
PKG_LICENSE_FILES:=LICENSE

include $(INCLUDE_DIR)/package.mk

define Package/ucode-mod-plutosvg
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=ucode
  TITLE:=ucode binding for the plutosvg SVG rendering library
  DEPENDS:=+libucode +plutovg +plutosvg +ucode-mod-plutovg
endef

define Package/ucode-mod-plutosvg/description
  Exposes the plutosvg document API to ucode scripts: loading SVG
  documents from memory or from a file, querying their intrinsic size and
  element extents, and rendering a document or a single element onto a
  plutovg canvas or into a fresh surface. CSS currentColor and colour
  variables can be resolved from a ucode object or callback.
endef

define Build/Configure
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(TARGET_LDFLAGS) \
		$(FPIC) -Wall -ffunction-sections -Wl,--gc-sections -shared \
		-I$(STAGING_DIR)/usr/include/plutovg \
		-o $(PKG_BUILD_DIR)/plutosvg.so $(PKG_BUILD_DIR)/plutosvg.c \
		-lplutosvg -lplutovg
endef

define Package/ucode-mod-plutosvg/install
	$(INSTALL_DIR) $(1)/usr/lib/ucode
	$(CP) $(PKG_BUILD_DIR)/plutosvg.so $(1)/usr/lib/ucode/
endef

$(eval $(call BuildPackage,ucode-mod-plutosvg))
