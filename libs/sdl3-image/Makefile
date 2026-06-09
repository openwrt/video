include $(TOPDIR)/rules.mk

PKG_NAME:=sdl3-image
PKG_VERSION:=3.4.4
PKG_RELEASE:=1

PKG_SOURCE:=SDL3_image-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/libsdl-org/SDL_image/tar.gz/release-$(PKG_VERSION)?
PKG_HASH:=b0c11bbde540e26d1cedf31174349fe6ab67e57658efe22e16e75172859c817d

PKG_BUILD_DIR:=$(BUILD_DIR)/SDL_image-release-$(PKG_VERSION)

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=Zlib
PKG_LICENSE_FILES:=LICENSE.txt

CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_BINARY_SUBDIR:=openwrt-build

CMAKE_OPTIONS += \
	-DSDLIMAGE_SAMPLES=OFF \
	-DSDLIMAGE_TESTS=OFF \
	-DSDLIMAGE_INSTALL=ON \
	-DSDLIMAGE_VENDORED=OFF \
	-DSDLIMAGE_DEPS_SHARED=OFF \
	-DSDLIMAGE_BACKEND_STB=OFF \
	-DSDLIMAGE_PNG=ON \
	-DSDLIMAGE_PNG_LIBPNG=ON \
	-DSDLIMAGE_JPG=ON \
	-DSDLIMAGE_TIF=ON \
	-DSDLIMAGE_WEBP=ON \
	-DSDLIMAGE_AVIF=OFF \
	-DSDLIMAGE_JXL=OFF

define Package/libsdl3-image
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=SDL3 Image
  URL:=https://www.libsdl.org/
  DEPENDS:=+libsdl3 +libpng +libjpeg +libtiff +libwebp
endef

define Package/libsdl3-image/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libSDL3_image*.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libsdl3-image))
