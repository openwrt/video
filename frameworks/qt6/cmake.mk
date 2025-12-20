include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/cmake.mk

# the bin-path should be determined correctly via cmake or qmake queries,
# but in case scripts call it relying on it in $PATH, add bin/qt6/ to it.
export PATH:=$(STAGING_DIR_HOSTPKG)/bin/qt6:$(PATH)

CMAKE_OPTIONS += \
  -DQT_HOST_PATH="$(STAGING_DIR_HOSTPKG)" \
  -DQT_QMAKE_TARGET_MKSPEC="linux-openwrt-g++"
  #-DCMAKE_TOOLCHAIN_FILE="/tmp/toolchain.cmake"

CMAKE_HOST_OPTIONS += \
  -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_SKIP_RPATH=FALSE \
  -DQT_QMAKE_TARGET_MKSPEC="linux-g++"
