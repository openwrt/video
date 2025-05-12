#
# Copyright (C) 2020 OpenWrt.org
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
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

# for target builds (STAGING_DIR)
QT_INSTALL_PREFIX:=$(CONFIGURE_PREFIX)
QT_INSTALL_CONFIGURATION:=/etc/qt5
QT_INSTALL_LIBS:=$(QT_INSTALL_PREFIX)/lib
QT_INSTALL_DATA:=$(QT_INSTALL_PREFIX)/share/qt5
QT_INSTALL_HEADERS:=$(QT_INSTALL_PREFIX)/include/qt5
QT_INSTALL_CMAKES:=$(QT_INSTALL_PREFIX)/lib/cmake
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
# for host builds defined in target project files (STAGING_DIR)/host
QT_HOST_PREFIX:=$(STAGING_DIR)/host
QT_HOST_DATA:=$(QT_HOST_PREFIX)/share/qt5
QT_HOST_BINS:=$(QT_HOST_PREFIX)/bin/qt5
QT_HOST_LIBS:=$(QT_HOST_PREFIX)/lib
# for host builds defined in host project files (STAGING_DIR_HOST)
QT_HOSTPKG_PREFIX:=$(STAGING_DIR_HOST)
QT_HOSTPKG_CONFIGURATION:=$(STAGING_DIR_HOST)/etc/qt5
QT_HOSTPKG_LIBS:=$(QT_HOSTPKG_PREFIX)/lib
QT_HOSTPKG_DATA:=$(QT_HOSTPKG_PREFIX)/share/qt5
QT_HOSTPKG_HEADERS:=$(QT_HOSTPKG_PREFIX)/include/qt5
QT_HOSTPKG_CMAKES:=$(QT_HOSTPKG_PREFIX)/lib/cmake
QT_HOSTPKG_BINS:=$(QT_HOSTPKG_PREFIX)/bin/qt5
QT_HOSTPKG_DOCS:=$(QT_HOSTPKG_DATA)/doc
QT_HOSTPKG_TRANSLATIONS:=$(QT_HOSTPKG_DATA)/translations
QT_HOSTPKG_ARCHDATA:=$(QT_HOSTPKG_LIBS)/qt5
QT_HOSTPKG_LIBEXECS:=$(QT_HOSTPKG_ARCHDATA)/libexec
QT_HOSTPKG_TESTS:=$(QT_HOSTPKG_ARCHDATA)/tests
QT_HOSTPKG_PLUGINS:=$(QT_HOSTPKG_ARCHDATA)/plugins
QT_HOSTPKG_IMPORTS:=$(QT_HOSTPKG_ARCHDATA)/imports
QT_HOSTPKG_QML:=$(QT_HOSTPKG_ARCHDATA)/qml
QT_HOSTPKG_EXAMPLES:=$(QT_HOSTPKG_ARCHDATA)/examples
QT_HOSTPKG_DEMOS:=$(QT_HOSTPKG_EXAMPLES)

QMAKE_SPEC:=linux-g++
QMAKE_XSPEC:=linux-openwrt-g++

PKG_INSTALL_DIR_ROOT:=$(PKG_INSTALL_DIR)
PKG_INSTALL_DIR:=$(PKG_INSTALL_DIR_ROOT)/$(STAGING_DIR)

# for target independant host builds (STAGING_DIR_HOST)
HOST_INSTALL_DIR_ROOT:=$(HOST_INSTALL_DIR)
HOST_INSTALL_DIR:=$(HOST_INSTALL_DIR_ROOT)/$(STAGING_DIR_HOST)
#HOST_INSTALL_DIR:=$(HOST_INSTALL_DIR_ROOT)/$(STAGING_DIR)

# qmake host tool for target builds
QMAKE_TARGET=$(STAGING_DIR)/host/bin/qt5/qmake
# qmake host tool for host builds
QMAKE_HOST=$(STAGING_DIR_HOST)/bin/qt5/qmake


