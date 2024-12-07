include $(TOPDIR)/rules.mk

PKG_NAME:=mesa
PKG_VERSION:=24.3.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://archive.mesa3d.org/
PKG_HASH:=97813fe65028ef21b4d4e54164563059e8408d8fee3489a2323468d198bf2efc

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-3-Clause

PKG_INSTALL:=1

# arc, powerpc and powerpc64 LLVM builds are currently broken, but powerpc{,64} would support JIT in theory
LLVM_SUPPORTED:=(aarch64||arm||i386||i686||loongarch64||mips||mipsel||mips64||mips64el||riscv64||x86_64)

PKG_BUILD_DEPENDS:=glslang/host python3/host libva (x86_64||i386||i686):mesa/host $(LLVM_SUPPORTED):llvm
HOST_BUILD_DEPENDS:=python3/host spirv-tools/host llvm

PKG_CONFIG_DEPENDS:= \
	CONFIG_PACKAGE_libosmesa \
	CONFIG_PACKAGE_libvulkan-broadcom \
	CONFIG_PACKAGE_libvulkan-intel \
	CONFIG_PACKAGE_libvulkan-intel-hasvk \
	CONFIG_PACKAGE_libvulkan-lvp \
	CONFIG_PACKAGE_libvulkan-nouveau \
	CONFIG_PACKAGE_libvulkan-radeon

TARGET_CPPFLAGS+=-Wno-format -Wno-format-security

PYTHON3_PKG_BUILD:=0
MESON_USE_STAGING_PYTHON:=1
HOST_PYTHON3_PACKAGE_BUILD_DEPENDS:=./host-pip-requirements.txt

ifneq ($(CONFIG_aarch64)$(CONFIG_arm)$(CONFIG_i386)$(CONFIG_i686)$(CONFIG_loongarch64)$(CONFIG_mips)$(CONFIG_mipsel)$(CONFIG_mips64)$(CONFIG_mips64el)$(CONFIG_riscv64)$(CONFIG_x86_64),)
MESA_USE_LLVM:=1
else
MESA_USE_LLVM:=
endif

include $(TOPDIR)/feeds/packages/lang/rust/rust-values.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(TOPDIR)/feeds/packages/lang/python/python3-package.mk
include $(INCLUDE_DIR)/meson.mk

MESON_HOST_VARS += PYTHONPATH="$(HOST_PYTHON3PATH)"
MESON_VARS += PYTHONPATH="$(HOST_PYTHON3PATH)"

TARGET_LDFLAGS += -Wl,--gc-sections,--as-needed

define Package/mesa/Default
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=Mesa3D
  URL:=https://www.mesa3d.org
endef

define Package/libmesa/Default
$(call Package/mesa/Default)
  DEPENDS:=+libglapi +libstdcpp +zlib +libdrm +libsensors +libwayland +libzstd \
	   +wayland-protocols
  TITLE+= OpenGL and friends
  PROVIDES:=libmesa
endef


define Package/libglapi
$(call Package/mesa/Default)
  TITLE+= libglapi
  DEPENDS:=+libpthread +libatomic
  VARIANT:=softpipe
endef

define Package/libglapi/description
endef

define Package/libglapi/config
config HAVE_MESA
	bool
	default y
endef


define Package/libmesa-amd
$(call Package/libmesa/Default)
  TITLE+= (AMD)
  DEPENDS+=+libdrm-amdgpu +libdrm-radeon +libelf @$(LLVM_SUPPORTED)
  VARIANT:=amd
endef

define Package/libmesa-amd/description
endef

define Package/libmesa-intel
$(call Package/libmesa/Default)
  DEPENDS+=+libdrm-intel @(i386||i686||x86_64)
  TITLE+= (Intel)
  VARIANT:=intel
endef

define Package/libmesa-intel/description
endef


define Package/libmesa-tegra
$(call Package/libmesa/Default)
  DEPENDS+=+libdrm-tegra @(aarch64||arm)
  TITLE+= (nVidia Tegra)
  VARIANT:=tegra
endef

define Package/libmesa-tegra/description
endef


