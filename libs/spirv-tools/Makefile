include $(TOPDIR)/rules.mk

PKG_NAME:=spirv-tools
PKG_VERSION:=1.4.313.0

PKG_SOURCE:=SPIRV-Tools-vulkan-sdk-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/KhronosGroup/SPIRV-Tools/tar.gz/vulkan-sdk-$(PKG_VERSION)?
PKG_HASH:=6b60f723345ceed5291cceebbcfacf7fea9361a69332261fa08ae57e2a562005

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE

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
