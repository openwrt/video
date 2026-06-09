include $(TOPDIR)/rules.mk

PKG_NAME:=vulkan-tools
PKG_VERSION:=1.4.350.0

PKG_SOURCE:=Vulkan-Tools-vulkan-sdk-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/KhronosGroup/Vulkan-Tools/tar.gz/vulkan-sdk-$(PKG_VERSION)?
PKG_HASH:=3079796d51b29ce49dc7b7c7e243df93b343d54c3be9d4a8292c3231b9698deb

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=Apache-2.0
PKG_LICENSE_FILES:=LICENSE.txt

PKG_BUILD_DIR:=$(BUILD_DIR)/Vulkan-Tools-vulkan-sdk-$(PKG_VERSION)

PKG_BUILD_DEPENDS:=vulkan-headers
CMAKE_BINARY_SUBDIR:=build
CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_OPTIONS += \
	-DBUILD_VULKANINFO=ON \
	-DBUILD_CUBE=OFF \
	-DBUILD_ICD=OFF \
	-DBUILD_TESTS=OFF \
	-DBUILD_WSI_XCB_SUPPORT=OFF \
	-DBUILD_WSI_XLIB_SUPPORT=OFF \
	-DBUILD_WSI_WAYLAND_SUPPORT=ON \
	-DBUILD_WSI_DIRECTFB_SUPPORT=OFF

define Package/vulkaninfo
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=Video
  TITLE:=vulkaninfo
  URL:=https://github.com/KhronosGroup/Vulkan-Tools
  DEPENDS:=+libstdcpp +vulkan-loader +libwayland
endef

define Package/vulkaninfo/description
The Khronos official Vulkan utilities package. Provides vulkaninfo,
a tool for querying Vulkan installations: enumerated GPUs, supported
extensions, format properties, surface capabilities, etc.
endef

define Package/vulkaninfo/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/vulkaninfo $(1)/usr/bin
endef

$(eval $(call BuildPackage,vulkaninfo))