define Package/libmesa-etnaviv
$(call Package/libmesa/Default)
  DEPENDS+=+libdrm-etnaviv @(aarch64||arm||mips||mips64||mipsel||mips64el||riscv64)
  TITLE+= (Vivante)
  VARIANT:=etnaviv
endef

define Package/libmesa-etnaviv/description
endef


define Package/libmesa-nouveau
$(call Package/libmesa/Default)
  DEPENDS+=+libdrm-nouveau @$(LLVM_SUPPORTED)
  TITLE+= (nVidia)
  VARIANT:=nouveau
endef

define Package/libmesa-nouveau/description
endef


define Package/libmesa-broadcom
$(call Package/libmesa/Default)
  TITLE+= (Broadcom)
  VARIANT:=broadcom
  DEPENDS+=+libarchive @(aarch64||arm)
endef

define Package/libmesa-broadcom/description
endef


define Package/libmesa-lima
$(call Package/libmesa/Default)
  TITLE+= (ARM Mali 400)
  VARIANT:=lima
  DEPENDS+=@(aarch64||arm)
endef

define Package/libmesa-lima/description
endef


define Package/libmesa-panfrost
$(call Package/libmesa/Default)
  TITLE+= (ARM Mali 450+)
  VARIANT:=panfrost
  DEPENDS+=@(aarch64||arm)
endef

define Package/libmesa-panfrost/description
endef


define Package/libmesa-virgl
$(call Package/libmesa/Default)
  TITLE+= (VirtIO GL)
  VARIANT:=virgl
endef

define Package/libmesa-virgl/description
endef


define Package/libmesa-zink
$(call Package/libmesa/Default)
  TITLE+= (Zink GL via Vulkan)
  VARIANT:=vulkan
  DEPENDS+=@$(LLVM_SUPPORTED)
endef

define Package/libmesa-zink/description
endef


define Package/libmesa-softpipe
$(call Package/libmesa/Default)
  TITLE+= (Softpipe)
  VARIANT:=softpipe
  DEFAULT_VARIANT:=1
endef

define Package/libmesa-softpipe/description
endef

define Package/libmesa-llvmpipe
$(call Package/libmesa/Default)
  TITLE+= (LLVMpipe)
  DEPENDS+=@$(LLVM_SUPPORTED)
  VARIANT:=llvmpipe
endef

define Package/libmesa-llvmpipe/description
endef


define Package/libopencl/Default
$(call Package/mesa/Default)
  TITLE+= OpenCL
  PROVIDES:=libopencl
  DEPENDS:=+$(LLVM_SUPPORTED):libclang-cpp +libdrm +libelf +libexpat +libstdcpp +libzstd +zlib @$(LLVM_SUPPORTED)
endef


define Package/libopencl-amd
$(call Package/libopencl/Default)
  TITLE+= (AMD)
  VARIANT:=amd
endef

define Package/libopencl-amd/description
endef

define Package/libopencl-intel
$(call Package/libopencl/Default)
  TITLE+= (Intel)
  VARIANT:=intel
  DEPENDS+=@(i386||i686||x86_64)
endef

define Package/libopencl-intel/description
endef


define Package/libopencl-tegra
$(call Package/libopencl/Default)
  TITLE+= (nVidia Tegra)
  VARIANT:=tegra
  DEPENDS+=@(aarch64||arm)
endef

define Package/libopencl-tegra/description
endef


define Package/libopencl-etnaviv
$(call Package/libopencl/Default)
  TITLE+= (Vivante)
  VARIANT:=etnaviv
  DEPENDS+=@(aarch64||arm||mips||mipsel||mips64||mips64el)
endef

define Package/libopencl-etnaviv/description
endef


define Package/libopencl-nouveau
$(call Package/libopencl/Default)
  TITLE+= (nVidia)
  VARIANT:=nouveau
endef

define Package/libopencl-nouveau/description
endef


define Package/libopencl-broadcom
$(call Package/libopencl/Default)
  TITLE+= (Broadcom)
  VARIANT:=broadcom
  DEPENDS+=@(aarch64||arm)
endef

define Package/libopencl-broadcom/description
endef


define Package/libopencl-lima
$(call Package/libopencl/Default)
  TITLE+= (ARM Mali 400)
  VARIANT:=lima
  DEPENDS+=@(aarch64||arm)
