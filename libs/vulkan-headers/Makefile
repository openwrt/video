include $(TOPDIR)/rules.mk

PKG_NAME:=vulkan-headers
PKG_VERSION:=1.4.357.0
PKG_RELEASE:=1
PKG_SOURCE:=Vulkan-Headers-vulkan-sdk-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/KhronosGroup/Vulkan-Headers/tar.gz/vulkan-sdk-$(PKG_VERSION)?
PKG_HASH:=e87dce08116151f6b6d7de6b6faf41498e87e6cf848ff16fa3bd5402190ad4a3

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=Apache-2.0 OR MIT
PKG_LICENSE_FILES:=LICENSE.md

PKG_BUILD_DIR:=$(BUILD_DIR)/Vulkan-Headers-vulkan-sdk-$(PKG_VERSION)

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
