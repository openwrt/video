include $(TOPDIR)/rules.mk

PKG_NAME:=wayland
PKG_VERSION:=1.23.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://gitlab.freedesktop.org/wayland/wayland/-/releases/$(PKG_VERSION)/downloads/
PKG_HASH:=05b3e1574d3e67626b5974f862f36b5b427c7ceeb965cb36a4e6c2d342e45ab2

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
  USERID:=:video=40
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
	-Dtests=false \
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
	$(SED) 's/^wayland_scanner=$$$${bindir}\/\(.*\)/wayland_scanner=$$$${prefix_hostpkg}\/bin\/\1/' $(1)/usr/lib/pkgconfig/wayland-scanner.pc
endef

define Package/libwayland/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so.* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/etc/profile.d
	$(INSTALL_DATA) ./files/xdg-runtime-dir.profile $(1)/etc/profile.d/xdr-runtime-dir.sh
	$(INSTALL_DIR) $(1)/etc/hotplug.d/drm
	$(INSTALL_BIN) ./files/video.hotplug $(1)/etc/hotplug.d/drm/wayland
endef

define Package/wayland-scanner/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/wayland-scanner $(1)/usr/bin/
endef

$(eval $(call BuildPackage,libwayland))
$(eval $(call BuildPackage,wayland-scanner))
$(eval $(call HostBuild))
