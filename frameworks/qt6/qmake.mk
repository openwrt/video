#
# Copyright (C) 2026 OpenWrt
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
#    This is worked around by overriding the PKG_INSTALL_DIR, so the Makefiles don't
#    have to care about that.
#  - Once compiled, qmake's location and its requirements (mkspecs, etc.) are fixed,
#    since its absolute paths were hardcoded. No moving around of the toolchain.
#  - Those variables might be used for target binaries for some weird reason, so
#    paths to the host staging_dir would make it to the target, logically leading to
#    errors.
#  - Paths might make it into target binaries, thus referencing non-existing
#    objects on the target platform. This behaviour wasn't observed so far, however
#    one might use the QT_INSTALL_* variables for some weird reason during runtime.


include $(TOPDIR)/rules.mk

QMAKE_SPEC:=linux-openwrt-host-g++ # mkspec for host builds
QMAKE_XSPEC:=linux-openwrt-g++ # mkspec for target builds

# read by the linux-openwrt-host-g++ mkspec, which is also used for the host
# parts (host_build) of target builds
QMAKE_HOST_VARS = \
	HOSTCC="$(HOSTCC)" \
	HOSTCXX="$(HOSTCXX)" \
	HOST_CFLAGS="$(HOST_CFLAGS)" \
	HOST_CXXFLAGS="$(HOST_CXXFLAGS)" \
	HOST_LDFLAGS="$(HOST_LDFLAGS)"

QMAKE_TARGET=$(STAGING_DIR)/host/bin/qt6/qmake # host exe used for target specific builds
QMAKE_HOST=$(QT_HOST_BINS)/qmake # host exe used for host builds

define Build/Configure/Default
	TARGET_CROSS="$(TARGET_CROSS)" \
	TARGET_CFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
	TARGET_CXXFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS)" \
	TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
	$(QMAKE_HOST_VARS) \
	$(QMAKE_TARGET) \
		-xspec $(QMAKE_XSPEC) \
		-o $(PKG_BUILD_DIR)/$(MAKE_PATH)/Makefile \
		$(PKG_BUILD_DIR)/$(MAKE_PATH)/$(if $(1),$(1).pro,)
endef

define Host/Configure/Default
	$(QMAKE_HOST_VARS) \
	$(QMAKE_HOST) \
		-spec $(QMAKE_SPEC) \
		-o $(HOST_BUILD_DIR)/$(HOST_MAKE_PATH)/Makefile \
		$(HOST_BUILD_DIR)/$(HOST_MAKE_PATH)/$(if $(1),$(1).pro,)
endef

# We need to pass all qmake (TARGET_*) related variables to $(MAKE) as well, as
# (generated) Makefiles may invoke qmake once again for creating further Makefiles.
# Actually we'd also like to pass all other vars (defined in $MAKE_VARS and
# $MAKE_FLAGS) to also make ordinary non-qmake generated Makefiles calling tool-
# chain executables like $CC/$CXX/$AR.. work, however this would interfere with
# qmake generated Makefiles, since they expect variables being set differently.
# For example qmake generated Makefiles expect $AR to also contain ar's arguments,
# while ordinary Makefiles don't.
# Until we find a way to distinguish both kinds of Makefiles, we will neglect
# ordinary Makefiles calling toolchain executables, however as they might take
# $CFLAGS/CXXFLAGS into account (e.g. flags as -D*), we pass at least those
# hoping to not interfere / break something.
# Mixing qmake generated and ordinary Makefiles - both calling toolchain execut-
# ables - is probably a very rare case anyway.
define Build/Compile/Default
	+TARGET_CROSS="$(TARGET_CROSS)" \
	TARGET_CFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
	TARGET_CXXFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS)" \
	TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
	CFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
	CXXFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CXXFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)" \
	$(QMAKE_HOST_VARS) \
		$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
			$(1)
endef

define Host/Compile/Default
	+$(QMAKE_HOST_VARS) \
		$(MAKE) $(HOST_JOBS) -C $(HOST_BUILD_DIR)/$(HOST_MAKE_PATH) \
		$(1)
endef

# Files installed to $$[QT_INSTALL_*] end up below the absolute staging path,
# files installed to fixed paths (e.g. /usr/bin) directly below INSTALL_ROOT.
# Both are moved to their target paths below PKG_INSTALL_DIR.
define Build/Install/Default
	rm -rf $(PKG_INSTALL_DIR)/.owrttmp
	INSTALL_ROOT="$(PKG_INSTALL_DIR)/.owrttmp" \
		$(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
			$(1) install
	if [ -d "$(PKG_INSTALL_DIR)/.owrttmp$(STAGING_DIR)" ]; then \
		$(CP) \
			"$(PKG_INSTALL_DIR)/.owrttmp$(STAGING_DIR)/." \
			$(PKG_INSTALL_DIR)/ && \
		rm -rf "$(PKG_INSTALL_DIR)/.owrttmp$(STAGING_DIR)"; \
	fi
	$(FIND) $(PKG_INSTALL_DIR)/.owrttmp -mindepth 1 -depth -type d -empty -delete
	$(CP) \
		$(PKG_INSTALL_DIR)/.owrttmp/. \
		$(PKG_INSTALL_DIR)/
	rm -rf $(PKG_INSTALL_DIR)/.owrttmp
endef

# Host projects have to install to $$[QT_INSTALL_*] or $$[QT_HOST_*], which
# are below the host Qt prefix. Files installed anywhere else fail the build.
# Host/Install passes the install prefix as $(1), it is not a make argument.
define Host/Install/Default
	rm -rf $(HOST_INSTALL_DIR)
	INSTALL_ROOT="$(HOST_INSTALL_DIR)" \
		$(MAKE) -C $(HOST_BUILD_DIR)/$(HOST_MAKE_PATH) \
			install
	if $(FIND) $(HOST_INSTALL_DIR) \
			-path "$(HOST_INSTALL_DIR)$(QT_HOST_PREFIX)" -prune \
			-o ! -type d -print | grep .; then \
		echo "Files installed outside of $(QT_HOST_PREFIX)" >&2; \
		false; \
	fi
	$(CP) \
		$(HOST_INSTALL_DIR)$(QT_HOST_PREFIX)/. \
		$(QT_HOST_PREFIX)/
endef

# target specific host builds triggered by target qmake runs
define Build/Install/HostFiles
	$(INSTALL_DIR) \
		$(STAGING_DIR)/host

	$(CP) \
		$(PKG_INSTALL_DIR)/host/* \
		$(STAGING_DIR)/host/
endef
