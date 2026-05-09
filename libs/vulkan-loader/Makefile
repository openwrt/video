include $(TOPDIR)/rules.mk

PKG_NAME:=vulkan-loader
PKG_VERSION:=1.4.341.0

PKG_SOURCE:=Vulkan-Loader-vulkan-sdk-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/KhronosGroup/Vulkan-Loader/tar.gz/vulkan-sdk-$(PKG_VERSION)?
PKG_HASH:=fe982697c780a950641bfcf94707135c26c501352242d285fa95d087d691292e

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=Apache-2.0
PKG_LICENSE_FILES:=LICENSE.txt

PKG_BUILD_DIR:=$(BUILD_DIR)/Vulkan-Loader-vulkan-sdk-$(PKG_VERSION)

PKG_BUILD_DEPENDS:=vulkan-headers
CMAKE_BINARY_SUBDIR:=build
CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_OPTIONS += \
	-DBUILD_WSI_XCB_SUPPORT=OFF \
	-DBUILD_WSI_XLIB_SUPPORT=OFF \
	-DBUILD_WSI_WAYLAND_SUPPORT=ON \
	-DBUILD_WSI_DIRECTFB_SUPPORT=OFF

define Package/vulkan-loader
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=Vulkan Loader
  URL:=https://github.com/KhronosGroup/Vulkan-Loader
endef

define Package/vulkan-loader/description
This project provides the Khronos official Vulkan Loader.
endef

define Package/vulkan-loader/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,vulkan-loader))
