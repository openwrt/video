include $(TOPDIR)/rules.mk

PKG_NAME:=sdl2-compat
PKG_VERSION:=2.32.70
PKG_RELEASE:=1

PKG_SOURCE:=sdl2-compat-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/libsdl-org/sdl2-compat/releases/download/release-$(PKG_VERSION)/
PKG_HASH:=998fa62557eb46ffe7e5c3e2c123bc332f7df9d9f593b3ceed88ed1158428a44

PKG_BUILD_DIR:=$(BUILD_DIR)/sdl2-compat-$(PKG_VERSION)

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=Zlib
PKG_LICENSE_FILES:=LICENSE.txt

CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_BINARY_SUBDIR:=openwrt-build

CMAKE_OPTIONS += \
	-DSDL2COMPAT_INSTALL=ON \
	-DSDL2COMPAT_INSTALL_TESTS=OFF \
	-DSDL2COMPAT_STATIC=OFF \
	-DSDL2COMPAT_TESTS=OFF \
	-DSDL2COMPAT_X11=OFF

define Package/libsdl2-compat
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=SDL2 compatibility layer built on SDL3
  URL:=https://www.libsdl.org
  DEPENDS:=+libsdl3
  PROVIDES:=libsdl2
endef

define Package/libsdl2-compat/description
 sdl2-compat is an implementation of the SDL2 API on top of SDL3,
 letting SDL2 applications and libraries run against a single SDL3 build.
 It ships the libSDL2 shared library and SDL2 development files.
endef

define Build/InstallDev
	$(call Build/InstallDev/cmake,$(1))
	$(SED) 's|^libdir=.*|libdir=\x24{exec_prefix}/lib|' \
		$(1)/usr/lib/pkgconfig/sdl2-compat.pc
	$(SED) 's|^includedir=.*|includedir=\x24{prefix}/include|' \
		$(1)/usr/lib/pkgconfig/sdl2-compat.pc
endef

define Package/libsdl2-compat/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libSDL2*.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,libsdl2-compat))
