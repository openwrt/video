include $(TOPDIR)/rules.mk

PKG_NAME:=weston
PKG_RELEASE:=1

PKG_VERSION:=16.0.0
PKG_MAJOR_VERSION:=$(firstword $(subst .,$(space),$(PKG_VERSION)))
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://gitlab.freedesktop.org/wayland/$(PKG_NAME)/-/releases/$(PKG_VERSION)/downloads/
PKG_HASH:=dfb32e2bccabda957b94a8d0ec6075acd18c71c87ebc543ee3e618d294ca0f7f

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING

PKG_INSTALL:=1
PKG_BUILD_DEPENDS:=wayland/host

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/weston
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=weston
  URL:=https://wayland.freedesktop.org/
  DEPENDS:=+gstreamer1-plugins-base +libcairo +libevdev +libdisplay-info \
	+libgst1allocators +libgst1app +libgst1video +libgstreamer1 +libinput \
	+libmesa +libpango +libseat +libwayland +libwebp +libxkbcommon +pixman \
	+seatd +vulkan-loader +wayland-protocols +xkeyboard-config
endef

define Package/weston/description
endef

MESON_ARGS += \
	-Dbackend-drm=true \
	-Dbackend-headless=false \
	-Dbackend-pipewire=false \
	-Dbackend-rdp=false \
	-Dbackend-vnc=false \
	-Dbackend-x11=false \
	-Dbackend-wayland=true \
	-Dxwayland=false \
	-Dcolor-management-lcms=false \
	-Dsystemd=false \
	-Ddeprecated-pipewire=false \
	-Dtest-junit-xml=false \
	-Ddemo-clients=true \
	-Drenderer-gl=true \
	-Dshell-desktop=true \
	-Dshell-ivi=true \
	-Dshell-kiosk=true \
	-Dshell-lua=false \
	-Ddeprecated-remoting=false \
	-Dimage-jpeg=true \
	-Dimage-webp=true \
	-Dtools=calibrator,debug,info,terminal,touch-calibrator \
	-Dtests=false \
	-Ddoc=false

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

define Package/weston/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/libweston-$(PKG_MAJOR_VERSION)
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/libweston-$(PKG_MAJOR_VERSION)/*.so* \
		$(1)/usr/lib/libweston-$(PKG_MAJOR_VERSION)
	$(INSTALL_DIR) $(1)/usr/lib/weston
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/weston/*.so* $(1)/usr/lib/weston
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/libexec
	$(CP) $(PKG_INSTALL_DIR)/usr/libexec/* $(1)/usr/libexec
	$(INSTALL_DIR) $(1)/usr/share/libweston-$(PKG_MAJOR_VERSION)/protocols
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/share/libweston-$(PKG_MAJOR_VERSION)/protocols/* \
		$(1)/usr/share/libweston-$(PKG_MAJOR_VERSION)/protocols
	$(INSTALL_DIR) $(1)/usr/share/weston
	$(CP) $(PKG_INSTALL_DIR)/usr/share/weston/* $(1)/usr/share/weston
endef

$(eval $(call BuildPackage,weston))