define Build/Configure/Default
	TARGET_CROSS="$(TARGET_CROSS)" \
	TARGET_CFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
	TARGET_CXXFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS)" \
	TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
	$(QMAKE_TARGET) \
		-o $(PKG_BUILD_DIR)/$(MAKE_PATH)/Makefile \
		$(PKG_BUILD_DIR)/$(MAKE_PATH)/$(if $(1),$(1).pro,)
endef

define Host/Configure/Default
	$(QMAKE_HOST) \
		-o $(HOST_BUILD_DIR)/$(MAKE_PATH)/Makefile \
		$(HOST_BUILD_DIR)/$(MAKE_PATH)/$(if $(1),$(1).pro,)
endef

# We need to pass all qmake (TARGET_*) related variables to $(MAKE) as well, as
# (generated) Makefiles may invoke qmake once again for creating further Makefiles.
# Actually we'd also like to pass all other vars (defined in $MAKE_VARS and
# $MAKE_FLAGS) to also make ordinary non-qmake generated Makefiles calling tool-
# chain executables like $CC/$CXX/$AR.. work, however this would interfere with
# qmake generated Makefiles, since they expect variables being set differently.
# For example qmake generated Makefiles expect $AR to also contain ar's arguments,
# while ordinary Makefiles don't.
# Until we find a way to disginguish both kinds of Makefiles, we will neglect
# ordinary Makefiles calling toolchain executables, however as they might take
# $CFLAGS/CXXFLAGS into account (e.g. flags as -D*), we pass at least those
# hoping to not interfere / break something.
# Mixing qmake generated and ordinary Makfiles - both calling toolchain execut-
# ables - is probably a very rare case anyway.
define Build/Compile/Default
	+TARGET_CROSS="$(TARGET_CROSS)" \
	TARGET_CFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
	TARGET_CXXFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS)" \
	TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
	CFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
	CXXFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)" \
		$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
			$(1)
endef

define Host/Compile/Default
		$(MAKE) $(PKG_JOBS) -C $(HOST_BUILD_DIR)/$(MAKE_PATH) \
			$(1)
endef

define Build/Install/Default
	INSTALL_ROOT="$(PKG_INSTALL_DIR_ROOT)" \
		$(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
			$(1) install
endef

define Host/Install/Default
	INSTALL_ROOT="$(HOST_INSTALL_DIR_ROOT)" \
		$(MAKE) -C $(HOST_BUILD_DIR)/$(MAKE_PATH) \
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

define Build/Install/Cmakes
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_CMAKES)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_CMAKES)/* \
		$(1)/$(QT_INSTALL_CMAKES)/
endef

define Build/Install/Translations
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_TRANSLATIONS)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_TRANSLATIONS)/$(2).qm \
		$(1)/$(QT_INSTALL_TRANSLATIONS)/
endef

define Build/Install/Plugins
	if [ "$(2)" = '*' ]; then \
		$(INSTALL_DIR) \
			$(1)/$(QT_INSTALL_PLUGINS) ; \
		$(CP) \
			$(PKG_INSTALL_DIR)/$(QT_INSTALL_PLUGINS)/$(2) \
			$(1)/$(QT_INSTALL_PLUGINS)/ ; \
	else \
		$(INSTALL_DIR) \
			$(1)/$(QT_INSTALL_PLUGINS)/$(2) ; \
		$(CP) \
			$(PKG_INSTALL_DIR)/$(QT_INSTALL_PLUGINS)/$(2)/$(3).so* \
			$(1)/$(QT_INSTALL_PLUGINS)/$(2)/ ; \
	fi
endef

define Build/Install/Examples
	$(INSTALL_DIR) \
		$(1)/$(QT_INSTALL_EXAMPLES)

	$(CP) \
		$(PKG_INSTALL_DIR)/$(QT_INSTALL_EXAMPLES)/* \
		$(1)/$(QT_INSTALL_EXAMPLES)/

	$(FIND) $(1)/$(QT_INSTALL_EXAMPLES) \
		-type f \( -name '*.cpp' -o -name '*.h' -o -name '*.pro' -o -name '*.pri' \) | \
		$(XARGS) $(RM) -vf
endef
