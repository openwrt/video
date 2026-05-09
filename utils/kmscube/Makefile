include $(TOPDIR)/rules.mk

PKG_NAME:=kmscube
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_DATE:=2025-10-24
PKG_SOURCE_VERSION:=f60e50e887d3c49e91ac9b06d8199b36152632fa
PKG_SOURCE_URL:=https://gitlab.freedesktop.org/mesa/kmscube
PKG_MIRROR_HASH:=480a9ee8d761be990b2296317562d4f1e2be84acf43a19240aaaf1df09039773

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/kmscube
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=Video
  DEPENDS:=+glib2 +libmesa +libpng +gstreamer1-plugins-base +libgstreamer1 +libgst1allocators +libgst1video +libgst1app +libgst1gl
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
