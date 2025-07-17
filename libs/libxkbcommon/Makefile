include $(TOPDIR)/rules.mk

PKG_NAME:=libxkbcommon
PKG_VERSION:=1.10.0
PKG_RELEASE:=2

PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=xkbcommon-$(PKG_VERSION)
PKG_SOURCE_URL:=https://github.com/xkbcommon/$(PKG_NAME).git
PKG_MIRROR_HASH:=f0a0582a82c1aadef3c92342c65d233aca9aa37444d31a1d9c64c0150e86c82e

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-3-Clause

PKG_INSTALL:=1

PKG_BUILD_DEPENDS:=wayland

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

MESON_ARGS += \
	-Denable-x11=false \
	-Denable-docs=false \
	-Denable-wayland=true

define Package/libxkbcommon
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=libxkbcommon
  URL:=https://xkbcommon.org/
  DEPENDS:=+libwayland +libxml2 +wayland-protocols
endef

define Package/libxkbcommon/description
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig/
endef

define Package/libxkbcommon/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/libexec/xkbcommon
	$(CP) $(PKG_INSTALL_DIR)/usr/libexec/xkbcommon/* $(1)/usr/libexec/xkbcommon/
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/xkbcli $(1)/usr/bin/
endef

$(eval $(call BuildPackage,libxkbcommon))
