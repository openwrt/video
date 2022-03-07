include $(TOPDIR)/rules.mk

PKG_NAME:=wayland
PKG_VERSION:=1.20.0
PKG_RELEASE:=$(AUTORELEASE)

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://wayland.freedesktop.org/releases
PKG_HASH:=b8a034154c7059772e0fdbd27dbfcda6c732df29cae56a82274f6ec5d7cd8725

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING

PKG_INSTALL:=1
PKG_BUILD_DEPENDS:=libffi/host libxml2/host wayland/host
HOST_BUILD_DEPENDS:=$(PKG_BUILD_DEPENDS)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/meson.mk

define Package/libwayland
  SECTION:=libs
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=wayland
  URL:=https://wayland.freedesktop.org/
  DEPENDS:=+libexpat +libffi
endef

define Package/libwayland/description
endef

define Package/wayland-scanner
  SECTION:=libs
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=wayland
  URL:=https://wayland.freedesktop.org/
  DEPENDS:=+libxml2 +libexpat
endef

define Package/wayland-scanner/description
endef

MESON_HOST_ARGS += \
	-Dscanner=true \
	-Dlibraries=false \
	-Ddocumentation=false \
	-Ddtd_validation=true

HOST_LDFLAGS += $(STAGING_DIR_HOST)/lib/libz.a -lm

MESON_ARGS += \
	-Dscanner=true \
	-Dlibraries=true \
	-Ddocumentation=false \
	-Ddtd_validation=true \
	-Dscanner_bin="$(STAGING_DIR_HOSTPKG)/bin/wayland-scanner"

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

define Package/libwayland/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so.* $(1)/usr/lib/
endef

define Package/wayland-scanner/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/wayland-scanner $(1)/usr/bin/
endef

$(eval $(call BuildPackage,libwayland))
$(eval $(call BuildPackage,wayland-scanner))
$(eval $(call HostBuild))
