include $(TOPDIR)/rules.mk

PKG_NAME:=spirv-headers
PKG_VERSION:=1.4.328.0

PKG_SOURCE:=SPIRV-Headers-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/KhronosGroup/SPIRV-Headers/tar.gz/vulkan-sdk-$(PKG_VERSION)?
PKG_HASH:=00284a33e1e19014723c8e88ca7a16e8988cd23f839ec2b7da6bb1808fd2a751

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE

PKG_BUILD_DIR:=$(BUILD_DIR)/SPIRV-Headers-vulkan-sdk-$(PKG_VERSION)
HOST_BUILD_DIR:=$(BUILD_DIR_HOST)/SPIRV-Headers-vulkan-sdk-$(PKG_VERSION)

CMAKE_BINARY_SUBDIR := build
#PKG_BUILD_DEPENDS:=llvm
#HOST_BUILD_DEPENDS:=llvm/host

CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_HOST_OPTIONS += \
	-DSPIRV_HEADERS_ENABLE_TESTS=OFF \
	-DSPIRV_HEADERS_ENABLE_INSTALL=ON

CMAKE_OPTIONS += \
	-DSPIRV_HEADERS_ENABLE_TESTS=OFF \
	-DSPIRV_HEADERS_ENABLE_INSTALL=ON

define Package/spirv-headers
  SECTION:=devel
  CATEGORY:=Development
  TITLE:=SPIR-V Headers
  URL:=https://github.com/KhronosGroup/SPIRV-Headers
  BUILDONLY:=1
endef

define Package/spirv-headers/description
The LLVM Project is a collection of modular and reusable compiler and toolchain technologies.
endef

define Package/spirv-headers/install
true
endef

$(eval $(call BuildPackage,spirv-headers))
$(eval $(call HostBuild))
