include $(TOPDIR)/rules.mk

PKG_NAME:=wayland-utils
PKG_VERSION:=1.2.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://gitlab.freedesktop.org/wayland/wayland-utils/-/releases/$(PKG_VERSION)/downloads/
PKG_HASH:=d9278c22554586881802540751bcc42569262bf80cd9ac9b0fd12ff4bd09a9e4

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING

PKG_INSTALL:=1
PKG_BUILD_DEPENDS:=wayland/host

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/wayland-info
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=wayland-utils
  URL:=https://wayland.freedesktop.org/
  DEPENDS:=+libwayland +wayland-protocols +libdrm
endef

define Package/wayland-info/description
endef

define Package/wayland-info/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/wayland-info $(1)/usr/bin
endef

$(eval $(call BuildPackage,wayland-info))
