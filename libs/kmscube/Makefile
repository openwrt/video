include $(TOPDIR)/rules.mk

PKG_NAME:=kmscube
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_DATE:=2024-09-26
PKG_SOURCE_VERSION:=b2f97f53e01e07a920761e5d3e3ba22898b794a4
PKG_SOURCE_URL:=https://gitlab.freedesktop.org/mesa/kmscube
PKG_MIRROR_HASH:=ea21648c9b012d53653136a2490635331507a8f2873f9a66a201a587f87c9fb6

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/kmscube
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  DEPENDS:=+glib2 +libmesa +libpng +gstreamer1-plugins-base +libgstreamer1 +libgst1allocators +libgst1video +libgst1app
  TITLE:=kmscube KMS/DRM/EGL example
  URL:=https://www.mesa3d.org
endef

define Package/kmscube/description
endef

define Package/kmscube/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin/
endef

$(eval $(call BuildPackage,kmscube))
