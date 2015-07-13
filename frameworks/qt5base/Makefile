#
# Copyright (C) 2015 OpenWrt.org
# Author: Mirko Vogt <mirko@openwrt.org>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

# TODO / known bugs:
# - plugins don't work when sstrip is used for stripping
# - only framebuffer (linuxfb) support for now, XBC/DirectFB to come - thus no GL thus no qtquick2 / QML2, as it has a hard dependency on GL
# - host_build functionality seems to be broken - qmake doesn't switch to the host toolchain (linux-g++) when host_build gets invoked

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5base
PKG_VERSION:=5.4.2
PKG_RELEASE:=1
PKG_MD5SUM:=67a95eec79ffc4a14f516ad6f3d24c96

PKG_SYS_NAME:=qtbase-opensource-src-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_SYS_NAME).tar.xz
PKG_SOURCE_URL:=http://download.qt-project.org/official_releases/qt/$(basename $(PKG_VERSION))/$(PKG_VERSION)/submodules

PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SYS_NAME)
PKG_BUILD_PARALLEL:=1
PKG_BUILD_DEPENDS:=librpc
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include ./files/qmake.mk

define Package/qt5base/Default
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Qt5base
  DEPENDS:=qt5base
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko@openwrt.org>
endef

define Package/qt5base
  $(call Package/qt5base/Default)
  DEPENDS:=
  MENU:=1
endef

define Package/qt5base-concurrent
  $(call Package/qt5base/Default)
  TITLE+=concurrent
  DEPENDS+=+qt5base-core
endef

define Package/qt5base-core
  $(call Package/qt5base/Default)
  TITLE+=core
  DEPENDS+=+libpthread +zlib +libpcre16 +libstdcpp +icu +librt +libiconv-full
endef

define Package/qt5base-gui
  $(call Package/qt5base/Default)
  TITLE+=gui
  DEPENDS+=+qt5base-core +libpng
endef

define Package/qt5base-network
  $(call Package/qt5base/Default)
  TITLE+=network
  DEPENDS+=+qt5base-core +libopenssl
endef

# seems to be only present as static lib
# define Package/qt5base-paltformSupport
#   $(call Package/qt5base/Default)
#   DEPENDS+=
# endef

define Package/qt5base-printSupport
  $(call Package/qt5base/Default)
  TITLE+=print support
  DEPENDS+=+qt5base-core +qt5base-gui +qt5base-widgets
endef

define Package/qt5base-sql
  $(call Package/qt5base/Default)
  TITLE+=sql
  DEPENDS+=+qt5base-core
endef

define Package/qt5base-test
  $(call Package/qt5base/Default)
  TITLE+=test
  DEPENDS+=+qt5base-core
endef

define Package/qt5base-widgets
  $(call Package/qt5base/Default)
  TITLE+=widgets
  DEPENDS+=+qt5base-core +qt5base-gui
endef

define Package/qt5base-xml
  $(call Package/qt5base/Default)
  TITLE+=xml
  DEPENDS+=+qt5base-core
endef

#### plugins

####### bearer

define Package/qt5base-plugin-bearer-generic
  $(call Package/qt5base/Default)
TITLE+=bearer (plugin)
  DEPENDS+=+qt5base-core +qt5base-network
endef

####### generic / input

define Package/qt5base-plugin-input-evdevkeyboard
  $(call Package/qt5base/Default)
  TITLE+=evdev keyboard (plugin)
  DEPENDS+=+qt5base-core +qt5base-gui +libfreetype +fontconfig +udev
endef

define Package/qt5base-plugin-input-evdevmouse
  $(call Package/qt5base/Default)
  TITLE+=evdev mouse (plugin)
  DEPENDS+=+qt5base-core +qt5base-gui +libfreetype +fontconfig +udev
endef

define Package/qt5base-plugin-input-evdevtablet
  $(call Package/qt5base/Default)
  TITLE+=evdev tablet (plugin)
  DEPENDS+=+qt5base-core +qt5base-gui +libfreetype +fontconfig +udev
endef

define Package/qt5base-plugin-input-evdevtouch
  $(call Package/qt5base/Default)
  TITLE+=evdev touch (plugin)
  DEPENDS+=+qt5base-core +qt5base-gui +libfreetype +fontconfig +udev
endef

####### imageformats

define Package/qt5base-plugin-imageformats-gif
  $(call Package/qt5base/Default)
TITLE+=imageformat gif (plugin)
  DEPENDS+=+qt5base-core +qt5base-gui
endef

define Package/qt5base-plugin-imageformats-ico
  $(call Package/qt5base/Default)
  TITLE+=imageformat ico (plugin)
  DEPENDS+=+qt5base-core +qt5base-gui
endef

define Package/qt5base-plugin-imageformats-jpeg
  $(call Package/qt5base/Default)
  TITLE+=imageformat jpeg (plugin)
  DEPENDS+=+qt5base-core +qt5base-gui +libjpeg
endef

####### platforms

define Package/qt5base-plugin-platforms-linuxfb
  $(call Package/qt5base/Default)
  TITLE+=platform linuxfb (plugin)
  DEPENDS+=+qt5base-core +qt5base-gui +libfreetype +fontconfig +udev
