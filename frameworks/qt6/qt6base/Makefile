#
# Copyright (C) 2026 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

# some parts of the journey documented here:
# https://bugreports.qt.io/browse/QTBUG-136244
# https://bugreports.qt.io/browse/QTBUG-136582
# https://bugreports.qt.io/browse/QTBUG-136446

include $(TOPDIR)/rules.mk

PKG_NAME:=qt6base
PKG_RELEASE:=1
PKG_HASH:=c1800c2ea835801af04a05d4a32321d79a93954ee3ae2172bbeacf13d1f0598c

PKG_MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
PKG_LICENSE:=LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
# Qt's own licenses, followed by those of the bundled third-party code
# compiled into the target packages with the options below
PKG_LICENSE_FILES:= \
  LICENSES/LicenseRef-Qt-Commercial.txt \
  LICENSES/LGPL-3.0-only.txt \
  LICENSES/GPL-2.0-only.txt \
  LICENSES/GPL-3.0-only.txt \
  LICENSES/Apache-2.0.txt \
  LICENSES/BSD-2-Clause.txt \
  LICENSES/BSD-3-Clause.txt \
  LICENSES/BSD-4-Clause.txt \
  LICENSES/CC0-1.0.txt \
  LICENSES/FTL.txt \
  LICENSES/HPND.txt \
  LICENSES/Imlib2.txt \
  LICENSES/LicenseRef-ICC-License.txt \
  LICENSES/LicenseRef-SHA1-Public-Domain.txt \
  LICENSES/Linux-syscall-note.txt \
  LICENSES/MIT.txt \
  LICENSES/MPL-2.0.txt \
  LICENSES/Unicode-3.0.txt \
  LICENSES/X11.txt

QT_MODULE:=qtbase

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include ../cmake.mk

PKG_CONFIG_DEPENDS:= \
  CONFIG_BUILD_qt6base-core_INOTIFY \
  CONFIG_BUILD_qt6base-gui_DRM \
  CONFIG_BUILD_qt6base-gui_ICU \
  CONFIG_BUILD_qt6base-gui_OPENGL_NONE \
  CONFIG_BUILD_qt6base-gui_OPENGL_OPENGLES2 \
  CONFIG_BUILD_qt6base-gui_QPA_DEFAULT_EGLFS \
  CONFIG_BUILD_qt6base-gui_QPA_DEFAULT_LINUXFB \
  CONFIG_BUILD_qt6base-gui_QPA_DEFAULT_VNC \
  CONFIG_BUILD_qt6base-gui_QPA_DEFAULT_OFFSCREEN \
  CONFIG_BUILD_qt6base-gui_QPA_DEFAULT_MINIMAL \
  CONFIG_BUILD_qt6base-gui_UDEV \
  CONFIG_BUILD_qt6base-network_SSL \
  CONFIG_PACKAGE_qt6base-gui \
  CONFIG_PACKAGE_qt6base-widgets \
  CONFIG_PACKAGE_qt6base-plugin-platforms-linuxfb \
  CONFIG_PACKAGE_qt6base-plugin-platforms-vnc \
  CONFIG_PACKAGE_qt6base-plugin-input-libinput

HOST_BUILD_DEPENDS:=pcre2/host libdouble-conversion/host
# Forced features fail configure if their library is not staged:
# FEATURE_libudev is forced on whenever UDEV is enabled (independent of the
# gui module), FEATURE_kms whenever DRM is enabled. Neither is guaranteed
# through the DEPENDS of the selected subpackages.
PKG_BUILD_DEPENDS:=qt6base/host BUILD_qt6base-gui_UDEV:libudev-zero BUILD_qt6base-gui_DRM:libdrm

# KMS-based EGLFS integrations require both OpenGL ES and DRM
QT6BASE_EGLFS_KMS:=$(and $(CONFIG_BUILD_qt6base-gui_OPENGL_OPENGLES2),$(CONFIG_BUILD_qt6base-gui_DRM))

QT6BASE_QPA_DEFAULT_PLATFORM:=$(strip \
  $(if $(CONFIG_BUILD_qt6base-gui_QPA_DEFAULT_EGLFS),eglfs) \
  $(if $(CONFIG_BUILD_qt6base-gui_QPA_DEFAULT_LINUXFB),linuxfb) \
  $(if $(CONFIG_BUILD_qt6base-gui_QPA_DEFAULT_VNC),vnc) \
  $(if $(CONFIG_BUILD_qt6base-gui_QPA_DEFAULT_OFFSCREEN),offscreen) \
  $(if $(CONFIG_BUILD_qt6base-gui_QPA_DEFAULT_MINIMAL),minimal))

