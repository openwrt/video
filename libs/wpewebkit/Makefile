include $(TOPDIR)/rules.mk

PKG_NAME:=wpewebkit
PKG_VERSION:=2.48.3
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://wpewebkit.org/releases
PKG_HASH:=807571f07e87823b8fb79564692c9b1ef81ee62edbf51345a15bd0e7e1f2e07b

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=LGPL-2.1-or-later BSD-2-Clause
PKG_LICENSE_FILES:=Source/WebCore/LICENSE-Apple Source/Webcore/LICENSE-LGPL-2.1

PKG_BUILD_DEPENDS:=unifdef/host ruby/host
PKG_CONFIG_DEPENDS:=CONFIG_PACKAGE_wpewebkit-minibrowser
CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_BINARY_SUBDIR:=openwrt-build

CMAKE_OPTIONS += \
	-DBWRAP_EXECUTABLE=/usr/bin/bwrap \
	-DDBUS_PROXY_EXECUTABLE=/usr/bin/xdg-dbus-proxy \
	-DGLIB_COMPILE_RESOURCES_EXECUTABLE="$(STAGING_DIR_HOSTPKG)/bin/glib-compile-resources" \
	-DENABLE_ACCESSIBILITY=OFF \
	-DENABLE_API_TESTS=OFF \
	-DENABLE_BUBBLEWRAP_SANDBOX=ON \
	-DENABLE_INTROSPECTION=OFF \
	-DENABLE_DOCUMENTATION=OFF \
	-DENABLE_WEB_CRYPTO=ON \
	-DENABLE_JOURNALD_LOG=OFF \
	-DENABLE_MINIBROWSER=$(if $(CONFIG_PACKAGE_wpewebkit-minibrowser),ON,OFF) \
	-DENABLE_WPE_PLATFORM=ON \
	-DENABLE_WPE_PLATFORM_WAYLAND=ON \
	-DENABLE_WPE_PLATFORM_DRM=ON \
	-DENABLE_SPELLCHECK=OFF \
	-DENABLE_SPEECH_SYNTHESIS=OFF \
	-DENABLE_TOUCH_EVENTS=ON \
	-DENABLE_JIT=$(if $(CONFIG_arm_v7)$(CONFIG_aarch64)$(CONFIG_riscv64)$(CONFIG_x86_64),ON,OFF) \
	-DENABLE_C_LOOP=$(if $(CONFIG_arm_v7)$(CONFIG_aarch64)$(CONFIG_riscv64)$(CONFIG_x86_64),OFF,ON) \
	-DENABLE_WEBGL=ON \
	-DENABLE_VIDEO=ON \
	-DPORT=WPE \
	-DUSE_ATK=OFF \
	-DUSE_LIBBACKTRACE=OFF \
	-DUSE_LIBHYPHEN=OFF \
	-DUSE_LIBSECRET=OFF \
	-DUSE_SOUP2=OFF \
	-DUSE_JPEGXL=OFF \
	-DUSE_WOFF2=OFF \
	-DUSE_AVIF=OFF \
	-DUSE_LCMS=OFF \
	-DUSE_SYSTEM_SYSPROF_CAPTURE=OFF

define Package/libwpewebkit
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=WPEWebKit Library
  URL:=https://wpewebkit.org
  DEPENDS:=+bubblewrap +glib2 +gst1-mod-opengl +harfbuzz +icu-full-data \
	   +libepoxy +libgcrypt +libgst1allocators +libgst1app +libgst1audio \
	   +libgst1fft +libgst1gl +libgst1pbutils +libgst1tag \
	   +libgst1transcoder +libgst1video +libinput +libjpeg +libpng \
	   +libseccomp +libsoup3 +libsqlite3 +libtasn1 +libudev +libwayland \
	   +libwebp +libwpe +libxml2 +libxslt +zlib \
	   @(arm_v7||i386||i686||ARCH_64BIT)
endef

define Package/libwpewebkit/description
 WebKitGTK is a full-featured port of the WebKit rendering engine,
 suitable for projects requiring any kind of web integration, from
 hybrid HTML/CSS applications to full-fledged web browsers.
endef

define Package/wpewebkit-driver
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=WebDriver service implementation for WPE WebKit
  URL:=https://wpewebkit.org
  DEPENDS:=+libwpewebkit
