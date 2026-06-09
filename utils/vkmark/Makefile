include $(TOPDIR)/rules.mk

PKG_NAME:=vkmark
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_DATE:=2025-09-08
PKG_SOURCE_VERSION:=83a128d65b1adddfacb4fa2e555a7e90817a7266
PKG_SOURCE_URL:=https://github.com/$(PKG_NAME)/$(PKG_NAME).git
PKG_MIRROR_HASH:=8be3889bec221dceb026dc00e7041cabb5086122fd2d7e1cb50618d88c0f4a80

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=LGPL-2.1
PKG_LICENSE_FILES:=COPYING-LGPL2.1

PKG_BUILD_FLAGS:=gc-sections lto
PKG_BUILD_DEPENDS:=glm

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk

define Package/vkmark
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=Video
  TITLE:=Vulkan benchmark
  URL:=https://github.com/vkmark/vkmark
  DEPENDS:=+libstdcpp +libassimp +libdrm +libmesa +libwayland +vulkan-loader
endef

define Package/vkmark/description
vkmark is an extensible Vulkan benchmarking suite with targeted, configurable scenes.
endef

MESON_VARS += CMAKE_PREFIX_PATH=$(STAGING_DIR)/usr

MESON_ARGS += \
	-Dxcb=false \
	-Dwayland=true \
	-Dkms=true

define Package/vkmark/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib/vkmark
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/vkmark/*.so $(1)/usr/lib/vkmark
	$(INSTALL_DIR) $(1)/usr/share/vkmark
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vkmark/* $(1)/usr/share/vkmark
endef

$(eval $(call BuildPackage,vkmark))