define Package/qt6base/Default
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Qt6base
  DEPENDS:=
  URL:=https://www.qt.io
endef

define Package/qt6base-concurrent
  $(call Package/qt6base/Default)
  TITLE+=concurrent
  DEPENDS+=+qt6base-core
endef

define Package/qt6base-core
  $(call Package/qt6base/Default)
  TITLE+=core
  DEPENDS+=+zlib +libzstd +libstdcpp +libpcre2-16 +libdouble-conversion +BUILD_qt6base-gui_ICU:icu

endef

define Package/qt6base-core/config
        source "$(SOURCE)/Config_qt6base-core.in"
endef

define Package/qt6base-gui
  $(call Package/qt6base/Default)
  TITLE+=gui
  DEPENDS+=+qt6base-core +libpng +libfreetype +BUILD_qt6base-gui_OPENGL_OPENGLES2:libmesa # +BUILD_qt6base-gui_OPENGL_OPENGLES2:qt6base-widgets # qt6base-widgets appears to be needed when compiled with gl support, otherwise unneeded

endef

define Package/qt6base-gui/config
        source "$(SOURCE)/Config_qt6base-gui.in"
endef

define Package/qt6base-network
  $(call Package/qt6base/Default)
  TITLE+=network
  DEPENDS+=+qt6base-core +BUILD_qt6base-network_SSL:libopenssl
endef

define Package/qt6base-network/config
        source "$(SOURCE)/Config_qt6base-network.in"
endef

#define Package/qt6base-printSupport
#  $(call Package/qt6base/Default)
#  TITLE+=print support
#  DEPENDS+=+qt6base-core +qt6base-gui +qt6base-widgets
#endef
#
#define Package/qt6base-sql
#  $(call Package/qt6base/Default)
#  TITLE+=sql
#  DEPENDS+=+qt6base-core
#endef

define Package/qt6base-test
  $(call Package/qt6base/Default)
  TITLE+=test
  DEPENDS+=+qt6base-core
endef

define Package/qt6base-widgets
  $(call Package/qt6base/Default)
  TITLE+=widgets
  DEPENDS+=+qt6base-gui
endef

define Package/qt6base-openglwidgets
  $(call Package/qt6base/Default)
  TITLE+=widgets
  DEPENDS+=+qt6base-opengl +qt6base-widgets @BUILD_qt6base-gui_OPENGL_OPENGLES2
endef

define Package/qt6base-xml
  $(call Package/qt6base/Default)
  TITLE+=xml
  DEPENDS+=+qt6base-core
endef

define Package/qt6base-opengl
  $(call Package/qt6base/Default)
  TITLE+=opengl
  DEPENDS+=+qt6base-gui +libmesa @BUILD_qt6base-gui_OPENGL_OPENGLES2
endef

#### plugins

####### bearer

define Package/qt6base-plugin-bearer-generic
  $(call Package/qt6base/Default)
  TITLE+=bearer (plugin)
  DEPENDS+=+qt6base-core +qt6base-network
endef

####### generic / input

define Package/qt6base-plugin-input-libinput
  $(call Package/qt6base/Default)
  TITLE+=libinput (plugin)
  DEPENDS+=+qt6base-core +qt6base-gui +libinput
endef

define Package/qt6base-plugin-input-evdevkeyboard
  $(call Package/qt6base/Default)
  TITLE+=evdev keyboard (plugin)
  DEPENDS+=+qt6base-core +qt6base-gui +BUILD_qt6base-gui_UDEV:libudev +PACKAGE_qt6base-plugin-input-libinput:libinput
endef

define Package/qt6base-plugin-input-evdevmouse
  $(call Package/qt6base/Default)
  TITLE+=evdev mouse (plugin)
  DEPENDS+=+qt6base-core +qt6base-gui +BUILD_qt6base-gui_UDEV:libudev +PACKAGE_qt6base-plugin-input-libinput:libinput
endef

define Package/qt6base-plugin-input-evdevtablet
  $(call Package/qt6base/Default)
  TITLE+=evdev tablet (plugin)
  DEPENDS+=+qt6base-core +qt6base-gui +BUILD_qt6base-gui_UDEV:libudev +PACKAGE_qt6base-plugin-input-libinput:libinput
