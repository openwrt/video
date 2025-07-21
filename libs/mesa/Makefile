include $(TOPDIR)/rules.mk

PKG_NAME:=mesa
PKG_VERSION:=25.1.6
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://archive.mesa3d.org/
PKG_HASH:=9f2b69eb39d2d8717d30a9868fdda3e0c0d3708ba32778bbac8ddb044538ce84

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-3-Clause

PKG_INSTALL:=1

PKG_BUILD_DEPENDS:=glslang/host python3/host libva MESA_USE_LLVM:llvm MESA_USE_LLVM:mesa/host

HOST_BUILD_DEPENDS:=python3/host spirv-tools/host llvm

PKG_CONFIG_DEPENDS:= \
	CONFIG_PACKAGE_libmesa-amd \
	CONFIG_PACKAGE_libmesa-intel \
	CONFIG_PACKAGE_libmesa-tegra \
	CONFIG_PACKAGE_libmesa-etnaviv \
	CONFIG_PACKAGE_libmesa-broadcom \
	CONFIG_PACKAGE_libmesa-nouveau \
	CONFIG_PACKAGE_libmesa-lima \
	CONFIG_PACKAGE_libmesa-panfrost \
	CONFIG_PACKAGE_libmesa-virgl \
	CONFIG_PACKAGE_libmesa-zink \
	CONFIG_PACKAGE_libmesa-softpipe \
	CONFIG_PACKAGE_libmesa-llvmpipe \
	CONFIG_PACKAGE_libopencl-amd \
	CONFIG_PACKAGE_libopencl-nouveau \
	CONFIG_PACKAGE_libosmesa-softpipe \
	CONFIG_PACKAGE_libosmesa-llvmpipe \
	CONFIG_PACKAGE_libvulkan-broadcom \
	CONFIG_PACKAGE_libvulkan-imagination \
	CONFIG_PACKAGE_libvulkan-intel \
	CONFIG_PACKAGE_libvulkan-intel-hasvk \
	CONFIG_PACKAGE_libvulkan-lvp \
	CONFIG_PACKAGE_libvulkan-nouveau \
	CONFIG_PACKAGE_libvulkan-panfrost \
	CONFIG_PACKAGE_libvulkan-radeon

TARGET_CPPFLAGS+=-Wno-format -Wno-format-security

PYTHON3_PKG_BUILD:=0
MESON_USE_STAGING_PYTHON:=1
HOST_PYTHON3_PACKAGE_BUILD_DEPENDS:=./host-pip-requirements.txt

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
  DEPENDS:=+libatomic
  TITLE:=Mesa3D
  URL:=https://www.mesa3d.org
endef

define Package/mesa3d
$(call Package/mesa/Default)
  HIDDEN:=1
endef

define Package/mesa3d/config
config HAVE_MESA
	bool
	default y

config MESA_USE_LLVM
	bool "Use LLVM to build MESA"
	depends on aarch64||arm||i386||i686||loongarch64||mips||mipsel||mips64||mips64el||riscv64||x86_64
	default y
endef

define Package/libmesa/Default
$(call Package/mesa/Default)
  DEPENDS+=+libstdcpp +zlib +libdrm +libsensors +libwayland +libzstd \
           +wayland-protocols
  PROVIDES:=libmesa
  TITLE+= OpenGL and friends
endef


define Package/libmesa-amd
$(call Package/libmesa/Default)
  TITLE+= (AMD)
  DEPENDS+=+libdrm-amdgpu +libdrm-radeon +libelf @MESA_USE_LLVM
  VARIANT:=amd
endef

define Package/libmesa-amd/description
A collection of drivers for AMD Radeon dedicated GPUs.

AMD R300

The R300 driver supports AMD's Radeon R300 GPU series.

AMD R600

The R600 driver supports AMD's Radeon HD 2000 GPU series. It's
officially supported by AMD.

AMD RadeonSI

