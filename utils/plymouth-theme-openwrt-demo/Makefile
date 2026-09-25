include $(TOPDIR)/rules.mk

PKG_NAME:=plymouth-theme-openwrt-demo
PKG_VERSION:=1
PKG_RELEASE:=1

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-or-later

include $(INCLUDE_DIR)/package.mk

define Package/plymouth-theme-openwrt-demo
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=OpenWrt boot splash demo theme
  URL:=https://openwrt.org/
  DEPENDS:=+plymouth-plugin-ucode +ucode-mod-plutovg +ucode-mod-plutosvg \
	   +ucode-mod-fs
endef

define Package/plymouth-theme-openwrt-demo/description
  The OpenWrt logo held still above a mesh network that is generated
  afresh on every boot and carries traffic while the system starts. The
  whole theme is a ucode program and an SVG, with no rendered frames.

  OpenWrt is a registered trademark owned by Software Freedom Conservancy
  (SFC), used under the trademark policy at https://openwrt.org/trademark.
  The logo artwork is all rights reserved and is not covered by this
  package's licence.
endef

define Build/Prepare
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/plymouth-theme-openwrt-demo/install
	$(INSTALL_DIR) $(1)/usr/share/plymouth/themes/openwrt-demo
	$(INSTALL_DATA) ./files/openwrt.plymouth \
		$(1)/usr/share/plymouth/themes/openwrt-demo/
	$(INSTALL_DATA) ./files/openwrt.uc \
		$(1)/usr/share/plymouth/themes/openwrt-demo/
	$(INSTALL_DATA) ./files/logo.svg \
		$(1)/usr/share/plymouth/themes/openwrt-demo/
	$(INSTALL_DIR) $(1)/etc/hotplug.d/ntp
	$(INSTALL_DATA) ./files/hotplug-ntp.sh \
		$(1)/etc/hotplug.d/ntp/00-plymouth
	$(INSTALL_DIR) $(1)/usr/share/plymouth/themes/openwrt-demo/pictograms
	$(INSTALL_DATA) ./files/pictograms/*.svg \
		$(1)/usr/share/plymouth/themes/openwrt-demo/pictograms/
endef

define Package/plymouth-theme-openwrt-demo/postinst
#!/bin/sh
conf="$${IPKG_INSTROOT}/etc/plymouth/plymouthd.conf"

[ -f "$$conf" ] || exit 0

if grep -q '^Theme=' "$$conf"; then
	sed -i 's/^Theme=.*/Theme=openwrt-demo/' "$$conf"
else
	echo "Theme=openwrt-demo" >> "$$conf"
fi

exit 0
endef

$(eval $(call BuildPackage,plymouth-theme-openwrt-demo))