endef

define Package/qt6base-plugin-input-evdevtouch
  $(call Package/qt6base/Default)
  TITLE+=evdev touch (plugin)
  DEPENDS+=+qt6base-core +qt6base-gui +BUILD_qt6base-gui_UDEV:libudev +PACKAGE_qt6base-plugin-input-libinput:libinput
endef

####### imageformats

#define Package/qt6base-plugin-imageformats-gif
#  $(call Package/qt6base/Default)
#TITLE+=imageformat gif (plugin)
#  DEPENDS+=+qt6base-core +qt6base-gui
#endef

define Package/qt6base-plugin-imageformats-ico
  $(call Package/qt6base/Default)
  TITLE+=imageformat ico (plugin)
  DEPENDS+=+qt6base-core +qt6base-gui
endef

define Package/qt6base-plugin-imageformats-jpeg
  $(call Package/qt6base/Default)
  TITLE+=imageformat jpeg (plugin)
  DEPENDS+=+qt6base-core +qt6base-gui +libjpeg
endef

####### platforms

define Package/qt6base-plugin-platforms-minimal
  $(call Package/qt6base/Default)
  TITLE+=platform minimal (plugin)
  DEPENDS+=+qt6base-gui
endef

define Package/qt6base-plugin-platforms-offscreen
  $(call Package/qt6base/Default)
  TITLE+=platform offscreen (plugin)
  DEPENDS+=+qt6base-gui
endef

define Package/qt6base-plugin-platforms-linuxfb
  $(call Package/qt6base/Default)
  TITLE+=platform linuxfb (plugin)
  DEPENDS+=+qt6base-core +qt6base-gui +BUILD_qt6base-gui_UDEV:libudev +BUILD_qt6base-gui_DRM:libdrm +PACKAGE_qt6base-plugin-input-libinput:libinput
endef

define Package/qt6base-plugin-platforms-vnc
  $(call Package/qt6base/Default)
  TITLE+=platform vnc (plugin)
  DEPENDS+=+qt6base-core +qt6base-gui +qt6base-network +libfreetype +BUILD_qt6base-gui_UDEV:libudev +PACKAGE_qt6base-plugin-input-libinput:libinput
endef

define Package/qt6base-plugin-platforms-minimalegl
  $(call Package/qt6base/Default)
  TITLE+=platform minimalegl (plugin)
  DEPENDS+=+qt6base-gui +qt6base-opengl
endef

define Package/qt6base-plugin-platforms-eglfs
  $(call Package/qt6base/Default)
  TITLE+=platform eglfs (plugin)
  DEPENDS+=+qt6base-gui +qt6base-opengl +BUILD_qt6base-gui_UDEV:libudev +BUILD_qt6base-gui_DRM:libdrm
endef


###### sql

#define Package/qt6base-plugin-sqldrivers-sqlite
#  $(call Package/qt6base/Default)
#  TITLE+=sqldriver sqlite (plugin)
#  DEPENDS+=+qt6base-core +qt6base-sql +libsqlite3
#endef

