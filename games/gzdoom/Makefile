include $(TOPDIR)/rules.mk

PKG_NAME:=gzdoom
PKG_VERSION:=4.14.2
PKG_RELEASE:=1

PKG_SOURCE:=g$(PKG_VERSION).zip
PKG_SOURCE_URL:=https://github.com/ZDoom/gzdoom/archive/refs/tags/
PKG_HASH:=e1d3efd0d0a9b9c83357b87b1c8c2f9a5f68f144b28494dbae26369e7b378b7c

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-g$(PKG_VERSION)
HOST_BUILD_DIR:=$(BUILD_DIR_HOST)/$(PKG_NAME)-g$(PKG_VERSION)

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-3.0-or-later BSD-3-Clause
PKG_LICENSE_FILES:=LICENSE

CMAKE_INSTALL:=1
PKG_BUILD_DEPENDS:=gzdoom/host vulkan-loader
PKG_BUILD_FLAGS:=no-mips16

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_OPTIONS += \
	-DHAVE_VULKAN=ON \
	-DHAVE_GLES2=ON \
	-DFORCE_INTERNAL_BZIP2=ON \
	-DSDL2_INCLUDE_DIR="$(STAGING_DIR)/usr/include/SDL2" \
	-DIMPORT_EXECUTABLES="$(STAGING_DIR_HOSTPKG)/share/gzdoom/ImportExecutables.cmake" \
	-DVULKAN_USE_XLIB=OFF \
	-DVULKAN_USE_WAYLAND=ON

TARGET_LDFLAGS += -latomic

define Package/gzdoom
  SECTION:=games
  CATEGORY:=Games
  TITLE:=GZDoom
  URL:=https://zdoom.org/
  DEPENDS:=+alsa-lib +glib2 +libatomic +libbz2 +libstdcpp +libzmusic +libvpx +libsdl2 +USE_MUSL:musl-fts @(ARCH_64BIT||BROKEN)
endef

define Package/gzdoom/description
 ZDoom is a family of enhanced ports of the Doom engine for running on modern
 operating systems. Use this port of DOOM on systems with some sort of hardware
 3D acceleration support.
endef

define Host/Install
	$(INSTALL_DIR) $(STAGING_DIR_HOSTPKG)/share/gzdoom
	$(INSTALL_BIN) $(HOST_BUILD_DIR)/tools/re2c/re2c $(STAGING_DIR_HOSTPKG)/share/gzdoom
	$(INSTALL_BIN) $(HOST_BUILD_DIR)/tools/lemon/lemon $(STAGING_DIR_HOSTPKG)/share/gzdoom
	$(INSTALL_DATA) $(HOST_BUILD_DIR)/tools/lemon/lempar.c $(STAGING_DIR_HOSTPKG)/share/gzdoom
	$(INSTALL_BIN) $(HOST_BUILD_DIR)/tools/zipdir/zipdir $(STAGING_DIR_HOSTPKG)/share/gzdoom
	$(INSTALL_DATA) $(HOST_BUILD_DIR)/ImportExecutables.cmake $(STAGING_DIR_HOSTPKG)/share/gzdoom
	$(SED) "s#$(HOST_BUILD_DIR)/tools/re2c/re2c#$(STAGING_DIR_HOSTPKG)/share/gzdoom/re2c#" \
		$(STAGING_DIR_HOSTPKG)/share/gzdoom/ImportExecutables.cmake
	$(SED) "s#$(HOST_BUILD_DIR)/tools/lemon/lemon#$(STAGING_DIR_HOSTPKG)/share/gzdoom/lemon#" \
		$(STAGING_DIR_HOSTPKG)/share/gzdoom/ImportExecutables.cmake
	$(SED) "s#$(HOST_BUILD_DIR)/tools/zipdir/zipdir#$(STAGING_DIR_HOSTPKG)/share/gzdoom/zipdir#" \
		$(STAGING_DIR_HOSTPKG)/share/gzdoom/ImportExecutables.cmake
endef

define Host/Uninstall
	$(RM) $(STAGING_DIR_HOSTPKG)/share/gzdoom/ImportExecutables.cmake
endef

define Package/gzdoom/install
	$(INSTALL_DIR) $(1)/usr/share/games
	$(CP) $(PKG_INSTALL_DIR)/usr/share/games/* $(1)/usr/share/games
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gzdoom $(1)/usr/bin
endef

$(eval $(call BuildPackage,gzdoom))
$(eval $(call HostBuild))
