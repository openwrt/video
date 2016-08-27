#
# Copyright (C) 2015 OpenWrt.org
# Author: Mirko Vogt <mirko@openwrt.org>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

# qmake - oh my.. qmake is supposed to generate Makefiles suitable for cross-compiling
# however fails itself hard being used in a cross compiling toolchain in any sane way.
# 
# There are the QT_INSTALL_* variables - which get set via Qt's configure options,
# containing paths which become hardcoded into the qmake binary.
# Those paths are supposed to refer to the target system, however are also used for
# include and linker paths.
# Hence, setting QT_INSTALL_PREFIX=/usr would result in -I/usr/include,
# -L/usr/lib, etc., referencing the host headers and libraries.
# The QT_SYSROOT variable looks most promising for distinguishing between
# host and target specific paths, however it fails hard and is totally undocumented.
# The extprefix variable tries to cover the situation, however actually just prepends
# its path to the QT_INSTALL_* variables - basically cosmetics.
# 
# The QT_HOST_* variables are used for host tools, libraries, mkspecs and its data.
# 
# As a consequence we set QT_INSTALL_* and QT_HOST_* to absolute paths, which
# inevitably results in the following issues:
# 
#  - 'make install' results in paths like:
#    /tmp/install_root/home/cross/openwrt/staging_dir/target-*/usr.
#    This is workarounded by overriding the PKG_INSTALL_DIR, so the Makefiles don't
#    have to care about that.
#  - Once compiled, qmake's location and its requirements (mkspecs, etc.) are fixed,
#    since its absolute paths were hardcoded. No moving around of the toolchain.
#  - Those variables might be used for target binaries for some weird reason, so
#    paths to the host staging_dir would make it to the target, logically leading to
#    errors.
#  - Paths might make it into target binaries, thus referencing non-existing
#    objects on the target platform. Tihs behaviour wasn't observed so far, however 
#    one might use the QT_INSTALL_* variables for some weird reason during runtime.


QT_EXTPREFIX:=$(STAGING_DIR)/$(CONFIGURE_PREFIX)
QT_SYSROOT:=
QT_INSTALL_CONFIGURATION:=/etc/qt5
QT_INSTALL_PREFIX:=$(CONFIGURE_PREFIX)
QT_INSTALL_LIBS:=$(QT_INSTALL_PREFIX)/lib
QT_INSTALL_DATA:=$(QT_INSTALL_PREFIX)/share/qt5
QT_INSTALL_HEADERS:=$(QT_INSTALL_PREFIX)/include
QT_INSTALL_BINS:=$(QT_INSTALL_PREFIX)/bin
QT_INSTALL_DOCS:=$(QT_INSTALL_DATA)/doc
QT_INSTALL_TRANSLATIONS:=$(QT_INSTALL_DATA)/translations
QT_INSTALL_ARCHDATA:=$(QT_INSTALL_LIBS)/qt5
QT_INSTALL_LIBEXECS:=$(QT_INSTALL_ARCHDATA)
QT_INSTALL_TESTS:=$(QT_INSTALL_ARCHDATA)/tests
QT_INSTALL_PLUGINS:=$(QT_INSTALL_ARCHDATA)/plugins
QT_INSTALL_IMPORTS:=$(QT_INSTALL_ARCHDATA)/imports
QT_INSTALL_QML:=$(QT_INSTALL_ARCHDATA)/qml
QT_INSTALL_EXAMPLES:=$(QT_INSTALL_ARCHDATA)/examples
QT_INSTALL_DEMOS:=$(QT_INSTALL_EXAMPLES)
QT_HOST_EXTPREFIX:=$(STAGING_DIR)/host
QT_HOST_PREFIX:=$(QT_HOST_EXTPREFIX)
QT_HOST_DATA:=$(QT_HOST_PREFIX)/share
QT_HOST_BINS:=$(QT_HOST_PREFIX)/bin
QT_HOST_LIBS:=$(QT_HOST_PREFIX)/lib

QMAKE_SPEC:=linux-g++
QMAKE_XSPEC:=linux-openwrt-g++

PKG_INSTALL_DIR_ROOT:=$(PKG_INSTALL_DIR)
PKG_INSTALL_DIR:=$(PKG_INSTALL_DIR_ROOT)/$(STAGING_DIR)

define Build/Configure/Default
	TARGET_CROSS="$(TARGET_CROSS)" \
	TARGET_CFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
	TARGET_CXXFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS)" \
	TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
	qmake \
		-o $(PKG_BUILD_DIR)/$(2)/Makefile \
		$(PKG_BUILD_DIR)/$(2)/$(if $(1),$(1),$(PKG_NAME)).pro
endef

# we need to pass everything to $(MAKE) as well, as Makefiles may invoke qmake once again for creating further Makefiles
define Build/Compile/Default
	+TARGET_CROSS="$(TARGET_CROSS)" \
	TARGET_CFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
	TARGET_CXXFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS)" \
	TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
		$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
			$(1)
endef

define Build/Install/Default
	INSTALL_ROOT="$(PKG_INSTALL_DIR_ROOT)" \
		$(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
			$(1) install
endef

define Build/Install/HostFiles
	$(INSTALL_DIR) \
		$(1)/host

	$(CP) \
		$(PKG_INSTALL_DIR)/host/* \
		$(1)/host/
endef

define Build/Install/Headers
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_HEADERS)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_HEADERS)/* \
		$(1)/$(QT_INSTALL_HEADERS)/
endef

define Build/Install/Libs
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_LIBS)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_LIBS)/$(2).so* \
		$(1)/$(QT_INSTALL_LIBS)/
endef

define Build/Install/Plugins
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_PLUGINS)/$(2)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_PLUGINS)/$(2)/$(3).so* \
		$(1)/$(QT_INSTALL_PLUGINS)/$(2)/
endef

define Build/Install/Examples
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_EXAMPLES)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_EXAMPLES)/* \
		$(1)/$(QT_INSTALL_EXAMPLES)/

	$(FIND) $(1)/usr/share/qt5/examples/ \
		-type f \( -name '*.cpp' -o -name '*.h' -o -name '*.pro' -o -name '*.pri' \) | \
		$(XARGS) $(RM) -vf
endef