define Package/qt6base-examples
  $(call Package/qt6base/Default)
  TITLE+=examples
  DEPENDS+=\
	+qt6base-core \
	+qt6base-gui \
	+qt6base-widgets \
	+qt6base-network \
	+qt6base-sql \
	+qt6base-xml \
	+qt6base-printSupport \
	+qt6base-concurrent \
	+qt6base-gl
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	# building qt6base for the target requires mkspecs in the target staging-dir, see:
	# https://bugreports.qt.io/browse/QTBUG-136244
	$(CP) \
	  ./files/mkspecs/* \
	  $(PKG_BUILD_DIR)/mkspecs/
endef

define Host/Prepare
	$(call Host/Prepare/Default)
	# qmake6 looks for mkspecs inside QT_HOST_DATA / HostData - even for target builds
	$(CP) \
	  ./files/mkspecs/* \
	  $(HOST_BUILD_DIR)/mkspecs/
endef

# mkspecs are not actually used/read when compiling via cmake,
# but at least the respective mkspec-dir still needs to be present.
# cmake-qt.mk defines linux-openwrt-g++ as mkspec, which eventually
# needs to be present in the *target*-staging-dir for other modules
# to find and include it.
# qt6base itself, though, expects all custom mkspecs to present in
# its own source dir.
CMAKE_OPTIONS += \
	-DINSTALL_PREFIX=$(QT_INSTALL_PREFIX) \
	-DINSTALL_ARCHDATADIR=$(QT_INSTALL_ARCHDATA) \
	-DINSTALL_BINDIR=$(QT_INSTALL_BINS) \
	-DINSTALL_DATADIR=$(QT_INSTALL_DATA) \
	-DINSTALL_DOCDIR=$(QT_INSTALL_DOCS) \
	-DINSTALL_EXAMPLESDIR=$(QT_INSTALL_EXAMPLES) \
	-DINSTALL_INCLUDEDIR=$(QT_INSTALL_HEADERS) \
	-DINSTALL_LIBDIR=$(QT_INSTALL_LIBS) \
	-DINSTALL_LIBEXECDIR=$(QT_INSTALL_LIBEXECS) \
	-DINSTALL_PLUGINSDIR=$(QT_INSTALL_PLUGINS) \
	-DINSTALL_QMLDIR=$(QT_INSTALL_QML) \
	-DINSTALL_SYSCONFDIR=$(QT_INSTALL_SYSCONF) \
	-DINSTALL_TESTSDIR=$(QT_INSTALL_TESTS) \
	-DINSTALL_TRANSLATIONDIR=$(QT_INSTALL_TRANSLATIONS) \
	-DINSTALL_MKSPECSDIR=$(QT_INSTALL_MKSPECS) \
	-DQT_BUILD_TESTS_PROJECT_all=OFF \
	-DQT_BUILD_EXAMPLES_PROJECT_all=OFF \
	-DBUILD_WITH_PCH=OFF \
	-DQT_USE_CCACHE=OFF \
	-DQT_USE_VCPKG=OFF \
	-DQT_BUILD_EXAMPLES=FALSE \
	-DQT_BUILD_TESTS=FALSE \
	-DFEATURE_optimize_size=ON \
	-DFEATURE_reduce_exports=OFF \
	-DFEATURE_reduce_relocations=OFF \
	-DFEATURE_rpath=OFF \
	-DQT_GENERATE_SBOM=OFF \
	-DFEATURE_intelcet=OFF \
	-DFEATURE_stack_protector=OFF \
	-DFEATURE_stack_clash_protection=OFF \
	-DFEATURE_glibc_fortify_source=OFF \
	-DFEATURE_libstdcpp_assertions=OFF \
	-DINPUT_linker=bfd \
	-DFEATURE_pkg_config=ON \
	-DFEATURE_gui=$(if $(CONFIG_PACKAGE_qt6base-gui),ON,OFF) \
	-DFEATURE_widgets=$(if $(CONFIG_PACKAGE_qt6base-widgets),ON,OFF) \
	-DFEATURE_dbus=OFF \
	-DFEATURE_accessibility=ON \
	-DINPUT_doubleconversion=system \
	-DFEATURE_glib=OFF \
	-DFEATURE_inotify=$(if $(CONFIG_BUILD_qt6base-core_INOTIFY),ON,OFF) \
	-DFEATURE_icu=$(if $(CONFIG_BUILD_qt6base-gui_ICU),ON,OFF) \
	-DINPUT_pcre=system \
	-DFEATURE_system_zlib=ON \
	-DFEATURE_journald=OFF \
	-DFEATURE_syslog=OFF \
	-DFEATURE_ssl=$(if $(CONFIG_BUILD_qt6base-network_SSL),ON,OFF) \
	-DINPUT_openssl=$(if $(CONFIG_BUILD_qt6base-network_SSL),linked,no) \
	-DFEATURE_sctp=OFF \
	-DFEATURE_libproxy=OFF \
	-DFEATURE_gssapi=OFF \
	-DFEATURE_system_proxies=ON \
	-DFEATURE_cups=OFF \
	-DFEATURE_emojisegmenter=OFF \
	-DFEATURE_fontconfig=OFF \
	-DINPUT_freetype=system \
	-DFEATURE_harfbuzz=OFF \
	-DFEATURE_gtk3=OFF \
	-DINPUT_opengl=$(if $(CONFIG_BUILD_qt6base-gui_OPENGL_OPENGLES2),es2,no) \
	-DFEATURE_egl=$(if $(CONFIG_BUILD_qt6base-gui_OPENGL_OPENGLES2),ON,OFF) \
	-DFEATURE_vulkan=OFF \
	-DQT_QPA_PLATFORMS=$(QT6BASE_QPA_DEFAULT_PLATFORM) \
	-DQT_QPA_DEFAULT_PLATFORM=$(QT6BASE_QPA_DEFAULT_PLATFORM) \
	-DFEATURE_directfb=OFF \
	-DFEATURE_eglfs=$(if $(CONFIG_BUILD_qt6base-gui_OPENGL_OPENGLES2),ON,OFF) \
	-DFEATURE_eglfs_gbm=$(if $(QT6BASE_EGLFS_KMS),ON,OFF) \
	-DFEATURE_eglfs_egldevice=$(if $(QT6BASE_EGLFS_KMS),ON,OFF) \
	-DFEATURE_gbm=$(if $(QT6BASE_EGLFS_KMS),ON,OFF) \
	-DFEATURE_kms=$(if $(CONFIG_BUILD_qt6base-gui_DRM),ON,OFF) \
	-DFEATURE_linuxfb=$(if $(CONFIG_PACKAGE_qt6base-plugin-platforms-linuxfb),ON,OFF) \
	-DFEATURE_vnc=$(if $(CONFIG_PACKAGE_qt6base-plugin-platforms-vnc),ON,OFF) \
	-DFEATURE_wayland=OFF \
	-DFEATURE_xcb=OFF \
	-DFEATURE_libudev=$(if $(CONFIG_BUILD_qt6base-gui_UDEV),ON,OFF) \
	-DFEATURE_evdev=ON \
	-DFEATURE_libinput=$(if $(CONFIG_PACKAGE_qt6base-plugin-input-libinput),ON,OFF) \
	-DFEATURE_mtdev=OFF \
	-DFEATURE_tslib=OFF \
	-DFEATURE_xkbcommon=OFF \
	-DFEATURE_gif=ON \
	-DFEATURE_ico=OFF \
	-DINPUT_libpng=system \
	-DINPUT_libjpeg=system \
	-DFEATURE_sql=OFF \
	-DFEATURE_sqlmodel=OFF


#HOST_LDFLAGS += -L$(HOST_BUILD_DIR) # otherwise local executables can't find local shared objects

# BUILD_SHARED_LIBS=ON needed for host in order for qtdeclarative/host building correctly with BUILD_SHARED_LIBS=ON which is needed for qtshaders/host building the actual host tools (as bin/qsb, which it doesn't if built statically), eventually needed for building qtdeclarative for the target
# FEATURE_gui=ON for host build required in order to build qt6shadertools for host
# CMAKE_SKIP_RPATH=FALSE otherwise host tools won't be able to load shared objects not in system paths (in this case, build_dir/qtbase-*/lib/Qt6Core.so)
#
# files/dirs within most install-paths are properly versioned and can be distinguished among versions (e.g. qt5 and qt6). some are not, though, and will bet set to version specifics explicitly
CMAKE_HOST_OPTIONS += \
	-DINSTALL_PREFIX=$(QT_HOSTPKG_PREFIX) \
	-DINSTALL_ARCHDATADIR=$(QT_HOSTPKG_ARCHDATA) \
	-DINSTALL_BINDIR=$(QT_HOSTPKG_BINS) \
	-DINSTALL_DATADIR=$(QT_HOSTPKG_DATA) \
	-DINSTALL_DOCDIR=$(QT_HOSTPKG_DOCS) \
	-DINSTALL_EXAMPLESDIR=$(QT_HOSTPKG_EXAMPLES) \
	-DINSTALL_INCLUDEDIR=$(QT_HOSTPKG_HEADERS) \
	-DINSTALL_LIBDIR=$(QT_HOSTPKG_LIBS) \
	-DINSTALL_LIBEXECDIR=$(QT_HOSTPKG_LIBEXECS) \
	-DINSTALL_PLUGINSDIR=$(QT_HOSTPKG_PLUGINS) \
	-DINSTALL_QMLDIR=$(QT_HOSTPKG_QML) \
	-DINSTALL_SYSCONFDIR=$(QT_HOSTPKG_SYSCONF) \
	-DINSTALL_TESTSDIR=$(QT_HOSTPKG_TESTS) \
	-DINSTALL_TRANSLATIONDIR=$(QT_HOSTPKG_TRANSLATIONS) \
	-DINSTALL_MKSPECSDIR=$(QT_HOSTPKG_MKSPECS) \
	-DCMAKE_SKIP_RPATH=FALSE  \
	-DQT_BUILD_TESTS_PROJECT_all=OFF \
	-DQT_BUILD_EXAMPLES_PROJECT_all=OFF \
	-DBUILD_WITH_PCH=ON \
	-DQT_USE_CCACHE=OFF \
	-DQT_USE_VCPKG=OFF \
	-DQT_BUILD_EXAMPLES=FALSE \
	-DQT_BUILD_TESTS=FALSE \
	-DFEATURE_optimize_size=ON \
	-DFEATURE_reduce_exports=OFF \
	-DFEATURE_reduce_relocations=OFF \
	-DQT_GENERATE_SBOM=OFF \
	-DFEATURE_intelcet=OFF \
	-DFEATURE_stack_protector=OFF \
	-DFEATURE_stack_clash_protection=OFF \
	-DFEATURE_libstdcpp_assertions=OFF \
	-DINPUT_linker=bfd \
	-DFEATURE_pkg_config=ON \
	-DFEATURE_gui=ON \
	-DFEATURE_widgets=OFF \
	-DFEATURE_dbus=OFF \
	-DFEATURE_accessibility=OFF \
	-DINPUT_doubleconversion=system \
	-DFEATURE_glib=OFF \
	-DFEATURE_inotify=OFF \
	-DFEATURE_icu=OFF \
	-DINPUT_pcre=system \
	-DFEATURE_system_zlib=ON \
	-DFEATURE_journald=OFF \
	-DFEATURE_syslog=OFF \
	-DFEATURE_ssl=OFF \
	-DINPUT_openssl=no \
	-DFEATURE_sctp=OFF \
	-DFEATURE_libproxy=OFF \
	-DFEATURE_system_proxies=ON \
	-DFEATURE_cups=OFF \
	-DFEATURE_emojisegmenter=OFF \
	-DFEATURE_fontconfig=OFF \
	-DFEATURE_freetype=OFF \
	-DFEATURE_harfbuzz=OFF \
	-DFEATURE_gtk3=OFF \
	-DINPUT_opengl=no \
	-DFEATURE_egl=OFF \
	-DFEATURE_directfb=OFF \
	-DFEATURE_eglfs=OFF \
	-DFEATURE_gbm=OFF \
	-DFEATURE_kms=OFF \
	-DFEATURE_linuxfb=OFF \
	-DFEATURE_xcb=OFF \
	-DFEATURE_libudev=OFF \
	-DFEATURE_evdev=ON \
	-DFEATURE_libinput=OFF \
	-DFEATURE_mtdev=OFF \
	-DFEATURE_tslib=OFF \
	-DFEATURE_xkbcommon=OFF \
	-DFEATURE_gif=OFF \
	-DFEATURE_ico=OFF \
	-DINPUT_libpng=no \
	-DINPUT_libjpeg=no \
	-DFEATURE_sql=OFF \
	-DFEATURE_sqlmodel=OFF