The RadeonSI OpenGL driver supports AMD's Southern Island GPUs and
later. It's officially supported by AMD.
endef

define Package/libmesa-intel
$(call Package/libmesa/Default)
  DEPENDS+=+libdrm-intel @(i386||i686||x86_64) @MESA_USE_LLVM
  TITLE+= (Intel)
  VARIANT:=intel
endef

define Package/libmesa-intel/description
A collection of drivers for GPUs embedded in Intel's CPUs.

Intel Iris

The Iris driver supports Intel's Gen 8 hardware and later. It's
officially supported by Intel and is their next-generation Linux OpenGL
driver.

Intel Crocus

The Crocus driver supports Intel's Gen 7 hardware and earlier. It's
based on Iris and the former i965 driver, but is not officially
supported or endorsed by Intel.

Intel i915

The i915 driver supports older Intel i915 and i945 chipsets.
endef


define Package/libmesa-tegra
$(call Package/libmesa/Default)
  DEPENDS+=+libdrm-tegra @(aarch64||arm) @MESA_USE_LLVM
  TITLE+= (NVIDIA Tegra)
  VARIANT:=tegra
endef

define Package/libmesa-tegra/description
The Nouveau driver built for GPUs embedded in the Tegra SoC family. It's
a reverse-engineered, community-developed driver, and is not endorsed by
NVIDIA.
endef


define Package/libmesa-etnaviv
$(call Package/libmesa/Default)
  DEPENDS+=+libdrm-etnaviv @(aarch64||arm||mips||mips64||mipsel||mips64el||riscv64)
  TITLE+= (Vivante)
  VARIANT:=etnaviv
endef

define Package/libmesa-etnaviv/description
The Etnaviv driver supports the Vivante GCxxx series of embedded GPUs.
It's a reverse-engineered, community-developed driver, and is not
endorsed by Vivante.
endef


define Package/libmesa-nouveau
$(call Package/libmesa/Default)
  DEPENDS+=+libdrm-nouveau @MESA_USE_LLVM
  TITLE+= (NVIDIA)
  VARIANT:=nouveau
endef

define Package/libmesa-nouveau/description
The Nouveau drivers supports a large set of NVIDIA chips, ranging from
NV04 found in the Riva TNT card to NVF0 found in the GeForce GTX 780.
It's a reverse-engineered, community-developed driver, and is not
endorsed by NVIDIA.
endef


define Package/libmesa-broadcom
$(call Package/libmesa/Default)
  TITLE+= (Broadcom)
  VARIANT:=broadcom
  DEPENDS+=+libarchive @(aarch64||arm) @HAS_FPU
endef

define Package/libmesa-broadcom/description
Mesa’s VC4 graphics driver supports multiple implementations of
Broadcom's VideoCore IV GPU. It is notably used in the Raspberry Pi 0
through Raspberry Pi 3 hardware.
Along with GLES 2.0, the Mesa driver also exposes OpenGL 2.1, which is
mostly correct but with a few caveats.

Mesa’s V3D graphics driver stack includes a conformant GLES3.1 driver
called V3D, notably used on the Raspberry Pi 4 and Raspberry Pi 5.

This package contains both, VC4 and V3D drivers and covers all
RaspberryPi boards.
endef


define Package/libmesa-lima
$(call Package/libmesa/Default)
  TITLE+= (ARM Mali Utgard)
  VARIANT:=lima
  DEPENDS+=@(aarch64||arm)
endef

define Package/libmesa-lima/description
Lima is an open source graphics driver which supports Mali Utgard
(Mali-4xx) embedded GPUs from ARM. It's a reverse-engineered,
community-developed driver, and is not endorsed by ARM. Lima was
upstreamed in Mesa 19.1 and Linux kernel 5.2.

Product         Architecture    Status
Mali-400        Utgard          Supported
Mali-450        Utgard          Supported
Mali-470        Utgard          Unsupported