endef

define Package/libopencl-lima/description
endef


define Package/libopencl-panfrost
$(call Package/libopencl/Default)
  TITLE+= (ARM Mali 450+)
  VARIANT:=panfrost
  DEPENDS+=@(aarch64||arm)
endef

define Package/libopencl-panfrost/description
endef


define Package/libopencl-virgl
$(call Package/libopencl/Default)
  TITLE+= (VirtIO GL)
  VARIANT:=virgl
endef

define Package/libopencl-virgl/description
endef


define Package/libopencl-zink
$(call Package/libopencl/Default)
  TITLE+= (Zink GL via Vulkan)
  VARIANT:=vulkan
endef

define Package/libopencl-zink/description
endef


define Package/libopencl-softpipe
$(call Package/libopencl/Default)
  TITLE+= (Softpipe)
  VARIANT:=softpipe
  DEFAULT_VARIANT:=1
endef

define Package/libopencl-softpipe/description
endef

define Package/libopencl-llvmpipe
$(call Package/libopencl/Default)
  TITLE+= (LLVMpipe)
  DEPENDS+=@$(LLVM_SUPPORTED)
  VARIANT:=llvmpipe
endef

define Package/libopencl-llvmpipe/description
endef

define Package/libosmesa/Default
$(call Package/mesa/Default)
  DEPENDS:=+libdrm +libglapi +libstdcpp +zlib +libwayland +libzstd +wayland-protocols
  TITLE+= off-screen rendering library
endef

define Package/libosmesa-softpipe
$(call Package/libosmesa/Default)
  TITLE+= (Softpipe)
  VARIANT:=softpipe
  DEFAULT_VARIANT:=1
endef

define Package/libosmesa-softpipe/description
endef

define Package/libosmesa-llvmpipe
$(call Package/libosmesa/Default)
  TITLE+= (LLVMpipe)
  DEPENDS+=@$(LLVM_SUPPORTED)
  VARIANT:=llvmpipe
endef

define Package/libosmesa-llvmpipe/description
endef


define Package/libvulkan-broadcom
$(call Package/mesa/Default)
  DEPENDS:=+libdrm +libexpat +libstdcpp +libudev +libwayland +libzstd +zlib \
	   @(arm||aarch64)
  TITLE+= Broadcom Vulkan driver
  VARIANT:=vulkan
endef

define Package/libvulkan-broadcom/description
endef

define Package/libvulkan-imagination
$(call Package/mesa/Default)
  DEPENDS:=+libatomic +libdrm +libexpat +libstdcpp +libudev +libwayland +libzstd +zlib \
	   @(aarch64||arm||mips||mips64||mipsel||mips64el||riscv64)
  TITLE+= Imagination PowerVR driver (EXPERIMENTAL)
  VARIANT:=vulkan
endef

define Package/libvulkan-imagination/description
endef

define Package/libvulkan-intel
$(call Package/mesa/Default)
  DEPENDS:=+libdrm-intel +libexpat +libstdcpp +libudev +libwayland +libzstd +zlib \
	   @(i386||i686||x86_64)
  TITLE+= Intel Gen9+ Vulkan driver
  VARIANT:=vulkan
endef

define Package/libvulkan-intel/description
endef

define Package/libvulkan-intel-hasvk
$(call Package/mesa/Default)
  DEPENDS:=+libdrm-intel +libexpat +libstdcpp +libudev +libwayland +libzstd +zlib \
	   @(i386||i686||x86_64)
  TITLE+= Intel Gen7/Gen8 Vulkan driver
  VARIANT:=vulkan
endef

define Package/libvulkan-intel-hasvk/description
endef

define Package/libvulkan-lvp
$(call Package/mesa/Default)
  DEPENDS:=+libatomic +libdrm +libstdcpp +libudev +libwayland +libzstd +zlib @$(LLVM_SUPPORTED)
  TITLE+= LLVM JIT CPU rendering Vulkan driver
  VARIANT:=vulkan
endef

define Package/libvulkan-lvp/description
endef

