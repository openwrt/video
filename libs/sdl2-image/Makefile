include $(TOPDIR)/rules.mk

PKG_NAME:=sdl2-image
PKG_VERSION:=2.8.12
PKG_RELEASE:=1

PKG_SOURCE:=SDL2_image-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/libsdl-org/SDL_image/tar.gz/release-$(PKG_VERSION)?
PKG_HASH:=26f07b431d09a9fb2cd759268030a1c34de44328d17f2a304f0612703a0d8cab

PKG_BUILD_DIR:=$(BUILD_DIR)/SDL_image-release-$(PKG_VERSION)

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=Zlib
PKG_LICENSE_FILES:=LICENSE.txt

CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_BINARY_SUBDIR:=openwrt-build

CMAKE_OPTIONS += \
	-DSDL2IMAGE_SAMPLES=OFF \
	-DSDL2IMAGE_TESTS=OFF \
	-DSDL2IMAGE_INSTALL=ON \
	-DSDL2IMAGE_VENDORED=OFF \
	-DSDL2IMAGE_DEPS_SHARED=OFF \
	-DSDL2IMAGE_BACKEND_STB=ON \
	-DSDL2IMAGE_AVIF=OFF \
	-DSDL2IMAGE_JXL=OFF \
	-DSDL2IMAGE_TIF=OFF \
	-DSDL2IMAGE_WEBP=OFF

define Build/InstallDev
	$(call Build/InstallDev/cmake,$(1))
ifdef CONFIG_DEBUG
	$(LN) libSDL2_imaged.so $(1)/usr/lib/libSDL2_image.so
endif
endef

define Package/libsdl2-image
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=SDL2 Image
  URL:=https://www.libsdl.org/
  DEPENDS:=+libsdl2
endef

define Package/libsdl2-image/description
  SDL_image is an image file loading library for SDL2. PNG and JPEG are
  handled by the bundled stb_image backend, alongside built-in support for
  BMP, GIF, LBM, PCX, PNM, QOI, SVG, TGA, XCF, XPM and XV.
endef

define Package/libsdl2-image/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib/
ifdef CONFIG_DEBUG
	$(LN) libSDL2_imaged.so $(1)/usr/lib/libSDL2_image.so
endif
endef

$(eval $(call BuildPackage,libsdl2-image))
