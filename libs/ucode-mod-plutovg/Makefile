include $(TOPDIR)/rules.mk

PKG_NAME:=ucode-mod-plutovg
PKG_RELEASE:=1

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=ISC
PKG_LICENSE_FILES:=LICENSE

include $(INCLUDE_DIR)/package.mk

define Package/ucode-mod-plutovg
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=ucode
  TITLE:=ucode binding for the plutovg 2D vector graphics library
  DEPENDS:=+libucode +plutovg
endef

define Package/ucode-mod-plutovg/description
  Exposes the plutovg drawing API to ucode scripts: surfaces, canvases,
  paths, matrices, solid, gradient and texture paints, stroking with dash
  patterns, clipping, compositing and text rendering. Colours, points,
  rectangles, matrices and gradient stops are passed as plain ucode
  objects or arrays.
endef

define Build/Configure
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(TARGET_LDFLAGS) \
		$(FPIC) -Wall -ffunction-sections -Wl,--gc-sections -shared \
		-o $(PKG_BUILD_DIR)/plutovg.so $(PKG_BUILD_DIR)/plutovg.c \
		-lplutovg
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/uc-plutovg.h $(1)/usr/include/
endef

define Package/ucode-mod-plutovg/install
	$(INSTALL_DIR) $(1)/usr/lib/ucode
	$(CP) $(PKG_BUILD_DIR)/plutovg.so $(1)/usr/lib/ucode/
endef

$(eval $(call BuildPackage,ucode-mod-plutovg))
