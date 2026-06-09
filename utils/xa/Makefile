include $(TOPDIR)/rules.mk

PKG_NAME:=xa
PKG_VERSION:=2.4.1
PKG_RELEASE:=1

PKG_SOURCE:=xa-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://www.floodgap.com/retrotech/xa/dists/
PKG_HASH:=63c12a6a32a8e364f34f049d8b2477f4656021418f08b8d6b462be0ed3be3ac3

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-only
PKG_LICENSE_FILES:=COPYING

PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk

define Package/xa
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=xa65 6502/65816 cross-assembler
  URL:=https://www.floodgap.com/retrotech/xa/
endef

define Package/xa/description
  xa (xa65) is a high-speed, two-pass portable cross-assembler that
  generates code for the NMOS 6502, CMOS 6502 (65C02 and Rockwell R65C02)
  and the 65816 processors. The package also ships the companion tools
  file65, ldo65, reloc65, printcbm and uncpk.
endef

XA_BINS:=xa file65 ldo65 reloc65 printcbm uncpk

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src \
		CC="$(TARGET_CC)" \
		LD="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)"
	$(MAKE) -C $(PKG_BUILD_DIR)/misc \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS)"
endef

define Package/xa/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(foreach bin,$(XA_BINS),$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(bin) $(1)/usr/bin/;)
endef

define Host/Compile
	$(MAKE) -C $(HOST_BUILD_DIR)/src \
		CC="$(HOSTCC)" \
		LD="$(HOSTCC)" \
		CFLAGS="$(HOST_CFLAGS)"
endef

define Host/Install
	$(INSTALL_DIR) $(STAGING_DIR_HOSTPKG)/bin
	$(INSTALL_BIN) $(HOST_BUILD_DIR)/xa $(STAGING_DIR_HOSTPKG)/bin/
endef

define Host/Clean
	$(RM) $(STAGING_DIR_HOSTPKG)/bin/xa
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,xa))
