#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=webkit-contentfilter
PKG_VERSION:=1
PKG_RELEASE:=1

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-only

include $(INCLUDE_DIR)/package.mk

define Package/webkit-contentfilter
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Adblock filter lists for WebKit content blockers
  DEPENDS:=+libwpewebkit +ucode +ucode-mod-fs +ucode-mod-uci \
	+ucode-mod-uloop +ucode-mod-uclient +ca-bundle
endef

define Package/webkit-contentfilter/description
  Fetches Adblock Plus filter lists such as EasyList and EasyPrivacy from
  the URLs configured in /etc/config/webkit-contentfilter, converts them to
  WebKit content blocker rules and compiles them into a WebKit content
  filter store at /var/run/webkit-contentfilter. WPE applications load a
  list from there by its UCI section name with
  webkit_user_content_filter_store_load(), without compiling it again.

  Network rules and CSS element hiding are converted; scriptlets, regular
  expression rules and extended cosmetic filters have no WebKit equivalent
  and are left out. The lists are fetched once the clock is set by NTP and
  again after each day of uptime. Compiled lists stay in RAM unless a
  storage path is configured; lists kept there are published at boot,
  before the clock is set.
endef

define Package/webkit-contentfilter/conffiles
/etc/config/webkit-contentfilter
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CPPFLAGS) $(TARGET_CFLAGS) \
		$$$$($(PKG_CONFIG) --cflags wpe-webkit-2.0) \
		-o $(PKG_BUILD_DIR)/webkit-contentfilter-compile \
		$(PKG_BUILD_DIR)/compile.c $(TARGET_LDFLAGS) \
		$$$$($(PKG_CONFIG) --libs wpe-webkit-2.0)
endef

define Package/webkit-contentfilter/install
	$(INSTALL_DIR) $(1)/usr/sbin $(1)/usr/libexec
	$(INSTALL_BIN) ./files/webkit-contentfilter.uc $(1)/usr/sbin/webkit-contentfilter
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/webkit-contentfilter-compile $(1)/usr/libexec/
	$(INSTALL_DIR) $(1)/usr/share/ucode/webkit-contentfilter
	$(INSTALL_DATA) ./files/convert.uc $(1)/usr/share/ucode/webkit-contentfilter/convert.uc
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/webkit-contentfilter.init $(1)/etc/init.d/webkit-contentfilter
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/webkit-contentfilter.config $(1)/etc/config/webkit-contentfilter
	$(INSTALL_DIR) $(1)/etc/hotplug.d/ntp
	$(INSTALL_DATA) ./files/webkit-contentfilter.hotplug $(1)/etc/hotplug.d/ntp/50-webkit-contentfilter
endef

$(eval $(call BuildPackage,webkit-contentfilter))
