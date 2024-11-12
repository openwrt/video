include $(TOPDIR)/rules.mk

PKG_NAME:=cage
PKG_VERSION:=0.2.0

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/cage-kiosk/cage/releases/download/v0.2.0
PKG_HASH:=557c194d18af7202a9ec2e8be6dd7129f6c16d0f4528f4079ba26ccd57b6ef88

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE

PKG_INSTALL:=1
PKG_BUILD_DEPENDS:=wayland/host

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/cage
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=a Wayland kiosk
  URL:=https://wayland.freedesktop.org/
  DEPENDS:=+wlroots +xkeyboard-config
endef

define Package/cage/description
This is Cage, a Wayland kiosk. A kiosk runs a single, maximized application.
endef

MESON_ARGS += \
	-Dman-pages=disabled

define Package/cage/conffiles
/etc/config/cage
endef

define Package/cage/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/cage $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/cage.init $(1)/etc/init.d/cage
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/cage.config $(1)/etc/config/cage
endef

$(eval $(call BuildPackage,cage))
