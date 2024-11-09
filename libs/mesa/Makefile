include $(TOPDIR)/rules.mk

PKG_NAME:=mesa
PKG_VERSION:=24.2.6
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://archive.mesa3d.org/
PKG_HASH:=2b68c4a6f204c1999815a457299f81c41ba7bf48c4674b0b2d1d8864f41f3709

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-3-Clause

PKG_INSTALL:=1
PKG_BUILD_DEPENDS:=python3/host

PKG_CONFIG_DEPENDS:= \
	CONFIG_LIBDRM_INTEL \
	CONFIG_LIBDRM_NOUVEAU \
	CONFIG_LIBDRM_RADEON

TARGET_CPPFLAGS+=-Wno-format -Wno-format-security

PYTHON3_PKG_BUILD:=0
HOST_PYTHON3_PACKAGE_BUILD_DEPENDS:=./host-pip-requirements.txt

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/meson.mk
include $(TOPDIR)/feeds/packages/lang/python/python3-package.mk

DRI_PLUGINS := \
	armada-drm:@(aarch64||arm) \
	etnaviv:@(aarch64||arm) \
	exynos:@(aarch64||arm) \
	hx8357d \
	i915:@LIBDRM_INTEL \
	i965:@LIBDRM_INTEL \
	ili9225 \
	ili9341 \
	imx-dcss:@(aarch64||arm) \
	imx-drm:@(aarch64||arm) \
	ingenic-drm:@(aarch64||arm||mipsel) \
	kgsl:@(aarch64||arm) \
	kms_swrast \
	lima:@(aarch64||arm) \
	mcde:@arm \
	mediatek:@(aarch64||arm) \
	meson:@(aarch64||arm) \
	mi0283qt \
	msm:@(aarch64||arm) \
	mxsfb-drm:@(aarch64||arm) \
	nouveau:@LIBDRM_NOUVEAU \
	nouveau_vieux:@LIBDRM_NOUVEAU \
	panfrost:@(aarch64||arm) \
	pl111:@(aarch64||arm) \
	r200:@LIBDRM_RADEON \
	r300:@LIBDRM_RADEON \
	radeon:@LIBDRM_RADEON \
	repaper \
	rockchip:@(aarch64||arm) \
	st7586 \
	st7735r \
	stm:@(aarch64||arm) \
	sun4i-drm:@(aarch64||arm) \
	swrast \
	tegra:@((aarch64||arm)&&LIBDRM_NOUVEAU) \
	vmwgfx:@(i386||i686||x86_64) \
	virtio_gpu


define Package/libxatracker
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=Mesa3D libxatracker
  URL:=https://www.mesa3d.org
  DEPENDS:=+libstdcpp +zlib +libdrm +libexpat +libzstd @(aarch64||arm||i386||i686||x86_64)
endef

define Package/libxatracker/description
endef

define Package/libglapi
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=Mesa3D libglapi
  URL:=https://www.mesa3d.org
  DEPENDS:=+libpthread +libatomic
endef

define Package/libglapi/description
endef

define Package/libmesa
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  DEPENDS:=+libglapi +libstdcpp +zlib +libdrm +libsensors +libwayland +libzstd +wayland-protocols
  TITLE:=Mesa3D OpenGL and friends
  URL:=https://www.mesa3d.org
endef

define Package/libmesa/description
endef

define Package/libosmesa
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  DEPENDS:=+libglapi +libstdcpp +zlib +libwayland +libzstd +wayland-protocols
  TITLE:=off-screen Mesa library
  URL:=https://www.mesa3d.org
endef

define Package/libosmesa/description
endef

define Package/libvulkan-broadcom
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  DEPENDS:=libmesa @(arm||aarch64) @BROKEN
  TITLE:=Broadcom Vulkan driver
  URL:=https://www.mesa3d.org
endef

define Package/libvulkan-broadcom/description
endef

# define Package/libvulkan-freedreno
#   SECTION:=libs
#   CATEGORY:=Libraries
#   SUBMENU:=Video
#   DEPENDS:=libmesa @(arm||aarch64)
#   TITLE:=Freedreno Vulkan driver
#   URL:=https://www.mesa3d.org
# endef
#
# define Package/libvulkan-freedreno/description
# endef

define Package/libvulkan-intel
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  DEPENDS:=libmesa @(i386||i686||x86_64)
  TITLE:=Intel Vulkan driver
  URL:=https://www.mesa3d.org
endef

define Package/libvulkan-intel/description
endef

define PluginGen
define Package/libmesadri-$(subst _,-,$(firstword $(subst :, ,$(1))))
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=Mesa3D DRI $(firstword $(subst :, ,$(1))) plugin
  URL:=https://www.mesa3d.org
  DEPENDS:=libmesa +libsensors +libzstd $(wordlist 2,$(words $(subst :, ,$(1))),$(subst :, ,$(1)))
