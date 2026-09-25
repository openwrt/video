include $(TOPDIR)/rules.mk

PKG_NAME:=plymouth
PKG_VERSION:=26.134.222
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://gitlab.freedesktop.org/plymouth/plymouth/-/archive/$(PKG_VERSION)
PKG_HASH:=053687d4e8bb1320c14dafc014ebf4e45427f1cf417ce76b51102fbd61a3350a

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=COPYING

PKG_BUILD_DEPENDS:=gettext-full/host

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

MESON_ARGS += \
	-Ddrm=true \
	-Dudev=enabled \
	-Dgtk=disabled \
	-Dpango=disabled \
	-Dfreetype=disabled \
	-Ddocs=false \
	-Dsystemd-integration=false \
	-Dupstart-monitoring=false \
	-Dtracing=$(if $(CONFIG_DEBUG),true,false) \
	-Drelease-file=/etc/openwrt_release \
	-Drunstatedir=/var/run

define Package/plymouth/Default
  SECTION:=utils
  CATEGORY:=Utilities
  URL:=https://www.freedesktop.org/wiki/Software/Plymouth/
endef

define Package/plymouth
$(call Package/plymouth/Default)
  TITLE:=Graphical boot splash on a KMS device
  DEPENDS:=+libpng +libdrm +libudev +libevdev +libxkbcommon +xkeyboard-config
endef

define Package/plymouth/description
  Draws a boot splash directly on a DRM/KMS device and owns the display
  until the application that follows it takes over, so the console stays
  silent from reset to user interface.
endef

define Package/plymouth/conffiles
/etc/plymouth/plymouthd.conf
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/plymouth-1 $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libply*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig/
endef

define Package/plymouth/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libply*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/plymouth
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/plymouth/* $(1)/usr/lib/plymouth/
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/plymouth $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/plymouthd $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/share/plymouth
	$(CP) $(PKG_INSTALL_DIR)/usr/share/plymouth/plymouthd.defaults \
		$(1)/usr/share/plymouth/
	$(INSTALL_DIR) $(1)/etc/plymouth
	$(INSTALL_DATA) ./files/plymouthd.conf $(1)/etc/plymouth/plymouthd.conf
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/plymouth.init $(1)/etc/init.d/plymouth
endef

$(eval $(call BuildPackage,plymouth))
