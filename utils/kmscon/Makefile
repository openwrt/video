include $(TOPDIR)/rules.mk

PKG_NAME:=kmscon
PKG_VERSION:=10.0.3
PKG_RELEASE:=6

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/kmscon/kmscon/tar.gz/v$(PKG_VERSION)?
PKG_HASH:=4a0552d3cd3ba16df4fcff61f2b1431650bcd9e7ecabc03625ce52345b9c95c5

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1
PKG_BUILD_DEPENDS:=kmscon/host

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/meson.mk

PKG_CONFIG_DEPENDS:= \
	CONFIG_PACKAGE_kmscon-mod-gl \
	CONFIG_PACKAGE_kmscon-mod-unifont

MESON_ARGS += \
	-Dtests=false \
	-Ddocs=disabled \
	-Dlibseat=enabled \
	-Ddbus=disabled \
	-Dvideo_fbdev=disabled \
	-Dvideo_drm2d=enabled \
	-Dfont_psf=enabled \
	-Dfont_freetype=disabled \
	-Dfont_pango=disabled

ifdef CONFIG_PACKAGE_kmscon-mod-unifont
MESON_ARGS += -Dfont_unifont=enabled
else
MESON_ARGS += -Dfont_unifont=disabled
endif

ifdef CONFIG_PACKAGE_kmscon-mod-gl
MESON_ARGS += \
	-Dvideo_drm3d=enabled \
	-Drenderer_gltex=enabled
else
MESON_ARGS += \
	-Dvideo_drm3d=disabled \
	-Drenderer_gltex=disabled
endif

define Host/Configure
endef

define Host/Compile
	$(HOSTCC) $(HOST_CFLAGS) $(HOST_CPPFLAGS) \
		-o $(HOST_BUILD_DIR)/genunifont \
		$(HOST_BUILD_DIR)/src/font/genunifont.c $(HOST_LDFLAGS) -lz
endef

define Host/Install
	$(INSTALL_DIR) $(HOST_BUILD_PREFIX)/bin
	$(INSTALL_BIN) $(HOST_BUILD_DIR)/genunifont $(HOST_BUILD_PREFIX)/bin/
endef

define Host/Uninstall
	rm -f $(HOST_BUILD_PREFIX)/bin/genunifont
endef

define Package/kmscon
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Terminal emulator on a KMS device
  URL:=https://github.com/kmscon/kmscon
  DEPENDS:=input-support +kmod-input-evdev +libtsm +libxkbcommon +libudev \
	+libdrm +zlib +xkeyboard-config +libseat +seatd
endef

define Package/kmscon/description
  A terminal emulator that draws directly on a DRM/KMS device, so a
  system needs no framebuffer console to have a text console. Draws in
  software on the KMS device; install kmscon-mod-gl for the OpenGL ES
  path. The built-in font renders ASCII only; install
  kmscon-mod-unifont for the rest of Unicode.
endef

define Package/kmscon-mod-gl
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=OpenGL ES rendering for kmscon
  URL:=https://github.com/kmscon/kmscon
  DEPENDS:=kmscon +libmesa
endef

define Package/kmscon-mod-gl/description
  The drm3d video backend and the gltex renderer, which draw through
  OpenGL ES instead of in software. kmscon loads them if they are
  present and uses them only when started with --hwaccel. Worth
  installing on a system that carries Mesa anyway.
endef

define Package/kmscon-mod-unifont
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=GNU Unifont font engine for kmscon
  URL:=https://github.com/kmscon/kmscon
  LICENSE:=MIT OFL-1.1
  DEPENDS:=kmscon
endef

define Package/kmscon-mod-unifont/description
  A font engine carrying GNU Unifont, a bitmap font covering the Basic
  Multilingual Plane, so box drawing, symbols and wide characters are
  drawn instead of replaced. kmscon picks it when no font engine is
  configured.
endef

define Package/kmscon/conffiles
/etc/config/kmscon
endef

define Package/kmscon/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/kmscon $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/lib/kmscon
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/kmscon/mod-psf.so $(1)/usr/lib/kmscon/
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/kmscon.config $(1)/etc/config/kmscon
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/kmscon.init $(1)/etc/init.d/kmscon
endef

define Package/kmscon-mod-gl/install
	$(INSTALL_DIR) $(1)/usr/lib/kmscon
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/kmscon/mod-drm3d.so $(1)/usr/lib/kmscon/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/kmscon/mod-gltex.so $(1)/usr/lib/kmscon/
endef

define Package/kmscon-mod-unifont/install
	$(INSTALL_DIR) $(1)/usr/lib/kmscon
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/kmscon/mod-unifont.so $(1)/usr/lib/kmscon/
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,kmscon))
$(eval $(call BuildPackage,kmscon-mod-gl))
$(eval $(call BuildPackage,kmscon-mod-unifont))
