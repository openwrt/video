#
# Copyright (C) 2007-2019 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=gtk
PKG_VERSION:=4.18.6
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=@GNOME/$(PKG_NAME)/$(basename $(PKG_VERSION))
PKG_HASH:=e1817c650ddc3261f9a8345b3b22a26a5d80af154630dedc03cc7becefffd0fa

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=LGPL-2.1-or-later
PKG_LICENSE_FILES:=COPYING
PKG_CPE_ID:=cpe:/a:gnome:gtk

PKG_BUILD_DEPENDS:=gst1-plugins-base gst1-plugins-bad

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include $(INCLUDE_DIR)/meson.mk

define Package/gtk/Default
  URL:=http://www.gtk.org/
endef

define Package/gtk/description/Default
  The GTK Project - A free and open-source cross-platform widget toolkit
endef

define Package/libgtk
$(call Package/gtk/Default)
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  DEPENDS:=$(ICONV_DEPENDS) $(INTL_DEPENDS) +fribidi +glib2 +harfbuzz +libcairo \
	   +libepoxy +libgdk-pixbuf +libgraphene +libgstreamer1 +libgst1player \
	   +libpango +libxkbcommon
  TITLE:=GTK library
endef

define Package/libgtk/description
$(call Package/gtk/description/Default)

  Shared library and basic files needed at runtime.
endef

define Package/libgtk-utils
$(call Package/gtk/Default)
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=Video
  DEPENDS:=+libgtk
  TITLE:=GTK utils
endef

define Package/libgtk-utils/description
$(call Package/gtk/description/Default)

  Utility programs which come with the GTK library.
endef

define Package/libgtk-demos
$(call Package/gtk/Default)
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=Video
  DEPENDS:=+libgtk
  TITLE:=GTK demos
endef

define Package/libgtk-demos/description
$(call Package/gtk/description/Default)

  Demo programs which come with the GTK library.
endef

MESON_ARGS += \
	-Dintrospection=disabled \
	-Dx11-backend=false \
	-Dwayland-backend=true \
	-Dbroadway-backend=false \
	-Dmedia-gstreamer=enabled \
	-Dvulkan=disabled \
	-Dprint-cpdb=disabled \
	-Dprint-cups=disabled \
	-Dcloudproviders=disabled \
	-Dbuild-testsuite=false \
	-Dbuild-tests=false

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/gtk-4.0 $(1)/usr/include/

	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/gtk-4.0 $(1)/usr/lib/

	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/

	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc \
		$(1)/usr/lib/pkgconfig

endef

define Package/libgtk/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib/

	$(INSTALL_DIR) $(1)/usr/share/glib-2.0/schemas
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/glib-2.0/schemas/org.gtk.gtk4.Settings.ColorChooser.gschema.xml $(1)/usr/share/glib-2.0/schemas
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/glib-2.0/schemas/org.gtk.gtk4.Settings.Debug.gschema.xml $(1)/usr/share/glib-2.0/schemas
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/glib-2.0/schemas/org.gtk.gtk4.Settings.EmojiChooser.gschema.xml $(1)/usr/share/glib-2.0/schemas
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/glib-2.0/schemas/org.gtk.gtk4.Settings.FileChooser.gschema.xml $(1)/usr/share/glib-2.0/schemas

	$(INSTALL_DIR) $(1)/usr/share/icons
	$(CP) $(PKG_INSTALL_DIR)/usr/share/icons/hicolor $(1)/usr/share/icons
endef

define Package/libgtk-demos/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-demo $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-demo-application $(1)/usr/bin

	$(INSTALL_DIR) $(1)/usr/share/glib-2.0/schemas
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/glib-2.0/schemas/org.gtk.Demo4.gschema.xml $(1)/usr/share/glib-2.0/schemas

	$(INSTALL_DIR) $(1)/usr/share/applications
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/applications/org.gtk.Demo4.desktop $(1)/usr/share/applications

	$(INSTALL_DIR) $(1)/usr/share/metainfo
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/metainfo/org.gtk.Demo4.appdata.xml $(1)/usr/share/metainfo
endef

define Package/libgtk-utils/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-image-tool $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-encode-symbolic-svg $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-path-tool $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-query-settings $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-launch $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-builder-tool $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-rendernode-tool $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-print-editor $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-update-icon-cache $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-node-editor $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gtk4-widget-factory $(1)/usr/bin

	$(INSTALL_DIR) $(1)/usr/share/applications
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/applications/org.gtk.WidgetFactory4.desktop $(1)/usr/share/applications
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/applications/org.gtk.gtk4.NodeEditor.desktop $(1)/usr/share/applications
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/applications/org.gtk.PrintEditor4.desktop $(1)/usr/share/applications

	$(INSTALL_DIR) $(1)/usr/share/metainfo
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/metainfo/org.gtk.PrintEditor4.appdata.xml $(1)/usr/share/metainfo
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/metainfo/org.gtk.gtk4.NodeEditor.appdata.xml $(1)/usr/share/metainfo
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/share/metainfo/org.gtk.WidgetFactory4.appdata.xml $(1)/usr/share/metainfo
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,libgtk))
$(eval $(call BuildPackage,libgtk-demos))
$(eval $(call BuildPackage,libgtk-utils))