Newer Mali chips based on the Midgard/Bifrost architectures (Mali T or G
series) are handled by the Panfrost driver, not Lima.
endef


define Package/libmesa-panfrost
$(call Package/libmesa/Default)
  TITLE+= (ARM Mali Midgard and later)
  VARIANT:=panfrost
  DEPENDS+=@(aarch64||arm) @MESA_USE_LLVM
endef

define Package/libmesa-panfrost/description
The Panfrost driver stack includes an OpenGL ES implementation for Arm
Mali GPUs based on the Midgard, Bifrost and Valhall microarchitectures.
It is conformant on Mali-G52, Mali-G57 and Mali-G610, but non-conformant
on other GPUs. The following hardware is currently supported:

Models                          Architecture    OpenGL ES       OpenGL
T600, T620, T720                Midgard (v4)    2.0             2.1
T760, T820, T830 T860, T880     Midgard (v5)    3.1             3.1
G72                             Bifrost (v6)    3.1             3.1
G31, G51, G52, G76              Bifrost (v7)    3.1             3.1
G57                             Valhall (v9)    3.1             3.1
G310, G610                      Valhall (v10)   3.1             3.1

Other Midgard and Bifrost chips (e.g. G71) are not yet supported.
endef


define Package/libmesa-virgl
$(call Package/libmesa/Default)
  TITLE+= (VirtIO GL)
  VARIANT:=virgl
endef

define Package/libmesa-virgl/description
VirGL is a virtual 3D GPU for use inside QEMU virtual machines, that
allows the guest operating system to use the capabilities of the host
GPU to accelerate 3D rendering. The plan is to have a guest GPU that is
fully independent of the host GPU.
endef


define Package/libmesa-zink
$(call Package/libmesa/Default)
  TITLE+= (Zink GL via Vulkan)
  VARIANT:=vulkan
  DEPENDS+=@MESA_USE_LLVM
endef

define Package/libmesa-zink/description
The Zink driver is a Gallium driver that emits Vulkan API calls instead
of targeting a specific GPU architecture. This can be used to get full
desktop OpenGL support on devices that only support Vulkan.
endef


define Package/libmesa-softpipe
$(call Package/libmesa/Default)
  TITLE+= (Softpipe)
  VARIANT:=softpipe
  DEFAULT_VARIANT:=1
endef

define Package/libmesa-softpipe/description
The Softpipe driver is a reference software rasterizer; it's slow but
accurate. It's mostly useful for testing, and on systems that lack
support for LLVM.
endef

define Package/libmesa-llvmpipe
$(call Package/libmesa/Default)
  TITLE+= (LLVMpipe)
  DEPENDS+=@MESA_USE_LLVM
  VARIANT:=llvmpipe
endef

define Package/libmesa-llvmpipe/description
The Gallium LLVMpipe driver is a software rasterizer that uses LLVM to
do runtime code generation. Shaders, point/line/triangle rasterization
and vertex processing are implemented with LLVM IR which is translated
to native CPU machine code. Also, the driver is multithreaded to take
advantage of multiple CPU cores (up to 32 at this time). It's the
fastest software rasterizer for Mesa.
endef


define Package/libopencl/Default
$(call Package/mesa/Default)
  TITLE+= OpenCL
  PROVIDES:=libopencl
  DEPENDS+=+MESA_USE_LLVM:libclang-cpp +libdrm +libelf +libexpat +libstdcpp \
	   +libzstd +zlib @MESA_USE_LLVM
endef


define Package/libopencl-amd
$(call Package/libopencl/Default)
  TITLE+= (AMD)
  VARIANT:=amd
endef

define Package/libopencl-amd/description
Mesa Clover OpenCL 1.1 library for AMD GPUs.
endef


define Package/libopencl-nouveau
$(call Package/libopencl/Default)
  TITLE+= (nVidia)
  VARIANT:=nouveau
endef

