include $(TOPDIR)/rules.mk

PKG_NAME:=glslang
PKG_VERSION:=15.4.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/KhronosGroup/$(PKG_NAME)/tar.gz/$(PKG_VERSION)?
PKG_HASH:=b16c78e7604b9be9f546ee35ad8b6db6f39bbbbfb19e8d038b6fe2ea5bba4ff4

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-3-Clause BSD-2-Clause MIT Apache GPL-3.0-or-later
PKG_LICENSE_FILES:=LICENSE.txt

CMAKE_INSTALL:=1
PKG_BUILD_FLAGS:=gc-sections lto

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
