include $(TOPDIR)/rules.mk

PKG_NAME:=libtsm
PKG_VERSION:=4.7.1
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/kmscon/libtsm/tar.gz/v$(PKG_VERSION)?
PKG_HASH:=40d7f11698f0ce87af8da67a3142ee9f02e2b5d914f8ad83aa080e052fe1ae2b

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

MESON_ARGS += \
	-Dtests=false \
	-Dgtktsm=false

define Package/libtsm
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Terminal emulator state machine
  URL:=https://github.com/kmscon/libtsm
  DEPENDS:=+libxkbcommon
endef

define Package/libtsm/description
  State machine for a VT220 terminal emulator, without any rendering or
  input handling of its own. Used by kmscon to run a console on a KMS
  device.
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libtsm.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig/
endef

define Package/libtsm/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libtsm.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libtsm))
