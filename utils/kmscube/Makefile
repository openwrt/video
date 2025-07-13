include $(TOPDIR)/rules.mk

PKG_NAME:=kmscube
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_DATE:=2024-11-06
PKG_SOURCE_VERSION:=311eaaaa473d593c30d118799aa19ac4ad53cd65
PKG_SOURCE_URL:=https://gitlab.freedesktop.org/mesa/kmscube
PKG_MIRROR_HASH:=23b4284c0665448e30b5dfae8f0e8b75b1fb512d47f821a87b66b5934c6060e6

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/kmscube
  SECTION:=utils
  CATEGORY:=Utilities
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
