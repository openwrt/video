include $(TOPDIR)/rules.mk

PKG_NAME:=edid-decode
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_DATE:=2024-10-12
PKG_SOURCE_VERSION:=8fd97f32c8f25b6560a5766066218a72ad2e2a01
PKG_SOURCE_URL:=https://git.linuxtv.org/edid-decode.git
PKG_MIRROR_HASH:=b850202de0b0a5ad2e703d45c8a683ffefcbd8e9e09bd64caebd891f5710681e

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/edid-decode
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=EDID decode
  DEPENDS:=+libstdcpp
  URL:=https://www.linuxtv.org
endef

define Package/edid-decode/description
 Display EDID decoder application
endef

define Package/edid-decode/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin/
endef

$(eval $(call BuildPackage,edid-decode))
