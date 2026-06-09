include $(TOPDIR)/rules.mk

PKG_NAME:=sdl3-doom
PKG_SOURCE_DATE:=2025-01-26
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/raydelto/sdl3_doom
PKG_SOURCE_VERSION:=b271b6d72900d18ee3ef34c1608fe7e20a0c0ad3
PKG_MIRROR_HASH:=0d65720a1d68e63721cca360ebc2614fc2ad2e624df1e60a293a24ce67279373

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=License.md

PKG_BUILD_FLAGS:=gc-sections lto

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/sdl3-doom
  SECTION:=games
  CATEGORY:=Games
  TITLE:=SDL3 Doom
  URL:=https://github.com/raydelto/sdl3_doom
  DEPENDS:=+libstdcpp +libsdl3 +libsdl3-mixer
endef

define Package/sdl3-doom/description
  This is a source port of the ID Software source release of DOOM to SDL3.
  Using low-resolution software rendering designed in 1993 by idSoftware
  for (from today's perspective) very slow CPUs it runs with good
  performance even on low-end devices.
endef

define Package/sdl3-doom/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sdl3-doom $(1)/usr/bin
endef

define Build/Install
endef

$(eval $(call BuildPackage,sdl3-doom))
