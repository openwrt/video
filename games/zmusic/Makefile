include $(TOPDIR)/rules.mk

PKG_NAME:=zmusic
PKG_VERSION:=1.1.14
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/ZDoom/ZMusic
PKG_SOURCE_VERSION:=89f3d65734470fb7ec0c1e69f73a0cfcc88ed557
PKG_MIRROR_HASH:=104af83c55909a3cc2151bf9a32b153355fab76ee503bf199f49fb401f595a2b

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-3.0
PKG_LICENSE_FILES:=licenses/bsd.txt licenses/dumb.txt licenses/gplv3.txt \
		   licenses/legal.txt licenses/lgplv21.txt licenses/lgplv3.txt \
		   licenses/zmusic.txt

CMAKE_INSTALL:=1
PKG_BUILD_FLAGS:=gc-sections lto

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/libzmusic
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=ZMusic
  URL:=https://zdoom.org/
  DEPENDS:=+libstdcpp +libsndfile +alsa-lib +glib2
endef

define Package/libzmusic/description
  GZDoom's music system as a standalone library
endef

define Package/libzmusic/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libzmusic.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libzmusic))