define Package/libvulkan-nouveau
$(call Package/mesa/Default)
  TITLE+= nouveau Vulkan driver for nVidia GPUs
  DEPENDS:=$(RUST_ARCH_DEPENDS) @BROKEN
  VARIANT:=vulkan
# needs rust-bindgen which currently can't be built
endef

define Package/libvulkan-nouveau/description
endef

define Package/libvulkan-panfrost
$(call Package/mesa/Default)
  DEPENDS:=+libdrm +libexpat +libstdcpp +libudev +libwayland +libzstd +zlib \
	   @(arm||aarch64)
  TITLE+= ARM Mali 450+ Vulkan driver
  VARIANT:=vulkan
endef

define Package/libvulkan-panfrost/description
endef

define Package/libvulkan-radeon
$(call Package/mesa/Default)
  TITLE+= AMD Radeon Vulkan driver
  DEPENDS:=+libatomic +libdrm-amdgpu +libelf +libexpat +libstdcpp +libudev +libwayland +libzstd +zlib @$(LLVM_SUPPORTED)
  VARIANT:=vulkan
endef

define Package/libvulkan-radeon/description
endef

ifneq ($(CONFIG_PACKAGE_libvulkan-broadcom),)
  VULKAN_DRIVERS+=broadcom
endif

ifneq ($(CONFIG_PACKAGE_libvulkan-imagination),)
  VULKAN_DRIVERS+=imagination-experimental
endif

ifneq ($(CONFIG_PACKAGE_libvulkan-intel),)
  VULKAN_DRIVERS+=intel
endif

ifneq ($(CONFIG_PACKAGE_libvulkan-intel-hasvk),)
  VULKAN_DRIVERS+=intel_hasvk
endif

ifneq ($(CONFIG_PACKAGE_libvulkan-lvp),)
  VULKAN_DRIVERS+=swrast
endif

ifneq ($(CONFIG_PACKAGE_libvulkan-nouveau),)
  VULKAN_DRIVERS+=nouveau
endif

ifneq ($(CONFIG_PACKAGE_libvulkan-panfrost),)
  VULKAN_DRIVERS+=panfrost
endif

ifneq ($(CONFIG_PACKAGE_libvulkan-radeon),)
  VULKAN_DRIVERS+=amd
endif

MESON_HOST_ARGS += \
	-Dplatforms= \
	-Dglx=disabled \
	-Degl=disabled \
	-Dgbm=disabled \
	-Dgles1=disabled \
	-Dgles2=disabled \
	-Dglvnd=disabled \
	-Dopengl=false \
	-Dandroid-stub=false \
	-Dlibunwind=disabled \
	-Dvalgrind=disabled \
	-Dgallium-drivers= \
	-Dvulkan-drivers= \
	-Dosmesa=false \
	-Dgallium-xa=disabled \
	-Dtools=nir \
	-Dllvm=enabled \
	-Dshared-llvm=disabled \
	-Dshared-glapi=disabled \
	-Ddraw-use-llvm=false \
	-Dintel-clc=enabled \
	-Dstatic-libclc=all \
	-Dinstall-intel-clc=true

MESON_ARGS += \
	-Dplatforms=wayland \
	-Dglx=disabled \
	-Degl=enabled \
	-Dgbm=enabled \
	-Dgles1=enabled \
	-Dgles2=enabled \
	-Dglvnd=disabled \
	-Dopengl=true \
	-Dandroid-stub=false \
	-Dlibunwind=disabled \
	-Dvalgrind=disabled \
	-Dtools= \
	-Dllvm=$(if $(MESA_USE_LLVM),en,dis)abled \
	-Dshared-glapi=enabled \
	-Dgallium-opencl=$(if $(MESA_USE_LLVM),standalone,disabled) \
	-Ddraw-use-llvm=$(if $(MESA_USE_LLVM),true,false) \
	-Dintel-clc=system

ifeq ($(BUILD_VARIANT),amd)
MESON_ARGS += \
	-Dgallium-drivers=r300,r600,radeonsi
endif

ifeq ($(BUILD_VARIANT),intel)
MESON_ARGS += \
	-Dgallium-drivers=crocus,i915,iris \
	-Dintel-rt=$(if $(CONFIG_TARGET_x86_64),en,dis)abled