endef

define Package/qt5base-plugin-platforms-minimal
  $(call Package/qt5base/Default)
  TITLE+=platform minimal (plugin)
  DEPENDS+=+qt5base-core +qt5base-gui +libfreetype +fontconfig +udev
endef

define Package/qt5base-plugin-platforms-offscreen
  $(call Package/qt5base/Default)
  TITLE+=platform offscreen (plugin)
  DEPENDS+=+qt5base-core +qt5base-gui +libfreetype +fontconfig +udev
endef

###### sql

define Package/qt5base-plugin-sqldrivers-sqlite
  $(call Package/qt5base/Default)
  TITLE+=sqldriver sqlite (plugin)
  DEPENDS+=+qt5base-core +qt5base-sql +libsqlite3
endef

define Package/qt5base-examples
  $(call Package/qt5base/Default)
  TITLE+=examples
  DEPENDS+=\
		+qt5base-core \
		+qt5base-gui \
		+qt5base-widgets \
		+qt5base-network \
		+qt5base-sql \
		+qt5base-xml \
		+qt5base-printSupport \
		+qt5base-concurrent 
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) \
		./files/mkspecs/linux-openwrt-g++ \
		$(PKG_BUILD_DIR)/mkspecs/
endef


TARGET_CFLAGS+= -I$(STAGING_DIR)/usr/include/freetype2/ -I$(STAGING_DIR)/usr/lib/libiconv-full/include/ -liconv
TARGET_CXXFLAGS+= -I$(STAGING_DIR)/usr/include/freetype2/ -I$(STAGING_DIR)/usr/lib/libiconv-full/include/ -liconv
TARGET_LDFLAGS += -L$(STAGING_DIR)/usr/lib/libiconv-full/lib/ -liconv

define Build/Configure
	# CROSS/TARGET_* need to be passed to configure, in order to use cross-compiling tools to check for requirements.
	# Usually used variables such as CC/CXX/CFLAGS/CXXFLAGS/LDFLAGS would be used for compilation of host tools (qmake, moc, etc.),
	# hence we use the TARGET_* nomenclature.
	( cd $(PKG_BUILD_DIR) ; \
		TARGET_CROSS="$(TARGET_CROSS)" \
		TARGET_CFLAGS="$(TARGET_CFLAGS)" \
		TARGET_CXXFLAGS="$(TARGET_CFLAGS) $(TARGET_CXXFLAGS)" \
		TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
		./configure \
			-prefix $(QT_INSTALL_PREFIX) \
			-extprefix $(QT_EXTPREFIX) \
			-hostprefix $(QT_HOST_PREFIX) \
			-bindir $(QT_INSTALL_BINS) \
			-headerdir $(QT_INSTALL_HEADERS) \
			-libdir $(QT_INSTALL_LIBS) \
			-archdatadir $(QT_INSTALL_ARCHDATA) \
			-plugindir $(QT_INSTALL_PLUGINS) \
			-libexecdir $(QT_INSTALL_LIBEXECS) \
			-importdir $(QT_INSTALL_IMPORTS) \
			-qmldir $(QT_INSTALL_QML) \
			-datadir $(QT_INSTALL_DATA) \
			-docdir $(QT_INSTALL_DOCS) \
			-translationdir $(QT_INSTALL_TRANSLATIONS) \
			-sysconfdir $(QT_INSTALL_CONFIGURATION) \
			-examplesdir $(QT_INSTALL_EXAMPLES) \
			-testsdir $(QT_INSTALL_TESTS) \
			-hostbindir $(QT_HOST_BINS) \
			-hostlibdir $(QT_HOST_LIBS) \
			-hostdatadir $(QT_HOST_DATA) \
			-v \
			-release \
			-opensource \
			-confirm-license \
			-c++11 \
			-shared \
			-largefile \
			-accessibility \
			-system-sqlite \
			-no-qml-debug \
			-no-sse2 \
			-no-sse3 \
			-no-ssse3 \
			-no-sse4.1 \
			-no-sse4.2 \
			-no-avx \
			-no-avx2 \
			-no-mips_dsp \
			-no-mips_dspr2 \
			-force-pkg-config \
			-system-zlib \
			-mtdev \
			-no-journald \
			-system-libpng \
			-system-libjpeg \
			-system-freetype \
			-no-harfbuzz \
			-openssl-linked \
			-system-pcre \
			-system-xcb \
			-system-xkbcommon \
			-no-xinput2 \
			-no-xcb-xlib \
			-no-glib \
			-no-pulseaudio \
			-alsa \
			-no-gtkstyle \
			-nomake tools \
			-make examples \
			-gui \
			-widgets \
			-no-optimized-qmake \
			-no-cups \
			-iconv \
			-evdev \
			-no-icu \
			-fontconfig \
			-no-strip \
			-no-pch \
			-no-dbus \
			-no-reduce-relocations \
			-no-use-gold-linker \
			-no-xcb \
			-no-eglfs \
			-no-directfb \
			-linuxfb \
			-no-kms \
			-qpa minimal \
			-xplatform linux-openwrt-g++ \
			-no-opengl \
			-no-system-proxies \
			-no-warnings-are-errors \
	)
