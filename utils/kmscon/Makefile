include $(TOPDIR)/rules.mk

PKG_NAME:=kmscon
PKG_VERSION:=10.0.3
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/kmscon/kmscon/tar.gz/v$(PKG_VERSION)?
PKG_HASH:=4a0552d3cd3ba16df4fcff61f2b1431650bcd9e7ecabc03625ce52345b9c95c5

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

MESON_ARGS += \
	-Dtests=false \
	-Ddocs=disabled \
	-Dlibseat=enabled \
	-Ddbus=disabled \
	-Dvideo_fbdev=disabled \
	-Dvideo_drm2d=enabled \
	-Dvideo_drm3d=enabled \
	-Drenderer_gltex=enabled \
	-Dfont_psf=enabled \
	-Dfont_unifont=disabled \
	-Dfont_freetype=disabled \
	-Dfont_pango=disabled

define Package/kmscon
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Terminal emulator on a KMS device
  URL:=https://github.com/kmscon/kmscon
  DEPENDS:=+libtsm +libxkbcommon +libudev +libdrm +libmesa +zlib \
	+xkeyboard-config +libseat +seatd
endef

define Package/kmscon/description
  A terminal emulator that draws directly on a DRM/KMS device, so a
  system needs no framebuffer console to have a text console. Renders
  through OpenGL ES where the GPU allows and falls back to software
  drawing on the same KMS device.
endef

define Package/kmscon/conffiles
/etc/config/kmscon
endef

define Package/kmscon/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/kmscon $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/lib/kmscon
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/kmscon/*.so $(1)/usr/lib/kmscon/
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/kmscon.config $(1)/etc/config/kmscon
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/kmscon.init $(1)/etc/init.d/kmscon
endef

$(eval $(call BuildPackage,kmscon))
