include $(TOPDIR)/rules.mk

PKG_NAME:=vulkan-loader
PKG_VERSION:=1.3.302

PKG_SOURCE:=Vulkan-Loader-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/KhronosGroup/Vulkan-Loader/tar.gz/v$(PKG_VERSION)?
PKG_HASH:=7f8d3e5c7428fe2d6220a4ea1c0270c7695bfef8e48c38b827e9e81880637710

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
