include $(TOPDIR)/rules.mk

PKG_NAME:=vulkan-headers
PKG_VERSION:=1.4.328
PKG_SOURCE:=Vulkan-Headers-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/KhronosGroup/Vulkan-Headers/tar.gz/v$(PKG_VERSION)?
PKG_HASH:=3ad56d387179b47dd632432ccf989bbb69773e1d732692b7bbad9c4d36aa1304

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=Apache-2.0 OR MIT
PKG_LICENSE_FILES:=LICENSE.md

PKG_BUILD_DIR:=$(BUILD_DIR)/Vulkan-Headers-$(PKG_VERSION)

CMAKE_BINARY_SUBDIR:=build
CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/vulkan-headers
  SECTION:=devel
  CATEGORY:=Development
  TITLE:=Vulkan Headers
  URL:=https://github.com/KhronosGroup/Vulkan-Headers
  BUILDONLY:=1
endef

define Package/vulkan-headers/description
Vulkan header files and API registry.
endef

define Package/vulkan-headers/install
true
endef

$(eval $(call BuildPackage,vulkan-headers))
