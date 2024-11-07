include $(TOPDIR)/rules.mk

PKG_NAME:=sdl2-doom
PKG_SOURCE_DATE:=2022-03-06
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/AlexOberhofer/sdl2-doom
PKG_SOURCE_VERSION:=da7732ee6318371db2ee04ec4702c6064245846b
PKG_MIRROR_HASH:=871f9892ac49766e2729ae5c6ce1f75f830361aea048f2e8a64c3db63de7e121

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
