include $(TOPDIR)/rules.mk

PKG_NAME:=sdl2-mixer
PKG_VERSION:=2.8.1
PKG_RELEASE:=1

PKG_SOURCE:=SDL2_mixer-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/libsdl-org/SDL_mixer/tar.gz/release-$(PKG_VERSION)?
PKG_HASH:=63804b4b2ba503865c0853f102231aeff489b1dfc6dea4750a69e2a8ef54b2bb

PKG_BUILD_DIR:=$(BUILD_DIR)/SDL_mixer-release-$(PKG_VERSION)

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-3c
PKG_LICENSE_FILES:=COPYING LICENSE

CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_BINARY_SUBDIR:=openwrt-build

CMAKE_OPTIONS += \
	-DSDL2MIXER_MIDI_FLUIDSYNTH_SHARED=ON

define Build/InstallDev
	$(call Build/InstallDev/cmake,$(1))
ifdef CONFIG_DEBUG
	$(LN) libSDL2_mixerd.so $(1)/usr/lib/libSDL2_mixer.so
endif
endef

define Package/libsdl2-mixer
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=SDL2 Mixer
  URL:=https://www.libsdl.org/
  DEPENDS:=+libfluidsynth +libopusfile +libsdl2 +libwavpack +libxmp
endef

define Package/libsdl2-mixer/description
  SDL Audio Mixer library
endef

define Package/libsdl2-mixer/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib/
ifdef CONFIG_DEBUG
	$(LN) libSDL2_mixerd.so $(1)/usr/lib/libSDL2_mixer.so
endif
endef

$(eval $(call BuildPackage,libsdl2-mixer))