define Package/libopencl-nouveau/description
Mesa Clover OpenCL 1.1 library for nVidia GPUs.
endef

define Package/libvulkan-broadcom
$(call Package/mesa/Default)
  DEPENDS+=+libdrm +libexpat +libstdcpp +libudev +libwayland +libzstd +zlib \
	   @(arm||aarch64) @HAS_FPU
  TITLE+= Broadcom Vulkan driver
  VARIANT:=vulkan
endef

define Package/libvulkan-broadcom/description
V3DV Vulkan graphics driver, notably used on the Raspberry Pi 4 and
Raspberry Pi 5.
endef

define Package/libvulkan-imagination
$(call Package/mesa/Default)
  DEPENDS+=+libatomic +libdrm +libexpat +libstdcpp +libudev +libwayland +libzstd \
	   +zlib @(aarch64||arm||mips||mips64||mipsel||mips64el||riscv64)
  TITLE+= Imagination PowerVR driver (EXPERIMENTAL)
  VARIANT:=vulkan
endef

define Package/libvulkan-imagination/description
PowerVR is a Vulkan driver for Imagination Technologies PowerVR GPUs,
starting with those based on the Rogue architecture.

The driver is not yet fully conformant so it requires exporting
PVR_I_WANT_A_BROKEN_VULKAN_DRIVER=1 to the environment before running
any Vulkan content.

The following hardware is currently in active development:

Product         Series          B.V.N.C         Vulkan  Conformant
GX6250          Series 6XT      4.40.2.51       1.0     No
AXE-1-16M       A-Series        33.15.11.3      1.0     No
BXS-4-64        B-Series        36.53.104.796   1.0     No
endef

define Package/libvulkan-intel
$(call Package/mesa/Default)
  DEPENDS+=+libdrm-intel +libexpat +libstdcpp +libudev +libwayland +libzstd +zlib \
	   @(i386||i686||x86_64) @MESA_USE_LLVM
  TITLE+= Intel Gen9+ Vulkan driver
  VARIANT:=vulkan
endef

define Package/libvulkan-intel/description
Intel ANV for Skylake "Gen9" and newer graphics. It's officially
supported by Intel and is their official Vulkan driver for Linux.
endef

define Package/libvulkan-intel-hasvk
$(call Package/mesa/Default)
  DEPENDS+=+libdrm-intel +libexpat +libstdcpp +libudev +libwayland +libzstd +zlib \
	   @(i386||i686||x86_64) @MESA_USE_LLVM
  TITLE+= Intel Gen7/Gen8 Vulkan driver
  VARIANT:=vulkan
endef

define Package/libvulkan-intel-hasvk/description
The ANV vulkan driver supports Intel's Gen 7/8 hardware. It's officially
supported by Intel and is their official Vulkan driver for Linux.
endef

define Package/libvulkan-lvp
$(call Package/mesa/Default)
  DEPENDS+=+libatomic +libdrm +libstdcpp +libudev +libwayland +libzstd +zlib @MESA_USE_LLVM
  TITLE+= LLVM JIT CPU rendering Vulkan driver
  VARIANT:=vulkan
endef

define Package/libvulkan-lvp/description
The Vulkan LLVMpipe driver is a software rasterizer that uses LLVM to do
runtime code generation.
endef

define Package/libvulkan-nouveau
$(call Package/mesa/Default)
  TITLE+= nouveau Vulkan driver for nVidia GPUs
  DEPENDS+=$(RUST_ARCH_DEPENDS) @MESA_USE_LLVM @BROKEN
  VARIANT:=vulkan
# needs rust-bindgen which currently can't be built
endef

define Package/libvulkan-nouveau/description
NVK is a Vulkan driver for NVIDIA GPUs.

NVK currently supports Turing (RTX 20XX and GTX 16XX) and later GPUs.
Eventually, we plan to support as far back as Kepler (GeForce 600 and
700 series) GPUs but anything pre-Turing is currently disabled by
default.
endef

