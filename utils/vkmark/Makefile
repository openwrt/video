include $(TOPDIR)/rules.mk

PKG_NAME:=vkmark
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_DATE:=2023-04-12
PKG_SOURCE_VERSION:=ab6e6f34077722d5ae33f6bd40b18ef9c0e99a15
PKG_SOURCE_URL:=https://github.com/$(PKG_NAME)/$(PKG_NAME).git
PKG_MIRROR_HASH:=a0f268a88239d60650bc6ab2fa57cb99b75e28d4a11a07708c7dced5deea8fcb

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