endif

ifeq ($(BUILD_VARIANT),tegra)
MESON_ARGS += \
	-Dgallium-drivers=tegra,nouveau
endif

ifeq ($(BUILD_VARIANT),etnaviv)
MESON_ARGS += \
	-Dgallium-drivers=etnaviv
endif

ifeq ($(BUILD_VARIANT),broadcom)
MESON_ARGS += \
	-Dgallium-drivers=vc4,v3d
endif

ifeq ($(BUILD_VARIANT),nouveau)
MESON_ARGS += \
	-Dgallium-drivers=nouveau
endif

ifeq ($(BUILD_VARIANT),lima)
MESON_ARGS += \
	-Dgallium-drivers=lima
endif

ifeq ($(BUILD_VARIANT),panfrost)
MESON_ARGS += \
	-Dgallium-drivers=panfrost
endif

ifeq ($(BUILD_VARIANT),virgl)
MESON_ARGS += \
	-Dgallium-drivers=virgl
endif

ifeq ($(BUILD_VARIANT),llvmpipe)
MESON_ARGS += \
	-Dgallium-drivers=llvmpipe \
	-Dosmesa=$(if $(CONFIG_PACKAGE_libosmesa-llvmpipe),true,false)
endif

ifeq ($(BUILD_VARIANT),softpipe)
MESON_ARGS += \
	-Dgallium-drivers=softpipe \
	-Dosmesa=$(if $(CONFIG_PACKAGE_libosmesa-softpipe),true,false)
endif

ifeq ($(BUILD_VARIANT),vulkan)
MESON_ARGS += \
	-Dgallium-drivers=zink \
	-Dvulkan-drivers=$(subst $(space),$(comma),$(strip $(VULKAN_DRIVERS)))
else
MESON_ARGS += \
	-Dvulkan-drivers=
endif

ifneq ($(CONFIG_BUILD_PATENTED),)
MESON_ARGS += -Dvideo-codecs=all
else
MESON_ARGS += -Dvideo-codecs=all_free
endif

TARGET_CFLAGS += -I$(MESON_BUILD_DIR)/src/broadcom/

