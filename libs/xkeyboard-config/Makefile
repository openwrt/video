include $(TOPDIR)/rules.mk

PKG_NAME:=xkeyboard-config
PKG_VERSION:=2.45
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=http://www.x.org/releases/individual/data/$(PKG_NAME)/
PKG_HASH:=169e075a92d957a57787c199e84e359df2931b7196c1c5b4a3d576ee6235a87c

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

PKG_BUILD_DEPENDS:=libxslt/host perl-xml-parser/host python3/host

PYTHON3_PKG_BUILD:=0
MESON_USE_STAGING_PYTHON:=1
HOST_PYTHON3_PACKAGE_BUILD_DEPENDS:=./host-pip-requirements.txt

include $(INCLUDE_DIR)/package.mk
include $(TOPDIR)/feeds/packages/lang/python/python3-package.mk
include $(INCLUDE_DIR)/meson.mk

MESON_VARS += PYTHONPATH="$(HOST_PYTHON3PATH)"

MESON_ARGS += \
	-Dnls=$(if $(CONFIG_BUILD_NLS),true,false)

define Package/xkeyboard-config
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=xkeyboard-config
  URL:=https://www.freedesktop.org/wiki/Software/XKeyboardConfig/
  DEPENDS:=+libxkbcommon $(INTL_DEPENDS)
endef

define Package/xkeyboard-config/description
  The non-arch keyboard configuration database for X Window.
endef

define Package/xkeyboard-config/install
	$(INSTALL_DIR) $(1)/usr/share/xkeyboard-config-2
	$(CP) $(PKG_INSTALL_DIR)/usr/share/xkeyboard-config-2/* $(1)/usr/share/xkeyboard-config-2
	$(INSTALL_DIR) $(1)/usr/share/X11
	$(LN) ../xkeyboard-config-2 $(1)/usr/share/X11/xkb
endef

$(eval $(call BuildPackage,xkeyboard-config))
