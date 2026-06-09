include $(TOPDIR)/rules.mk

PKG_NAME:=vice
PKG_VERSION:=3.10
PKG_RELEASE:=1

PKG_SOURCE:=vice-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/vice-emu/releases
PKG_HASH:=8e5bac18cbcb9f192380ad3ef881f8790f5b75c41d7b3da65d831985d864d6d1

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=COPYING

PKG_BUILD_DEPENDS:=xa/host
PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/vice
  SECTION:=games
  CATEGORY:=Games
  TITLE:=Versatile Commodore Emulator (SDL2)
  URL:=https://vice-emu.sourceforge.io/
  DEPENDS:=+libsdl2 +libsdl2-image +libpng +zlib +libstdcpp +alsa-lib
endef

define Package/vice/description
  VICE is an emulator for the Commodore 8-bit machines: C64, C64DTV, C128,
  VIC20, all the PET models (except the SuperPET 9000), the PLUS4 and the
  CBM-II (aka C610/C510). This package is built against SDL2.
endef

CONFIGURE_VARS += \
	DOS2UNIX=true \
	MAKEINFO=true

CONFIGURE_ARGS += \
	--enable-sdl2ui \
	--disable-sdl1ui \
	--disable-gtk3ui \
	--disable-headlessui \
	--disable-html-docs \
	--without-pulse \
	--without-portaudio \
	--without-oss \
	--without-vorbis \
	--without-flac \
	--without-lame \
	--without-mpg123 \
	--without-libcurl \
	--disable-ethernet

define Package/vice/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/share/vice
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vice/* $(1)/usr/share/vice/
endef

$(eval $(call BuildPackage,vice))