#define Host/Install
#	$(call Host/Install/Default)
#
#	$(INSTALL_DIR) \
#	  $(QT_HOST_DATA)/mk
#
#	$(CP) \
#	  ./files/qt.mk \
#	  $(QT_HOST_DATA)/mk/qt.mk
#
#	$(CP) \
#	  ./files/qmake.mk \
#	  $(QT_HOST_DATA)/mk/qmake.mk
#
#	$(CP) \
#	  ./files/cmake.mk \
#	  $(QT_HOST_DATA)/mk/cmake.mk
#endef

define Build/InstallDev
	$(call Build/InstallDev/Qt,$(1))

	# target specific host tool `qmake`
	$(INSTALL_DIR) \
	  $(1)/host/etc/qt6 \
	  $(1)/host/$(subst $(QT_HOST_PREFIX),,$(QT_HOST_BINS)) \
	  $(1)/host/$(subst $(QT_HOST_PREFIX),,$(QT_HOST_LIBS)) \
	  $(1)/host/$(subst $(QT_HOST_PREFIX),,$(QT_HOST_LIBEXECS)) \
	  $(1)/host/$(subst $(QT_HOST_PREFIX),,$(QT_HOST_DATA)) \

	# while there were 2 qmakes in qt5, one for the host, one for the target,
	# it's the same binary but with different configs for qt6.
	# the config is generated at build time and contains relative paths to its own location
	# which assumptions are poor and don't allow for moving around.
	# so we generate the qmake6 wrapper for the target and modify the config ourselves.
	# (Sub)paths are computed based on Prefix and HostPrefix and derived from config/cmake
	# INSTALL_*DIR-args.
	# HostData is where qmake reads the mkspecs, qconfig.pri and the module
	# .pri files from. For target builds it has to be the target's ArchData,
	# otherwise qmake uses the configuration and modules of the host build.
	# HostSpec makes host parts (host_build) of target builds use OpenWrt's
	# host compiler and flags.
	sed \
	  -e 's#\(^Prefix=\).*#\1$(STAGING_DIR)/$(QT_INSTALL_PREFIX)#' \
	  -e 's#\(^HostPrefix=\).*#\1$(QT_HOST_PREFIX)#' \
	  -e 's#\(^HostBinaries=\).*#\1$(subst $(QT_HOST_PREFIX)/,,$(QT_HOST_BINS))#' \
	  -e 's#\(^HostLibraries=\).*#\1$(subst $(QT_HOST_PREFIX)/,,$(QT_HOST_LIBS))#' \
	  -e 's#\(^HostLibraryExecutables=\).*#\1$(subst $(QT_HOST_PREFIX)/,,$(QT_HOST_LIBEXECS))#' \
	  -e 's#\(^HostData=\).*#\1$(STAGING_DIR)$(QT_INSTALL_ARCHDATA)#' \
	  -e 's#\(^HostSpec=\).*#\1linux-openwrt-host-g++#' \
	  $(PKG_INSTALL_DIR)/$(QT_INSTALL_BINS)/target_qt.conf \
	  > $(1)/host/etc/qt6/qmake-cross.conf

	printf '%s\n' \
	  '#!/bin/sh' \
	  'exec $(QT_HOSTPKG_BINS)/qmake6 -qtconf "$(STAGING_DIR)/host/etc/qt6/qmake-cross.conf" "$$$$@"' \
	  > $(1)/host/bin/qt6/qmake
	chmod +x $(1)/host/bin/qt6/qmake
