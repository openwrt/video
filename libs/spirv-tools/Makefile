include $(TOPDIR)/rules.mk

PKG_NAME:=spirv-tools
PKG_VERSION:=1.3.296.0

PKG_SOURCE:=SPIRV-Tools-vulkan-sdk-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/KhronosGroup/SPIRV-Tools/tar.gz/vulkan-sdk-$(PKG_VERSION)?
PKG_HASH:=75aafdf7e731b4b6bfb36a590ddfbb38ebc605d80487f38254da24fe0cb95837

PKG_BUILD_DIR:=$(BUILD_DIR)/SPIRV-Tools-vulkan-sdk-$(PKG_VERSION)
HOST_BUILD_DIR:=$(BUILD_DIR_HOST)/SPIRV-Tools-vulkan-sdk-$(PKG_VERSION)

PKG_BUILD_DEPENDS:=spirv-headers
HOST_BUILD_DEPENDS:=spirv-headers/host

CMAKE_BINARY_SUBDIR:=build
CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_OPTIONS += \
	-DSPIRV-Headers_SOURCE_DIR=$(STAGING_DIR)/usr \
	-DSPIRV_SKIP_TESTS=ON

CMAKE_HOST_OPTIONS += \
	-DSPIRV-Headers_SOURCE_DIR=$(STAGING_DIR_HOSTPKG) \
	-DSPIRV_SKIP_TESTS=ON

define Package/spirv-tools
  SECTION:=devel
  CATEGORY:=Development
  TITLE:=SPIR-V Tools
  URL:=https://github.com/KhronosGroup/SPIRV-Tools
  BUILDONLY:=1
endef

define Package/spirv-tools/description
The LLVM Project is a collection of modular and reusable compiler and toolchain technologies.
endef

define Package/spirv-tools/install
true
endef

$(eval $(call BuildPackage,spirv-tools))
$(eval $(call HostBuild))