endef

define Build/InstallDev
	$(INSTALL_DIR) \
		$(STAGING_DIR)/host/mk

	$(CP) \
		./files/qmake.mk \
		$(STAGING_DIR)/host/mk/

	$(call Build/Install/HostFiles,$(1))
	$(call Build/Install/Headers,$(1))
	$(call Build/Install/Libs,$(1),*)
endef

define Package/qt5base-concurrent/install
	$(call Build/Install/Libs,$(1),libQt5Concurrent)
endef

define Package/qt5base-core/install
	$(call Build/Install/Libs,$(1),libQt5Core)
endef

define Package/qt5base-gui/install
	$(call Build/Install/Libs,$(1),libQt5Gui)
endef

define Package/qt5base-network/install
	$(call Build/Install/Libs,$(1),libQt5Network)
endef

define Package/qt5base-printSupport/install
	$(call Build/Install/Libs,$(1),libQt5PrintSupport)
endef

define Package/qt5base-sql/install
	$(call Build/Install/Libs,$(1),libQt5Sql)
endef

define Package/qt5base-test/install
	$(call Build/Install/Libs,$(1),libQt5Test)
endef

define Package/qt5base-widgets/install
	$(call Build/Install/Libs,$(1),libQt5Widgets)
endef

define Package/qt5base-xml/install
	$(call Build/Install/Libs,$(1),libQt5Xml)
endef

define Package/qt5base-plugin-bearer-generic/install
	$(call Build/Install/Plugins,$(1),bearer,libqgenericbearer)
endef

define Package/qt5base-plugin-input-evdevkeyboard/install
	$(call Build/Install/Plugins,$(1),generic,libqevdevkeyboardplugin)
endef

define Package/qt5base-plugin-input-evdevmouse/install
	$(call Build/Install/Plugins,$(1),generic,libqevdevmouseplugin)
endef

define Package/qt5base-plugin-input-evdevtablet/install
	$(call Build/Install/Plugins,$(1),generic,libqevdevtabletplugin)
endef

define Package/qt5base-plugin-input-evdevtouch/install
	$(call Build/Install/Plugins,$(1),generic,libqevdevtouchplugin)
endef

define Package/qt5base-plugin-imageformats-gif/install
	$(call Build/Install/Plugins,$(1),imageformats,libqgif)
endef

define Package/qt5base-plugin-imageformats-ico/install
	$(call Build/Install/Plugins,$(1),imageformats,libqico)
endef

define Package/qt5base-plugin-imageformats-jpeg/install
	$(call Build/Install/Plugins,$(1),imageformats,libqjpeg)
endef

define Package/qt5base-plugin-platforms-linuxfb/install
	$(call Build/Install/Plugins,$(1),platforms,libqlinuxfb)
endef

define Package/qt5base-plugin-platforms-minimal/install
	$(call Build/Install/Plugins,$(1),platforms,libqminimal)
endef

define Package/qt5base-plugin-platforms-offscreen/install
	$(call Build/Install/Plugins,$(1),platforms,libqoffscreen)
endef

define Package/qt5base-plugin-sqldrivers/install
	$(call Build/Install/Plugins,$(1),sqldrivers,libqsqlite)
endef

define Package/qt5base-examples/install
	$(call Build/Install/Examples,$(1))
endef

$(eval $(call BuildPackage,qt5base))
$(eval $(call BuildPackage,qt5base-concurrent))
$(eval $(call BuildPackage,qt5base-core))
$(eval $(call BuildPackage,qt5base-gui))
$(eval $(call BuildPackage,qt5base-network))
$(eval $(call BuildPackage,qt5base-printSupport))
$(eval $(call BuildPackage,qt5base-sql))
$(eval $(call BuildPackage,qt5base-test))
$(eval $(call BuildPackage,qt5base-widgets))
$(eval $(call BuildPackage,qt5base-xml))
$(eval $(call BuildPackage,qt5base-plugin-bearer-generic))
$(eval $(call BuildPackage,qt5base-plugin-input-evdevkeyboard))
$(eval $(call BuildPackage,qt5base-plugin-input-evdevmouse))
$(eval $(call BuildPackage,qt5base-plugin-input-evdevtablet))
$(eval $(call BuildPackage,qt5base-plugin-input-evdevtouch))
$(eval $(call BuildPackage,qt5base-plugin-imageformats-gif))
$(eval $(call BuildPackage,qt5base-plugin-imageformats-ico))
$(eval $(call BuildPackage,qt5base-plugin-imageformats-jpeg))
$(eval $(call BuildPackage,qt5base-plugin-platforms-linuxfb))
$(eval $(call BuildPackage,qt5base-plugin-platforms-minimal))
$(eval $(call BuildPackage,qt5base-plugin-platforms-offscreen))
$(eval $(call BuildPackage,qt5base-plugin-sqldrivers-sqlite))
$(eval $(call BuildPackage,qt5base-examples))
