include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_OPTIONS += \
  -DQT_QMAKE_TARGET_MKSPEC="linux-openwrt-g++"

CMAKE_HOST_OPTIONS += \
  -DQT_QMAKE_TARGET_MKSPEC="linux-openwrt-host-g++"