define Host/Configure
	$(call Py3Build/InstallBuildDepends)
	$(INSTALL_DIR) $(HOST_BUILD_DIR)/subprojects/llvm
	$(LN) $(STAGING_DIR_HOST)/llvm-mesa/bin $(HOST_BUILD_DIR)/subprojects/llvm
	$(LN) $(STAGING_DIR_HOST)/llvm-mesa/include $(HOST_BUILD_DIR)/subprojects/llvm
	$(LN) $(STAGING_DIR_HOST)/llvm-mesa/lib $(HOST_BUILD_DIR)/subprojects/llvm
	$(CP) ./llvm-meson.build.in $(HOST_BUILD_DIR)/subprojects/llvm/meson.build
	LLVM_BUILDDIR=$(wildcard $(BUILD_DIR_HOST)/llvm-mesa/llvm-project-*) ; LLVM_VER=$$$${LLVM_BUILDDIR##*llvm-project-} ; LLVM_VER=$$$${LLVM_VER%%.src} ;\
	$(SED) "s%@LLVM_VERSION@%$$$${LLVM_VER}%" $(HOST_BUILD_DIR)/subprojects/llvm/meson.build
	LLVM_LIBS=$$$$( next_comma='' ; for lib in $(STAGING_DIR_HOST)/llvm-mesa/lib/*.a; do \
		lib=$$$${lib##*/lib} ; \
		lib=$$$${lib##*/} ; \
		lib=$$$${lib%%.a} ; \
		echo -n "$$$${next_comma}'$$$${lib}'" ; \
		next_comma=', ' ; \
	done ) ; $(SED) "s%@LLVM_LIBS@%$$$${LLVM_LIBS}%" $(HOST_BUILD_DIR)/subprojects/llvm/meson.build
	$(INSTALL_DATA) \
		$(STAGING_DIR_HOST)/llvm-mesa/lib/clang/*/include/opencl-c.h \
		$(STAGING_DIR_HOST)/llvm-mesa/lib/clang/*/include/opencl-c-base.h \
		$(HOST_BUILD_DIR)/src/compiler/clc
	$(call Host/Configure/Meson)
endef

define Build/Configure
	$(call Py3Build/InstallBuildDepends)
	$(INSTALL_DIR) $(PKG_BUILD_DIR)/subprojects/llvm
	$(LN) $(STAGING_DIR)/usr/bin $(PKG_BUILD_DIR)/subprojects/llvm
	$(LN) $(STAGING_DIR)/usr/include $(PKG_BUILD_DIR)/subprojects/llvm
	$(LN) $(STAGING_DIR)/usr/lib $(PKG_BUILD_DIR)/subprojects/llvm
	$(LN) $(STAGING_DIR)/usr/share $(PKG_BUILD_DIR)/subprojects/llvm
	$(CP) ./llvm-meson.build.in $(PKG_BUILD_DIR)/subprojects/llvm/meson.build
	LLVM_BUILDDIR=$(wildcard $(BUILD_DIR)/llvm-mesa/llvm-project-*) ; LLVM_VER=$$$${LLVM_BUILDDIR##*llvm-project-} ; LLVM_VER=$$$${LLVM_VER%%.src} ;\
	$(SED) "s%@LLVM_VERSION@%$$$${LLVM_VER}%" $(PKG_BUILD_DIR)/subprojects/llvm/meson.build
	LLVM_LIBS=$$$$( next_comma='' ; for lib in $(STAGING_DIR)/usr/lib/libclang*.a $(STAGING_DIR)/usr/lib/libLLVM*.a; do \
		lib=$$$${lib##*/lib} ; \
		lib=$$$${lib##*/} ; \
		lib=$$$${lib%%.a} ; \
		echo -n "$$$${next_comma}'$$$${lib}'" ; \
		next_comma=', ' ; \
	done ) ; $(SED) "s%@LLVM_LIBS@%$$$${LLVM_LIBS}%" $(PKG_BUILD_DIR)/subprojects/llvm/meson.build
	$(call Build/Configure/Meson)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc $(1)/usr/lib/pkgconfig
endef

define Package/libglapi/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libglapi.so* $(1)/usr/lib
endef

define Package/libmesa/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*GL*.so* $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libgbm.so* $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libgallium*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/share/drirc.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/drirc.d/* $(1)/usr/share/drirc.d
endef

define Package/libopencl/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libOpenCL.so* $(1)/usr/lib
endef

define Package/libosmesa/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libOSMesa.so* $(1)/usr/lib
endef

define Package/libvulkan-radeon/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvulkan_radeon.so $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/share/vulkan/icd.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vulkan/icd.d/radeon_icd.*.json $(1)/usr/share/vulkan/icd.d
endef

define Package/libvulkan-broadcom/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvulkan_broadcom.so $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/share/vulkan/icd.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vulkan/icd.d/broadcom_icd.*.json $(1)/usr/share/vulkan/icd.d
endef

define Package/libvulkan-imagination/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvulkan_powervr_mesa.so $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libpowervr_rogue.so $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/share/vulkan/icd.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vulkan/icd.d/powervr_mesa_icd.*.json $(1)/usr/share/vulkan/icd.d
endef

define Package/libvulkan-intel/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvulkan_intel.so $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/share/vulkan/icd.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vulkan/icd.d/intel_icd.*.json $(1)/usr/share/vulkan/icd.d
endef

define Package/libvulkan-intel-hasvk/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvulkan_intel_hasvk.so $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/share/vulkan/icd.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vulkan/icd.d/intel_hasvk_icd.*.json $(1)/usr/share/vulkan/icd.d
endef

define Package/libvulkan-lvp/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvulkan_lvp.so $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/share/vulkan/icd.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vulkan/icd.d/lvp_icd.*.json $(1)/usr/share/vulkan/icd.d
endef

define Package/libvulkan-nouveau/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvulkan_nouveau.so $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/share/vulkan/icd.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vulkan/icd.d/nouveau_icd.*.json $(1)/usr/share/vulkan/icd.d
endef

define Package/libvulkan-panfrost/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libvulkan_panfrost.so $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/share/vulkan/icd.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/vulkan/icd.d/panfrost_icd.*.json $(1)/usr/share/vulkan/icd.d
endef

Package/libmesa-intel/install = $(Package/libmesa/install)
Package/libmesa-amd/install = $(Package/libmesa/install)
Package/libmesa-tegra/install = $(Package/libmesa/install)
Package/libmesa-etnaviv/install = $(Package/libmesa/install)
Package/libmesa-broadcom/install = $(Package/libmesa/install)
Package/libmesa-lima/install = $(Package/libmesa/install)
Package/libmesa-panfrost/install = $(Package/libmesa/install)
Package/libmesa-nouveau/install = $(Package/libmesa/install)
Package/libmesa-virgl/install = $(Package/libmesa/install)
Package/libmesa-zink/install = $(Package/libmesa/install)
Package/libmesa-softpipe/install = $(Package/libmesa/install)
Package/libmesa-llvmpipe/install = $(Package/libmesa/install)

Package/libopencl-intel/install = $(Package/libopencl/install)
Package/libopencl-amd/install = $(Package/libopencl/install)
Package/libopencl-tegra/install = $(Package/libopencl/install)
Package/libopencl-etnaviv/install = $(Package/libopencl/install)
Package/libopencl-broadcom/install = $(Package/libopencl/install)
Package/libopencl-lima/install = $(Package/libopencl/install)
Package/libopencl-panfrost/install = $(Package/libopencl/install)
Package/libopencl-nouveau/install = $(Package/libopencl/install)
Package/libopencl-virgl/install = $(Package/libopencl/install)
Package/libopencl-zink/install = $(Package/libopencl/install)
Package/libopencl-softpipe/install = $(Package/libopencl/install)
Package/libopencl-llvmpipe/install = $(Package/libopencl/install)

Package/libosmesa-softpipe/install = $(Package/libosmesa/install)
Package/libosmesa-llvmpipe/install = $(Package/libosmesa/install)

$(eval $(call BuildPackage,libglapi))
$(eval $(call BuildPackage,libmesa-amd))
$(eval $(call BuildPackage,libmesa-intel))
$(eval $(call BuildPackage,libmesa-tegra))
$(eval $(call BuildPackage,libmesa-etnaviv))
$(eval $(call BuildPackage,libmesa-broadcom))
$(eval $(call BuildPackage,libmesa-nouveau))
$(eval $(call BuildPackage,libmesa-lima))
$(eval $(call BuildPackage,libmesa-panfrost))
$(eval $(call BuildPackage,libmesa-virgl))
$(eval $(call BuildPackage,libmesa-zink))
$(eval $(call BuildPackage,libmesa-softpipe))
$(eval $(call BuildPackage,libmesa-llvmpipe))
$(eval $(call BuildPackage,libopencl-amd))
$(eval $(call BuildPackage,libopencl-intel))
$(eval $(call BuildPackage,libopencl-tegra))
$(eval $(call BuildPackage,libopencl-etnaviv))
$(eval $(call BuildPackage,libopencl-broadcom))
$(eval $(call BuildPackage,libopencl-nouveau))
$(eval $(call BuildPackage,libopencl-lima))
$(eval $(call BuildPackage,libopencl-panfrost))
$(eval $(call BuildPackage,libopencl-virgl))
$(eval $(call BuildPackage,libopencl-zink))
$(eval $(call BuildPackage,libopencl-softpipe))
$(eval $(call BuildPackage,libopencl-llvmpipe))
$(eval $(call BuildPackage,libosmesa-softpipe))
$(eval $(call BuildPackage,libosmesa-llvmpipe))
$(eval $(call BuildPackage,libvulkan-broadcom))
$(eval $(call BuildPackage,libvulkan-imagination))
$(eval $(call BuildPackage,libvulkan-intel))
$(eval $(call BuildPackage,libvulkan-intel-hasvk))
$(eval $(call BuildPackage,libvulkan-lvp))
$(eval $(call BuildPackage,libvulkan-nouveau))
$(eval $(call BuildPackage,libvulkan-panfrost))
$(eval $(call BuildPackage,libvulkan-radeon))
$(eval $(call HostBuild))
