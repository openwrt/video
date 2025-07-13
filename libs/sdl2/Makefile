include $(TOPDIR)/rules.mk

PKG_NAME:=sdl2
PKG_VERSION:=2.32.8
PKG_RELEASE:=1

PKG_SOURCE:=SDL2-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://www.libsdl.org/release/
PKG_HASH:=0ca83e9c9b31e18288c7ec811108e58bac1f1bb5ec6577ad386830eac51c787e

PKG_BUILD_DIR:=$(BUILD_DIR)/SDL2-$(PKG_VERSION)

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-3c
PKG_LICENSE_FILES:=COPYING LICENSE

CMAKE_INSTALL:=1
PKG_CONFIG_DEPENDS:=CONFIG_PACKAGE_libsdl2-tests

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_BINARY_SUBDIR:=openwrt-build

CMAKE_OPTIONS += \
	-DSDL_ALSA=ON \
	-DSDL_ALSA_SHARED=ON \
	-DSDL_ARTS=OFF \
	-DSDL_ARTS_SHARED=OFF \
	-DSDL_ASAN=OFF \
	-DSDL_BACKGROUNDING_SIGNAL=OFF \
	-DSDL_CCACHE=OFF \
	-DSDL_CLOCK_GETTIME=ON \
	-DSDL_COCOA=OFF \
	-DSDL_DBUS=OFF \
	-DSDL_DIRECTFB=OFF \
	-DSDL_DIRECTFB_SHARED=OFF \
	-DSDL_DIRECTX=OFF \
	-DSDL_DISKAUDIO=OFF \
	-DSDL_DUMMYAUDIO=ON \
	-DSDL_DUMMYVIDEO=ON \
	-DSDL_ESD=OFF \
	-DSDL_ESD_SHARED=OFF \
	-DSDL_FOREGROUNDING_SIGNAL=OFF \
	-DSDL_FUSIONSOUND=OFF \
	-DSDL_FUSIONSOUND_SHARED=OFF \
	-DSDL_GCC_ATOMICS=ON \
	-DSDL_HIDAPI=ON \
	-DSDL_HIDAPI_JOYSTICK=ON \
	-DSDL_HIDAPI_LIBUSB=ON \
	-DSDL_IBUS=OFF \
	-DSDL_JACK=OFF \
	-DSDL_JACK_SHARED=OFF \
	-DSDL_KMSDRM=ON \
	-DSDL_KMSDRM_SHARED=ON \
	-DSDL_LASX=OFF \
	-DSDL_LIBC=ON \
	-DSDL_LIBICONV=OFF \
	-DSDL_LIBSAMPLERATE=ON \
	-DSDL_LIBSAMPLERATE_SHARED=ON \
	-DSDL_LIBUDEV=ON \
	-DSDL_LSX=OFF \
	-DSDL_METAL=OFF \
	-DSDL_MMX=OFF \
	-DSDL_NAS=OFF \
	-DSDL_NAS_SHARED=OFF \
	-DSDL_OFFSCREEN=ON \
	-DSDL_OPENGL=ON \
	-DSDL_OPENGLES=ON \
	-DSDL_OSS=OFF \
	-DSDL_PIPEWIRE=OFF \
	-DSDL_PIPEWIRE_SHARED=OFF \
	-DSDL_PTHREADS=ON \
	-DSDL_PTHREADS_SEM=ON \
	-DSDL_PULSEAUDIO=ON \
	-DSDL_PULSEAUDIO_SHARED=ON \
	-DSDL_RENDER_D3D=OFF \
	-DSDL_RENDER_METAL=OFF \
	-DSDL_RPATH=OFF \
	-DSDL_RPI=OFF \
	-DSDL_SNDIO=OFF \
	-DSDL_SNDIO_SHARED=OFF \
	-DSDL_SYSTEM_ICONV=OFF \
	-DSDL_TESTS=$(if $(CONFIG_PACKAGE_libsdl2-tests),ON,OFF) \
	-DSDL_INSTALL_TESTS=$(if $(CONFIG_PACKAGE_libsdl2-tests),ON,OFF) \
	-DSDL_VENDOR_INFO=OFF \
	-DSDL_VIRTUAL_JOYSTICK=OFF \
	-DSDL_VIVANTE=OFF \
	-DSDL_VULKAN=ON \
	-DSDL_WASAPI=OFF \
	-DSDL_WAYLAND=ON \
	-DSDL_WAYLAND_LIBDECOR=OFF \
	-DSDL_WAYLAND_LIBDECOR_SHARED=OFF \
	-DSDL_WAYLAND_QT_TOUCH=ON \
	-DSDL_WAYLAND_SHARED=ON \
	-DSDL_X11=OFF \
	-DSDL_X11_SHARED=OFF \
	-DSDL_X11_XCURSOR=OFF \
	-DSDL_X11_XDBE=OFF \
	-DSDL_X11_XFIXES=OFF \
	-DSDL_X11_XINPUT=OFF \
	-DSDL_X11_XRANDR=OFF \
	-DSDL_X11_XSCRNSAVER=OFF \
	-DSDL_X11_XSHAPE=OFF \
	-DSDL_XINPUT=OFF

define Package/libsdl2
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Simple DirectMedia Layer
  URL:=https://www.libsdl.org
  DEPENDS:=+libstdcpp +alsa-lib +libudev-zero +pulseaudio +libmesa +libsamplerate +libwayland +wayland-protocols
endef

define Package/libsdl2/description
 Simple DirectMedia Layer is a cross-platform development library designed to
 provide low level access to audio, keyboard, mouse, joystick, and graphics
 hardware via OpenGL and Direct3D.
endef

define Build/InstallDev
	$(call Build/InstallDev/cmake,$(1))
ifdef CONFIG_DEBUG
	$(LN) libSDL2d.so $(1)/usr/lib/libSDL2.so
endif
endef

define Package/libsdl2/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libSDL2*.so* $(1)/usr/lib
ifdef CONFIG_DEBUG
	$(LN) libSDL2d.so $(1)/usr/lib/libSDL2.so
endif
endef

define Package/libsdl2-tests
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=Video
  TITLE:=SDL installed tests
  URL:=https://www.libsdl.org
  DEPENDS:=+libsdl2
endef

define Package/libsdl2-tests/description
 Various test programs to validate SDL is working correctly, and some benchmarks.
endef

define Package/libsdl2-tests/install
	$(INSTALL_DIR) $(1)/usr/libexec/installed-tests
	$(CP) $(PKG_INSTALL_DIR)/usr/libexec/installed-tests/* $(1)/usr/libexec/installed-tests
endef

$(eval $(call BuildPackage,libsdl2))
$(eval $(call BuildPackage,libsdl2-tests))
