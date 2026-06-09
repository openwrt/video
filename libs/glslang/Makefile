include $(TOPDIR)/rules.mk

PKG_NAME:=glslang
# 16.3.0 references the same commit as the Vulkan SDK 1.4.350.0 release tag
# (vulkan-sdk-1.4.350.0); use the semver tag here to keep package-manager
# version comparisons monotonic.
PKG_VERSION:=16.3.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/KhronosGroup/$(PKG_NAME)/tar.gz/$(PKG_VERSION)?
PKG_HASH:=efff5a15258dce1ca2d323bf64c974f5fca03778174615dbc30c8d36db645bf5

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-3-Clause BSD-2-Clause MIT Apache GPL-3.0-or-later
PKG_LICENSE_FILES:=LICENSE.txt

CMAKE_INSTALL:=1
PKG_BUILD_FLAGS:=gc-sections no-lto

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/glslang
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=GLSL/ESSL, HLSL and SPIR-V generator
  URL:=https://github.com/KhronosGroup/glslang
  DEPENDS:=+libstdcpp
endef

define Package/glslang/description
Khronos-reference front end for GLSL/ESSL, partial front end for HLSL, and a SPIR-V generator.
endef

CMAKE_OPTIONS += \
	-DBUILD_SHARED_LIBS=ON \
	-DBUILD_TESTING=OFF \
	-DENABLE_OPT=0

CMAKE_HOST_OPTIONS += \
	-DBUILD_SHARED_LIBS=OFF \
	-DBUILD_TESTING=OFF \
	-DENABLE_OPT=0

TARGET_CXXFLAGS += -fno-rtti

define Package/glslang/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,glslang))
$(eval $(call HostBuild))