endef

define Package/qt6base-concurrent/install
	$(call Build/Install/Libs,$(1),libQt6Concurrent)
endef

define Package/qt6base-core/install
	$(call Build/Install/Libs,$(1),libQt6Core)
endef

define Package/qt6base-gui/install
	$(call Build/Install/Libs,$(1),libQt6Gui)
endef

define Package/qt6base-network/install
	$(call Build/Install/Libs,$(1),libQt6Network)
	$(if $(CONFIG_BUILD_qt6base-network_SSL),$(call Build/Install/Plugins,$(1),tls,*),)
endef

define Package/qt6base-printSupport/install
	$(call Build/Install/Libs,$(1),libQt6PrintSupport)
endef

define Package/qt6base-sql/install
	$(call Build/Install/Libs,$(1),libQt6Sql)
endef

define Package/qt6base-test/install
	$(call Build/Install/Libs,$(1),libQt6Test)
endef

define Package/qt6base-widgets/install
	$(call Build/Install/Libs,$(1),libQt6Widgets)
endef

define Package/qt6base-openglwidgets/install
	$(call Build/Install/Libs,$(1),libQt6OpenGLWidgets)
endef

define Package/qt6base-xml/install
	$(call Build/Install/Libs,$(1),libQt6Xml)
endef

define Package/qt6base-opengl/install
	$(call Build/Install/Libs,$(1),libQt6OpenGL)