endef
endef

$(foreach file,$(DRI_PLUGINS),$(eval $(call PluginGen,$(file))))

GALLIUM_DRIVERS:=softpipe vc4 virgl

# requires LLVM:
# VULKAN_DRIVERS:=swrast

ifneq ($(CONFIG_LIBDRM_INTEL),)
  GALLIUM_DRIVERS+=i915
endif

ifneq ($(CONFIG_LIBDRM_NOUVEAU),)
  GALLIUM_DRIVERS+=nouveau
endif

ifneq ($(CONFIG_LIBDRM_RADEON),)
  GALLIUM_DRIVERS+=r300
# r600 and radeonsi require LLVM
endif

ifeq ($(ARCH),aarch64)
  GALLIUM_DRIVERS+=etnaviv panfrost lima
# freedreno doesn't detect Lua correctly
ifneq ($(CONFIG_LIBDRM_NOUVEAU),)
  GALLIUM_DRIVERS+=tegra
endif
endif

ifeq ($(ARCH),arm)
  GALLIUM_DRIVERS+=etnaviv panfrost lima
# freedreno doesn't detect Lua correctly
ifneq ($(CONFIG_LIBDRM_NOUVEAU),)
  GALLIUM_DRIVERS+=tegra
endif
endif

ifeq ($(ARCH),mips)
  GALLIUM_DRIVERS+=etnaviv
endif

ifeq ($(ARCH),mipsel)
  GALLIUM_DRIVERS+=etnaviv
endif

ifeq ($(ARCH),x86_64)
  GALLIUM_DRIVERS+=svga
endif

ifeq ($(ARCH),i386)
  GALLIUM_DRIVERS+=svga
endif

ifeq ($(ARCH),i686)
  GALLIUM_DRIVERS+=svga
endif

ifneq ($(CONFIG_PACKAGE_libvulkan-broadcom),)
  VULKAN_DRIVERS+=broadcom
endif

ifneq ($(CONFIG_PACKAGE_libvulkan-freedreno),)
  VULKAN_DRIVERS+=freedreno
endif

ifneq ($(CONFIG_PACKAGE_libvulkan-intel),)
  VULKAN_DRIVERS+=intel
endif

MESON_ARGS += \
	-Dplatforms=wayland \
	-Dglx=disabled \
	-Ddri3=enabled \
	-Degl=enabled \
	-Dgbm=enabled \
	-Dgles1=enabled \
	-Dgles2=enabled \
	-Dglvnd=disabled \
	-Dopengl=true \
	-Dandroid-stub=false \
	-Dgallium-drivers=$(subst $(space),$(comma),$(strip $(GALLIUM_DRIVERS))) \
	-Dvulkan-drivers=$(subst $(space),$(comma),$(strip $(VULKAN_DRIVERS))) \
	-Dosmesa=true \
	-Dtools= \
	-Dllvm=disabled \
	-Dshared-glapi=enabled \
	-Ddraw-use-llvm=false

TARGET_CFLAGS += -I$(MESON_BUILD_DIR)/src/broadcom/

define Build/Configure
	$(call Py3Build/InstallBuildDepends)
	$(call Build/Configure/Meson)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

define Package/libglapi/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libglapi.so* $(1)/usr/lib/
endef

define Package/libxatracker/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libxatracker.so* $(1)/usr/lib/
endef

define Package/libmesa/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*GL*.so* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libgbm.so* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libgallium*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/share/drirc.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/drirc.d/* $(1)/usr/share/drirc.d/
endef

define Package/libosmesa/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libOSMesa.so* $(1)/usr/lib/
endef

define Package/libvulkan-broadcom/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvulkan_broadcom.so $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/share/vulkan/icd.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vulkan/icd.d/broadcom_icd.generic.json $(1)/usr/share/vulkan/icd.d/
endef

define Package/libvulkan-freedreno/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvulkan_freedreno.so $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/share/vulkan/icd.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vulkan/icd.d/freedreno_icd.generic.json $(1)/usr/share/vulkan/icd.d/
endef

define Package/libvulkan-intel/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvulkan_intel.so $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/share/vulkan/icd.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vulkan/icd.d/intel_icd.generic.json $(1)/usr/share/vulkan/icd.d/
endef

$(eval $(call BuildPackage,libxatracker))
$(eval $(call BuildPackage,libglapi))
$(eval $(call BuildPackage,libmesa))
$(eval $(call BuildPackage,libosmesa))
$(eval $(call BuildPackage,libvulkan-intel))
$(eval $(call BuildPackage,libvulkan-broadcom))
# $(eval $(call BuildPackage,libvulkan-freedreno))
