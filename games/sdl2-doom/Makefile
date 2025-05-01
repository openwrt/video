include $(TOPDIR)/rules.mk

PKG_NAME:=sdl2-doom
PKG_SOURCE_DATE:=2024-05-31
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/gen04177/sdl2-doom
PKG_SOURCE_VERSION:=fbbfb86b34af2cf267a9287cd91a5d1575fcfe58
PKG_MIRROR_HASH:=b0f6b325c29c1bfb427e58190c9d560be7f99491656c219f10e4d72bc33c4ed8

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=License.md

PKG_BUILD_FLAGS:=gc-sections lto

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/sdl2-doom
  SECTION:=games
  CATEGORY:=Games
  TITLE:=SDL2 Doom
  URL:=https://github.com/AlexOberhofer/sdl2-doom
  DEPENDS:=+libstdcpp +libsdl2 +libsdl2-mixer
endef

define Package/sdl2-doom/description
  This is a source port of the ID Software source release of DOOM.
  Using low-resolution software rendering designed in 1993 by idSoftware
  for (from today's perspective) very slow CPUs it runs with good
  performance even on low-end devices.
endef

define Package/sdl2-doom/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sdl2-doom $(1)/usr/bin
endef

define Build/Install
endef

$(eval $(call BuildPackage,sdl2-doom))