endef

define Package/qt6base-plugin-platforms-eglfs/install
	$(call Build/Install/Libs,$(1),libQt6EglFSDeviceIntegration)
	$(call Build/Install/Plugins,$(1),platforms,libqeglfs)
	$(if $(QT6BASE_EGLFS_KMS),$(call Build/Install/Libs,$(1),libQt6EglFsKmsSupport))
	$(if $(QT6BASE_EGLFS_KMS),$(call Build/Install/Libs,$(1),libQt6EglFsKmsGbmSupport))
	$(if $(QT6BASE_EGLFS_KMS),$(call Build/Install/Plugins,$(1),egldeviceintegrations,libqeglfs-kms-egldevice-integration))
	$(if $(QT6BASE_EGLFS_KMS),$(call Build/Install/Plugins,$(1),egldeviceintegrations,libqeglfs-kms-integration))
endef

define Package/qt6base-plugin-bearer-generic/install
	$(call Build/Install/Plugins,$(1),bearer,libqgenericbearer)
endef

define Package/qt6base-plugin-input-libinput/install
	$(call Build/Install/Plugins,$(1),generic,libqlibinputplugin)
endef

define Package/qt6base-plugin-input-evdevkeyboard/install
	$(call Build/Install/Plugins,$(1),generic,libqevdevkeyboardplugin)