endef

define Package/wpewebkit-driver/description
 WebDriver is an automation API to control a web browser. It allows the
 user to create automated tests for web applications independently of the
 browser and platform.

 The WebKitWebDriver binary provides the WebDriver service implementation
 for WebKitGTK. It works as a server, processing client requests to spawn
 and control the web browser. This driver is not tied to any specific
 browser and can be used with any WebKitGTK-based browser, but it uses
 MiniBrowser as the default.
endef

define Package/wpewebkit-minibrowser
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=WPE WebKit Mini Browser
  URL:=https://wpewebkit.org/
  DEPENDS:=+libwpewebkit +wpebackend-fdo
endef

define Package/wpewebkit-minibrowser/description
 Minimalistic Web browser based on WPE WebKit.
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/wpe-webkit-2.0/jsc \
		       $(1)/usr/include/wpe-webkit-2.0/wpe \
		       $(1)/usr/include/wpe-webkit-2.0/wpe-platform/wpe/drm \
		       $(1)/usr/include/wpe-webkit-2.0/wpe-platform/wpe/headless \
		       $(1)/usr/include/wpe-webkit-2.0/wpe-platform/wpe/wayland \
		       $(1)/usr/include/wpe-webkit-2.0/wpe-platform/wpe \
		       $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/include/wpe-webkit-2.0/jsc/*.h \
		       $(1)/usr/include/wpe-webkit-2.0/jsc
	$(CP) $(PKG_INSTALL_DIR)/usr/include/wpe-webkit-2.0/wpe/*.h \
		       $(1)/usr/include/wpe-webkit-2.0/wpe
	$(CP) $(PKG_INSTALL_DIR)/usr/include/wpe-webkit-2.0/wpe-platform/wpe/*.h \
		       $(1)/usr/include/wpe-webkit-2.0/wpe-platform/wpe
	$(CP) $(PKG_INSTALL_DIR)/usr/include/wpe-webkit-2.0/wpe-platform/wpe/drm/*.h \
		       $(1)/usr/include/wpe-webkit-2.0/wpe-platform/wpe/drm
	$(CP) $(PKG_INSTALL_DIR)/usr/include/wpe-webkit-2.0/wpe-platform/wpe/headless/*.h \
		       $(1)/usr/include/wpe-webkit-2.0/wpe-platform/wpe/headless
	$(CP) $(PKG_INSTALL_DIR)/usr/include/wpe-webkit-2.0/wpe-platform/wpe/wayland/*.h \
		       $(1)/usr/include/wpe-webkit-2.0/wpe-platform/wpe/wayland
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc \
		       $(1)/usr/lib/pkgconfig
endef

define Package/libwpewebkit/install
	$(INSTALL_DIR) $(1)/usr/lib/wpe-webkit-2.0/injected-bundle
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/wpe-webkit-2.0/injected-bundle/*.so \
		$(1)/usr/lib/wpe-webkit-2.0/injected-bundle
	$(INSTALL_DIR) $(1)/usr/libexec/wpe-webkit-2.0
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/libexec/wpe-webkit-2.0/WPEGPUProcess \
		$(1)/usr/libexec/wpe-webkit-2.0
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/libexec/wpe-webkit-2.0/WPENetworkProcess \
		$(1)/usr/libexec/wpe-webkit-2.0
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/libexec/wpe-webkit-2.0/WPEWebProcess \
		$(1)/usr/libexec/wpe-webkit-2.0
	$(INSTALL_DIR) $(1)/usr/share/wpe-webkit-2.0
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/wpe-webkit-2.0/inspector.gresource \
		$(1)/usr/share/wpe-webkit-2.0
endef

define Package/wpewebkit-driver/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/WPEWebDriver $(1)/usr/bin
endef

define Package/wpewebkit-minibrowser/install
	$(INSTALL_DIR) $(1)/usr/libexec/wpe-webkit-2.0
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/libexec/wpe-webkit-2.0/MiniBrowser \
		$(1)/usr/libexec/wpe-webkit-2.0
endef

$(eval $(call BuildPackage,libwpewebkit))
$(eval $(call BuildPackage,wpewebkit-driver))
$(eval $(call BuildPackage,wpewebkit-minibrowser))
