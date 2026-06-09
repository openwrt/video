include $(TOPDIR)/rules.mk

PKG_NAME:=sdl3-mixer
PKG_VERSION:=3.2.4
PKG_RELEASE:=1

PKG_SOURCE:=SDL3_mixer-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/libsdl-org/SDL_mixer/tar.gz/release-$(PKG_VERSION)?
PKG_HASH:=f2ea848ccdf2f394cd4973ee0f6c482e04511044695cccfd46bab6dcd7f780aa

PKG_BUILD_DIR:=$(BUILD_DIR)/SDL_mixer-release-$(PKG_VERSION)

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-3c
PKG_LICENSE_FILES:=LICENSE.txt

CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_BINARY_SUBDIR:=openwrt-build

CMAKE_OPTIONS += \
	-DSDLMIXER_SAMPLES=OFF \
	-DSDLMIXER_VENDORED=OFF \
	-DSDLMIXER_MIDI_FLUIDSYNTH_SHARED=ON

define Build/InstallDev
	$(call Build/InstallDev/cmake,$(1))
endef

define Package/libsdl3-mixer
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=SDL3 Mixer
  URL:=https://www.libsdl.org/
  DEPENDS:=+libsdl3 +libfluidsynth +libopusfile +libwavpack +libxmp
endef

define Package/libsdl3-mixer/description
  SDL3 Audio Mixer library
endef

define Package/libsdl3-mixer/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libSDL3_mixer*.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libsdl3-mixer))