define Package/libvulkan-panfrost
$(call Package/mesa/Default)
  DEPENDS+=+libdrm +libexpat +libstdcpp +libudev +libwayland +libzstd +zlib \
	   @(arm||aarch64) @MESA_USE_LLVM
  TITLE+= ARM Mali Midgard/Bifrost/Valhall Vulkan driver
  VARIANT:=vulkan
endef

define Package/libvulkan-panfrost/description
The PanVk driver is an Open Source Vulkan driver for ARM Mali Midgard
and Bifrost GPUs.
endef

define Package/libvulkan-radeon
$(call Package/mesa/Default)
  TITLE+= AMD Radeon Vulkan driver
  DEPENDS+=+libdrm-amdgpu +libelf +libexpat +libstdcpp +libudev +libwayland \
	   +libzstd +zlib @MESA_USE_LLVM
  VARIANT:=vulkan
endef

define Package/libvulkan-radeon/description
RADV is a Vulkan driver for AMD GCN/RDNA GPUs.
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
	-Dtools=nir \
	-Dllvm=enabled \
	-Dmesa-clc=enabled \
	-Dprecomp-compiler=enabled \
	-Dshared-llvm=disabled \
	-Ddraw-use-llvm=false \
	-Dintel-clc=enabled \
	-Dintel-elk=true \
	-Dstatic-libclc=all \
	-Dinstall-intel-clc=true \
	-Dinstall-mesa-clc=true \
	-Dinstall-precomp-compiler=true

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
	-Dllvm=$(if $(CONFIG_MESA_USE_LLVM),en,dis)abled \
	-Dgallium-opencl=$(if $(CONFIG_MESA_USE_LLVM),standalone,disabled) \
	-Ddraw-use-llvm=$(if $(CONFIG_MESA_USE_LLVM),true,false) \
	-Dintel-clc=system \
	-Dmesa-clc=$(if $(CONFIG_MESA_USE_LLVM),system,auto) \
	-Dprecomp-compiler=system

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
	LLVM_BUILDDIR=$(lastword $(wildcard $(BUILD_DIR_HOST)/llvm-mesa/llvm-project-*)) ; LLVM_VER=$$$${LLVM_BUILDDIR##*llvm-project-} ; LLVM_VER=$$$${LLVM_VER%%.src} ;\
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

define Package/libmesa/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*GL*.so* $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libgbm.so* $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libgallium*.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/gbm
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/gbm/*.so $(1)/usr/lib/gbm
	if [ -e "$(PKG_INSTALL_DIR)/usr/lib/dri" ]; then \
		$(INSTALL_DIR) $(1)/usr/lib/dri ; \
		$(CP) $(PKG_INSTALL_DIR)/usr/lib/dri/*.so $(1)/usr/lib/dri ; \
	fi
	$(INSTALL_DIR) $(1)/usr/share/drirc.d
	$(CP) $(PKG_INSTALL_DIR)/usr/share/drirc.d/* $(1)/usr/share/drirc.d
endef

define Package/libopencl/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libOpenCL.so* $(1)/usr/lib
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

Package/libopencl-amd/install = $(Package/libopencl/install)
Package/libopencl-nouveau/install = $(Package/libopencl/install)

$(eval $(call BuildPackage,mesa3d))
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
$(eval $(call BuildPackage,libopencl-nouveau))
$(eval $(call BuildPackage,libvulkan-broadcom))
$(eval $(call BuildPackage,libvulkan-imagination))
$(eval $(call BuildPackage,libvulkan-intel))
$(eval $(call BuildPackage,libvulkan-intel-hasvk))
$(eval $(call BuildPackage,libvulkan-lvp))
$(eval $(call BuildPackage,libvulkan-nouveau))
$(eval $(call BuildPackage,libvulkan-panfrost))
$(eval $(call BuildPackage,libvulkan-radeon))
$(eval $(call HostBuild))
