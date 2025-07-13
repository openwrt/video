include $(TOPDIR)/rules.mk

PKG_NAME:=vulkan-loader
PKG_VERSION:=1.4.313

PKG_SOURCE:=Vulkan-Loader-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/KhronosGroup/Vulkan-Loader/tar.gz/v$(PKG_VERSION)?
PKG_HASH:=0c2436993597f5bd0ee420b6b27632758ed3ab439043d251795fd13d4e70a2f3

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=Apache-2.0
PKG_LICENSE_FILES:=LICENSE.txt

PKG_BUILD_DIR:=$(BUILD_DIR)/Vulkan-Loader-$(PKG_VERSION)

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
