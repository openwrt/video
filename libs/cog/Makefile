include $(TOPDIR)/rules.mk

PKG_NAME:=cog
PKG_VERSION:=0.18.5
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://wpewebkit.org/releases
PKG_HASH:=0ede9d09ab635ac519beec0543378e3fc51b56561a5fb7aa9c0cbca54c31b97c

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-2-clause
PKG_LICENSE_FILES:=COPYING

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/cog
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=launcher for WPE WebKit
  URL:=https://wpewebkit.org/
  DEPENDS:=+libcogcore
  USERID:=cog:video=40
endef

define Package/cog/description/default
Cog is a small single "window" launcher for the WebKit WPE port.
It is small, provides no user interface, and is suitable to be used as a Web
application container. The "window" may be fullscreen depending on the WPE
backend being used.
endef

define Package/libcogcore
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=WPE WebKit API library
  URL:=https://wpewebkit.org/
  DEPENDS:=+libdbus +libmanette +libwpewebkit +wpebackend-fdo
endef

define Package/libcogcore/description/default
libcogcore is a library with ready-to-use components typically needed
for implementing applications which use the WPE WebKit API.
endef

MESON_ARGS += \
	-Dcog_dbus_control=system \
	-Dcog_dbus_system_owner=cog \
	-Ddocumentation=false \
	-Dmanpages=false

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/cog
	$(CP) $(PKG_INSTALL_DIR)/usr/include/cog/*.h $(1)/usr/include/cog
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

define Package/libcogcore/install
	$(INSTALL_DIR) $(1)/usr/lib/cog/modules
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/cog/modules/* $(1)/usr/lib/cog/modules
endef

define Package/cog/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/share/dbus-1/system.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/dbus-1/system.d/com.igalia.Cog.conf $(1)/usr/share/dbus-1/system.d
endef

$(eval $(call BuildPackage,libcogcore))
$(eval $(call BuildPackage,cog))
