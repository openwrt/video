include $(TOPDIR)/rules.mk

PKG_NAME:=glm
PKG_VERSION:=1.0.1
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/g-truc/$(PKG_NAME)/tar.gz/$(PKG_VERSION)?
PKG_HASH:=9f3174561fd26904b23f0db5e560971cbf9b3cbda0b280f04d5c379d03bf234c

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT

CMAKE_INSTALL:=1
PKG_BUILD_FLAGS:=gc-sections lto

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/libglm
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=OpenGL Mathematics (GLM)
  URL:=https://github.com/g-truc/glm
  DEPENDS:=+libstdcpp
endef

define Package/libglm/description
OpenGL Mathematics (GLM) is a header only C++ mathematics library for
graphics software based on the OpenGL Shading Language (GLSL)
specifications.
endef

CMAKE_OPTIONS += \
	-DBUILD_SHARED_LIBS=ON

define Package/libglm/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,libglm))
