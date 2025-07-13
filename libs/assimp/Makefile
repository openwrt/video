include $(TOPDIR)/rules.mk

PKG_NAME:=assimp
PKG_VERSION:=6.0.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/$(PKG_NAME)/$(PKG_NAME)/tar.gz/v$(PKG_VERSION)?
PKG_HASH:=d1822d9a19c9205d6e8bc533bf897174ddb360ce504680f294170cc1d6319751

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-3-Clause

CMAKE_INSTALL:=1
PKG_BUILD_FLAGS:=gc-sections lto

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/libassimp
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=Open Asset Importer Library
  URL:=https://github.com/assimp/assimp
  DEPENDS:=+libstdcpp +minizip +zlib
endef

define Package/libassimp/description
Open Asset Import Library is a library that loads various 3D file formats
into a shared, in-memory format. It supports more than 40 file formats
for import and a growing selection of file formats for export.
endef

CMAKE_OPTIONS += \
	-DBUILD_SHARED_LIBS=ON

define Build/InstallDev
	$(call Build/InstallDev/cmake,$(1))
	$(SED) 's,/usr/include,$$$${prefix}/include,g' $(1)/usr/lib/pkgconfig/assimp.pc
	$(SED) 's,/usr/lib,$$$${prefix}/lib,g' $(1)/usr/lib/pkgconfig/assimp.pc
endef

define Package/libassimp/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,libassimp))
