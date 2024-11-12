include $(TOPDIR)/rules.mk


PKG_NAME:=wlroots
PKG_VERSION:=0.18.1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://gitlab.freedesktop.org/$(PKG_NAME)/$(PKG_NAME)/-/archive/$(PKG_VERSION)
PKG_HASH:=10084bba57bf4162cd81eb79c2a0bee0d02c7ccd747eac0a7021b45b10bb821c

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE

PKG_INSTALL:=1
PKG_BUILD_DEPENDS:=wayland/host

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/wlroots
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=modules for building a Wayland compositor
  URL:=https://gitlab.freedesktop.org/wlroots/wlroots
  DEPENDS:=+libmesa +libdisplay-info +libdrm +libinput +libseat +libxkbcommon \
	   +libudev +pixman +libwayland +xkeyboard-config
endef

define Package/wlroots/description
Pluggable, composable, unopinionated modules for building a Wayland compositor;
or about 60,000 lines of code you were going to write anyway.
endef

MESON_ARGS += \
	-Dxcb-errors=disabled \
	-Dxwayland=disabled \
	-Dexamples=false \
	-Drenderers=gles2 \
	-Dbackends=drm,libinput \
	-Dallocators=gbm \
	-Dsession=enabled \
	-Dcolor-management=disabled \
	-Dlibliftoff=disabled

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

define Package/wlroots/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,wlroots))
