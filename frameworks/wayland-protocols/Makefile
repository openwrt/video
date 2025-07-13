include $(TOPDIR)/rules.mk

PKG_NAME:=wayland-protocols
PKG_VERSION:=1.45
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://gitlab.freedesktop.org/wayland/wayland-protocols/-/releases/$(PKG_VERSION)/downloads/
PKG_HASH:=4d2b2a9e3e099d017dc8107bf1c334d27bb87d9e4aff19a0c8d856d17cd41ef0

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING

PKG_INSTALL:=1

PKG_BUILD_DEPENDS:=wayland
HOST_BUILD_DEPENDS:=wayland/host

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

MESON_ARGS += \
	-Dtests=false

define Package/wayland-protocols
  SECTION:=libs
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=wayland-protocols
  URL:=https://wayland.freedesktop.org/
  PKGARCH:=all
endef

define Package/wayland-protocols/description
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/share/wayland-protocols
	$(CP) $(PKG_INSTALL_DIR)/usr/share/wayland-protocols/* $(1)/usr/share/wayland-protocols
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/share/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

define Package/wayland-protocols/install
	$(INSTALL_DIR) $(1)/usr/share/wayland-protocols
	$(CP) $(PKG_INSTALL_DIR)/usr/share/wayland-protocols/* $(1)/usr/share/wayland-protocols
endef

$(eval $(call BuildPackage,wayland-protocols))