endef

define Package/qt6base-plugin-input-evdevmouse/install
	$(call Build/Install/Plugins,$(1),generic,libqevdevmouseplugin)
endef

define Package/qt6base-plugin-input-evdevtablet/install
	$(call Build/Install/Plugins,$(1),generic,libqevdevtabletplugin)
endef

define Package/qt6base-plugin-input-evdevtouch/install
	$(call Build/Install/Plugins,$(1),generic,libqevdevtouchplugin)
endef

#define Package/qt6base-plugin-imageformats-gif/install
#	$(call Build/Install/Plugins,$(1),imageformats,libqgif)
#endef

define Package/qt6base-plugin-imageformats-ico/install
	$(call Build/Install/Plugins,$(1),imageformats,libqico)
endef

define Package/qt6base-plugin-imageformats-jpeg/install
	$(call Build/Install/Plugins,$(1),imageformats,libqjpeg)
endef

define Package/qt6base-plugin-platforms-minimal/install
	$(call Build/Install/Plugins,$(1),platforms,libqminimal)
endef

define Package/qt6base-plugin-platforms-linuxfb/install
	$(call Build/Install/Plugins,$(1),platforms,libqlinuxfb)
endef

define Package/qt6base-plugin-platforms-offscreen/install
	$(call Build/Install/Plugins,$(1),platforms,libqoffscreen)
endef

define Package/qt6base-plugin-platforms-vnc/install
	$(call Build/Install/Plugins,$(1),platforms,libqvnc)
endef

define Package/qt6base-plugin-platforms-minimalegl/install
	$(call Build/Install/Plugins,$(1),platforms,libqminimalegl)
endef


define Package/qt6base-examples/install
	$(call Build/Install/Examples,$(1))
endef

# $(eval $(call BuildPackage,qt6base))
$(eval $(call BuildPackage,qt6base-concurrent))
$(eval $(call BuildPackage,qt6base-core))
$(eval $(call BuildPackage,qt6base-gui))
$(eval $(call BuildPackage,qt6base-opengl))
$(eval $(call BuildPackage,qt6base-network))
#$(eval $(call BuildPackage,qt6base-printSupport))
#$(eval $(call BuildPackage,qt6base-sql))
$(eval $(call BuildPackage,qt6base-test))
$(eval $(call BuildPackage,qt6base-widgets))
#$(eval $(call BuildPackage,qt6base-openglwidgets))
$(eval $(call BuildPackage,qt6base-xml))
#$(eval $(call BuildPackage,qt6base-plugin-bearer-generic))
#$(eval $(call BuildPackage,qt6base-plugin-input-libinput))
$(eval $(call BuildPackage,qt6base-plugin-input-evdevkeyboard))
$(eval $(call BuildPackage,qt6base-plugin-input-evdevmouse))
$(eval $(call BuildPackage,qt6base-plugin-input-evdevtablet))
$(eval $(call BuildPackage,qt6base-plugin-input-evdevtouch))
##$(eval $(call BuildPackage,qt6base-plugin-imageformats-gif))
#$(eval $(call BuildPackage,qt6base-plugin-imageformats-ico))
$(eval $(call BuildPackage,qt6base-plugin-imageformats-jpeg))
$(eval $(call BuildPackage,qt6base-plugin-platforms-minimal))
$(eval $(call BuildPackage,qt6base-plugin-platforms-linuxfb))
$(eval $(call BuildPackage,qt6base-plugin-platforms-offscreen))
$(eval $(call BuildPackage,qt6base-plugin-platforms-vnc))
$(eval $(call BuildPackage,qt6base-plugin-platforms-eglfs))
#$(eval $(call BuildPackage,qt6base-plugin-platforms-minimalegl))
#$(eval $(call BuildPackage,qt6base-examples))
$(eval $(call HostBuild))
