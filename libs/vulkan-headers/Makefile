include $(TOPDIR)/rules.mk

PKG_NAME:=vulkan-headers
PKG_VERSION:=1.4.363.0
PKG_RELEASE:=1
PKG_SOURCE:=Vulkan-Headers-vulkan-sdk-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/KhronosGroup/Vulkan-Headers/tar.gz/vulkan-sdk-$(PKG_VERSION)?
PKG_HASH:=4a078be12bef21cfebc09d878b77a63cff9d68f899254a0b00d0e37ef73e7f7e

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
