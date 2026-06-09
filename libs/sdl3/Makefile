include $(TOPDIR)/rules.mk

PKG_NAME:=sdl3
PKG_VERSION:=3.4.10
PKG_RELEASE:=1

PKG_SOURCE:=SDL3-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/libsdl-org/SDL/releases/download/release-$(PKG_VERSION)/
PKG_HASH:=12b34280415ec8418c864408b93d008a20a6530687ee613d60bfbd20411f2785

PKG_BUILD_DIR:=$(BUILD_DIR)/SDL3-$(PKG_VERSION)

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=Zlib
PKG_LICENSE_FILES:=LICENSE.txt

CMAKE_INSTALL:=1
PKG_CONFIG_DEPENDS:=CONFIG_PACKAGE_libsdl3-tests
PKG_BUILD_DEPENDS:=wayland/host libwayland wayland-protocols libxkbcommon

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_BINARY_SUBDIR:=openwrt-build

CMAKE_OPTIONS += \
	-DSDL_SHARED=ON \
	-DSDL_STATIC=OFF \
	-DSDL_TEST_LIBRARY=$(if $(CONFIG_PACKAGE_libsdl3-tests),ON,OFF) \
	-DSDL_TESTS=$(if $(CONFIG_PACKAGE_libsdl3-tests),ON,OFF) \
	-DSDL_INSTALL_TESTS=$(if $(CONFIG_PACKAGE_libsdl3-tests),ON,OFF) \
	-DSDL_ALSA=ON \
	-DSDL_ALSA_SHARED=ON \
	-DSDL_CLOCK_GETTIME=ON \
	-DSDL_DBUS=OFF \
	-DSDL_DISKAUDIO=OFF \
	-DSDL_DUMMYAUDIO=ON \
	-DSDL_DUMMYVIDEO=ON \
	-DSDL_GCC_ATOMICS=ON \
	-DSDL_HIDAPI=ON \
	-DSDL_HIDAPI_JOYSTICK=ON \
	-DSDL_HIDAPI_LIBUSB=ON \
	-DSDL_HIDAPI_LIBUSB_SHARED=ON \
	-DSDL_IBUS=OFF \
	-DSDL_JACK=OFF \
	-DSDL_KMSDRM=ON \
	-DSDL_KMSDRM_SHARED=ON \
	-DSDL_LIBC=ON \
	-DSDL_LIBICONV=OFF \
	-DSDL_LIBUDEV=ON \
	-DSDL_OFFSCREEN=ON \
	-DSDL_OPENGL=ON \
	-DSDL_OPENGLES=ON \
	-DSDL_OSS=OFF \
	-DSDL_PIPEWIRE=OFF \
	-DSDL_PTHREADS=ON \
	-DSDL_PTHREADS_SEM=ON \
	-DSDL_PULSEAUDIO=ON \
	-DSDL_PULSEAUDIO_SHARED=ON \
	-DSDL_RPATH=OFF \
	-DSDL_RPI=OFF \
	-DSDL_SNDIO=OFF \
	-DSDL_SYSTEM_ICONV=OFF \
	-DSDL_VIVANTE=OFF \
	-DSDL_VULKAN=ON \
	-DSDL_WAYLAND=ON \
	-DSDL_WAYLAND_LIBDECOR=OFF \
	-DSDL_WAYLAND_SHARED=ON \
	-DSDL_X11=OFF

define Package/libsdl3
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Simple DirectMedia Layer 3
  URL:=https://www.libsdl.org
  DEPENDS:=+libstdcpp +alsa-lib +libusb-1.0 +libudev-zero +pulseaudio +libmesa
endef

define Package/libsdl3/description
 Simple DirectMedia Layer is a cross-platform development library designed to
 provide low level access to audio, keyboard, mouse, joystick, and graphics
 hardware via OpenGL and Direct3D.
endef

define Build/InstallDev
	$(call Build/InstallDev/cmake,$(1))
endef

define Package/libsdl3/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libSDL3*.so* $(1)/usr/lib
endef

define Package/libsdl3-tests
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=Video
  TITLE:=SDL3 installed tests
  URL:=https://www.libsdl.org
  DEPENDS:=+libsdl3
endef

define Package/libsdl3-tests/description
 Various test programs to validate SDL3 is working correctly, and some benchmarks.
endef

define Package/libsdl3-tests/install
	$(INSTALL_DIR) $(1)/usr/libexec/installed-tests
	$(CP) $(PKG_INSTALL_DIR)/usr/libexec/installed-tests/* $(1)/usr/libexec/installed-tests
	$(INSTALL_DIR) $(1)/usr/share/installed-tests
	$(CP) $(PKG_INSTALL_DIR)/usr/share/installed-tests/* $(1)/usr/share/installed-tests
endef

$(eval $(call BuildPackage,libsdl3))
$(eval $(call BuildPackage,libsdl3-tests))
