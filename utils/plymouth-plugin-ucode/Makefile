include $(TOPDIR)/rules.mk

PKG_NAME:=plymouth-plugin-ucode
PKG_VERSION:=1
PKG_RELEASE:=1

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-or-later

include $(INCLUDE_DIR)/package.mk

define Package/plymouth-plugin-ucode
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=ucode-driven plymouth splash plugin
  URL:=https://www.freedesktop.org/wiki/Software/Plymouth/
  DEPENDS:=+plymouth +libucode +plutovg +ucode-mod-plutovg +libubus \
	   +libubox +libblobmsg-json
endef

define Package/plymouth-plugin-ucode/description
  A plymouth splash plugin that hands the screen to a ucode script. The
  script draws with plutovg and decides what changed, so a theme is a
  program rather than a sequence of rendered frames, and it can react to
  ubus events while the system boots.
endef

UCODE_SONAME:=$(notdir $(firstword $(wildcard $(STAGING_DIR)/usr/lib/libucode.so.*)))

TARGET_CFLAGS += \
	-DUCODE_SONAME=\"$(UCODE_SONAME)\" \
	-I$(STAGING_DIR)/usr/include/plymouth-1/ply \
	-I$(STAGING_DIR)/usr/include/plymouth-1/ply-splash-core \
	-std=gnu99 -Wall -Wextra -Wno-unused-parameter

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS) $(FPIC) \
		-ffunction-sections -fdata-sections -Wl,--gc-sections \
		-shared -o $(PKG_BUILD_DIR)/ucode.so \
		$(PKG_BUILD_DIR)/ucode-splash.c \
		-lply -lply-splash-core -lucode -lplutovg \
		-lubus -lubox -lblobmsg_json
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Package/plymouth-plugin-ucode/install
	$(INSTALL_DIR) $(1)/usr/lib/plymouth
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ucode.so $(1)/usr/lib/plymouth/
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/plymouth-theme-bench.uc \
		$(1)/usr/bin/plymouth-theme-bench
	$(SED) 's|@VERSION@|$(PKG_VERSION)-r$(PKG_RELEASE)|' \
		$(1)/usr/bin/plymouth-theme-bench
	$(INSTALL_DIR) $(1)/lib/upgrade
	$(INSTALL_DATA) ./files/upgrade-plymouth.sh \
		$(1)/lib/upgrade/plymouth.sh
	$(INSTALL_DIR) $(1)/lib/preinit
	$(INSTALL_DATA) ./files/preinit-plymouth.sh \
		$(1)/lib/preinit/03_plymouth
endef

$(eval $(call BuildPackage,plymouth-plugin-ucode))
