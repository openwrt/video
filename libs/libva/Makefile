include $(TOPDIR)/rules.mk

PKG_NAME:=libva
PKG_VERSION:=2.22.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/intel/$(PKG_NAME)/tar.gz/$(PKG_VERSION)?
PKG_HASH:=467c418c2640a178c6baad5be2e00d569842123763b80507721ab87eb7af8735

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/libva
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=Video Acceleration API
  URL:=https://intel.github.io/libva/
  DEPENDS:=+libdrm +libwayland
endef

define Package/libva/description
Libva is an implementation for VA-API (Video Acceleration API)

VA-API is an open-source library and API specification, which provides access
to graphics hardware acceleration capabilities for video processing.
It consists of a main library and driver-specific acceleration backends for
each supported hardware vendor.
endef

MESON_ARGS += \
	-Ddisable_drm=false \
	-Dwith_x11=no \
	-Dwith_glx=no \
	-Dwith_wayland=yes \
	-Dwith_win32=no \
	-Dwith_legacy= \
	-Denable_docs=false

define Package/libva/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/va
	$(CP) $(PKG_INSTALL_DIR)/usr/include/va/*.h $(1)/usr/include/va
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

$(eval $(call BuildPackage,libva))
