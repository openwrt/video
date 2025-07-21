include $(TOPDIR)/rules.mk

PKG_NAME:=llvm-project
PKG_VERSION:=20.1.8

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).src.tar.xz
PKG_SOURCE_URL:=https://github.com/llvm/llvm-project/releases/download/llvmorg-$(PKG_VERSION)
PKG_HASH:=6898f963c8e938981e6c4a302e83ec5beb4630147c7311183cf61069af16333d
PKG_CPE_ID:=cpe:/a:llvm:llvm

SPIRV_LLVM_TRANSLATOR_VERSION:=20.1.4
SPIRV_LLVM_TRANSLATOR_FILE:=SPIRV-LLVM-Translator-$(SPIRV_LLVM_TRANSLATOR_VERSION).tar.gz
SPIRV_LLVM_TRANSLATOR_URL:=https://codeload.github.com/KhronosGroup/SPIRV-LLVM-Translator/tar.gz/v$(SPIRV_LLVM_TRANSLATOR_VERSION)?
SPIRV_LLVM_TRANSLATOR_HASH:=f6c414f9384c8f68775d9461e55e7c492f01759f5a1754f30dad4cbfb049c7d7

HOST_BUILD_DIR:=$(BUILD_DIR_HOST)/llvm-mesa/$(PKG_NAME)-$(PKG_VERSION).src
PKG_BUILD_DIR:=$(BUILD_DIR)/llvm-mesa/$(PKG_NAME)-$(PKG_VERSION).src

CMAKE_BINARY_SUBDIR:=build
CMAKE_SOURCE_SUBDIR:=llvm

HOST_BUILD_DEPENDS:=spirv-headers/host spirv-tools/host
PKG_BUILD_DEPENDS:=llvm/host spirv-headers spirv-tools

CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/cmake.mk

define Download/spirv-llvm-translator
  FILE:=$(SPIRV_LLVM_TRANSLATOR_FILE)
  URL:=$(SPIRV_LLVM_TRANSLATOR_URL)
  HASH:=$(SPIRV_LLVM_TRANSLATOR_HASH)
endef

ifneq ($(HOST_OS),Linux)
  HOST_CFLAGS := $(filter-out -I$(STAGING_DIR_HOST)/include,$(HOST_CFLAGS))
  HOST_CXXFLAGS := $(filter-out -I$(STAGING_DIR_HOST)/include,$(HOST_CXXFLAGS))
endif

LLVM_MESA_PREFIX = llvm-mesa-$(PKG_VERSION).$(HOST_OS)-$(HOST_ARCH)

CMAKE_HOST_INSTALL_PREFIX = $(STAGING_DIR_HOST)/$(LLVM_MESA_PREFIX)

CMAKE_HOST_OPTIONS += \
	-DBUILD_SHARED_LIBS=OFF \
	-DCMAKE_SKIP_RPATH=OFF \
	-DCOMPILER_RT_BUILD_SANITIZERS=OFF \
	-DCOMPILER_RT_BUILD_XRAY=OFF \
	-DCOMPILER_RT_BUILD_LIBFUZZER=OFF \
	-DCOMPILER_RT_BUILD_PROFILE=OFF \
	-DCOMPILER_RT_BUILD_MEMPROF=OFF \
	-DLIBCLC_TARGETS_TO_BUILD= \
	-DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD= \
	-DLLVM_TARGETS_TO_BUILD= \
	-DLLVM_ENABLE_PROJECTS="libclc;clang" \
	-DLLVM_INCLUDE_BENCHMARKS=OFF \
	-DLLVM_INCLUDE_DOCS=OFF \
	-DLLVM_INCLUDE_EXAMPLES=OFF \
	-DLLVM_INCLUDE_TESTS=OFF \
	-DLLVM_ENABLE_ASSERTIONS=OFF \
	-DLLVM_ENABLE_BINDINGS=OFF \
	-DLLVM_ENABLE_IDE=OFF \
	-DLLVM_ENABLE_LIBEDIT=OFF \
	-DLLVM_ENABLE_LIBPFM=OFF \
	-DLLVM_ENABLE_LIBXML2=OFF \
	-DLLVM_ENABLE_OCAMLDOC=OFF \
	-DLLVM_ENABLE_TERMINFO=OFF \
	-DLLVM_ENABLE_Z3_SOLVER=OFF \
	-DLLVM_ENABLE_ZLIB=OFF \
	-DLLVM_ENABLE_ZSTD=OFF \
	-DLLVM_PARALLEL_LINK_JOBS=1 \
	-DLLVM_EXTERNAL_SPIRV_HEADERS_SOURCE_DIR=$(STAGING_DIR_HOSTPKG) \
	-DLLVM_SPIRV_INCLUDE_TESTS=OFF

LLVM_TARGET_TRIPLE:=$(subst muslgnueabi,musleabi,$(subst openwrt,unknown,$(REAL_GNU_TARGET_NAME)))

LLVM_TARGET_ARCH:=$(ARCH)
LLVM_TARGETS_TO_BUILD:=
LLVM_EXPERIMENTAL_TARGETS_TO_BUILD:=

ifeq ($(ARCH),i386)
  LLVM_TARGETS_TO_BUILD:=X86
else
ifeq ($(ARCH),i586)
  LLVM_TARGETS_TO_BUILD:=X86
else
ifeq ($(ARCH),i686)
  LLVM_TARGETS_TO_BUILD:=X86
else
ifeq ($(ARCH),x86_64)
  LLVM_TARGETS_TO_BUILD:=X86
else
ifeq ($(ARCH),aarch64)
  LLVM_TARGETS_TO_BUILD:=AArch64
else
ifneq ($(findstring arm,$(ARCH)),)
  LLVM_TARGETS_TO_BUILD:=ARM
else
ifeq ($(ARCH),arc)
  LLVM_EXPERIMENTAL_TARGETS_TO_BUILD:=ARC
else
ifneq ($(findstring loongarch,$(ARCH)),)
  LLVM_TARGETS_TO_BUILD:=LoongArch
else
ifneq ($(findstring mips,$(ARCH)),)
  LLVM_TARGETS_TO_BUILD:=Mips
  TARGET_LDFLAGS+=-latomic
else
ifneq ($(findstring powerpc,$(ARCH)),)
  LLVM_TARGETS_TO_BUILD:=PowerPC
else
ifneq ($(findstring riscv,$(ARCH)),)
  LLVM_TARGETS_TO_BUILD:=RISCV
else
  LLVM_TARGETS_TO_BUILD:=$(LLVM_TARGET_ARCH)
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif

# always include GPU targets
LLVM_TARGETS_TO_BUILD+=AMDGPU
LLVM_TARGETS_TO_BUILD+=NVPTX

CMAKE_OPTIONS += \
	-DBUILD_SHARED_LIBS=OFF \
	-DCMAKE_CROSSCOMPILING=True \
	-DCMAKE_SKIP_RPATH=OFF \
	-DCOMPILER_RT_BUILD_SANITIZERS=OFF \
	-DCOMPILER_RT_BUILD_XRAY=OFF \
	-DCOMPILER_RT_BUILD_LIBFUZZER=OFF \
	-DCOMPILER_RT_BUILD_PROFILE=OFF \
	-DCOMPILER_RT_BUILD_MEMPROF=OFF \
	-DLIBCLC_CUSTOM_LLVM_TOOLS_BINARY_DIR="$(STAGING_DIR_HOST)/llvm-mesa/bin" \
	-DLIBCLC_TARGETS_TO_BUILD="amdgcn--;amdgcn--amdhsa;clspv--;clspv64--;r600--;nvptx--;nvptx64--;nvptx--nvidiacl;nvptx64--nvidiacl;amdgcn-mesa-mesa3d;spirv-mesa3d-;spirv64-mesa3d-" \
	-DLLVM_TARGET_ARCH=$(LLVM_TARGET_ARCH) \
	-DLLVM_TARGETS_TO_BUILD="$(subst $(space),;,$(LLVM_TARGETS_TO_BUILD))" \
	-DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="$(subst $(space),;,$(LLVM_EXPERIMENTAL_TARGETS_TO_BUILD))" \
	-DLLVM_DEFAULT_TARGET_TRIPLE=$(LLVM_TARGET_TRIPLE) \
	-DLLVM_HOST_TRIPLE=$(LLVM_TARGET_TRIPLE) \
	-DLLVM_NATIVE_TOOL_DIR="$(STAGING_DIR_HOST)/llvm-mesa/bin" \
	-DLLVM_ENABLE_PROJECTS="libclc;clang" \
	-DLLVM_INCLUDE_BENCHMARKS=OFF \
	-DLLVM_INCLUDE_DOCS=OFF \
	-DLLVM_INCLUDE_EXAMPLES=OFF \
	-DLLVM_INCLUDE_TESTS=OFF \
	-DLLVM_ENABLE_ASSERTIONS=OFF \
	-DLLVM_ENABLE_BINDINGS=OFF \
	-DLLVM_ENABLE_IDE=OFF \
	-DLLVM_ENABLE_LIBEDIT=OFF \
	-DLLVM_ENABLE_LIBPFM=OFF \
	-DLLVM_ENABLE_LIBXML2=OFF \
	-DLLVM_ENABLE_OCAMLDOC=OFF \
	-DLLVM_ENABLE_RTTI=ON \
	-DLLVM_ENABLE_TERMINFO=OFF \
	-DLLVM_ENABLE_Z3_SOLVER=OFF \
	-DLLVM_ENABLE_ZLIB=OFF \
	-DLLVM_ENABLE_ZSTD=OFF \
	-DLLVM_PARALLEL_LINK_JOBS=1 \
	-DLLVM_EXTERNAL_SPIRV_HEADERS_SOURCE_DIR=$(STAGING_DIR)/usr \
	-DLLVM_SPIRV_INCLUDE_TESTS=OFF

define Package/llvm
  SECTION:=devel
  CATEGORY:=Development
  TITLE:=LLVM
  URL:=https://llvm.org/
  DEPENDS:=@(!arc&&!powerpc64&&!powerpc||BROKEN)
  BUILDONLY:=1
endef

define Package/llvm/description
The LLVM Project is a collection of modular and reusable compiler and toolchain technologies.
endef

define Package/libclang
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=LLVM Clang library
  URL:=https://llvm.org/
  DEPENDS:=@(!arc&&!powerpc64&&!powerpc||BROKEN) +libatomic +libstdcpp
endef

define Package/libclang-cpp
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=LLVM Clang C++ library
  URL:=https://llvm.org/
  DEPENDS:=@(!arc&&!powerpc64&&!powerpc||BROKEN) +libatomic +libstdcpp
endef

define Package/liblto
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=LLVM LTO library
  URL:=https://llvm.org/
  DEPENDS:=@(!arc&&!powerpc64&&!powerpc||BROKEN) +libatomic +libstdcpp
endef

define Package/libremarks
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=LLVM diagnostic remarks library
  URL:=https://llvm.org/
  DEPENDS:=@(!arc&&!powerpc64&&!powerpc||BROKEN) +libatomic +libstdcpp
endef

define Host/Prepare
	$(call Host/Prepare/Default)
	$(STAGING_DIR_HOST)/bin/libdeflate-gzip -dc $(DL_DIR)/$(SPIRV_LLVM_TRANSLATOR_FILE) | $(TAR) -C $(HOST_BUILD_DIR)/llvm/projects $(TAR_OPTIONS)
endef

define Host/Install
	rm -rf $(STAGING_DIR_HOST)/llvm-mesa*
	$(Host/Install/Default)
	for file in \
		clang-diff clang-fuzzer-dictionary count FileCheck lli-child-target \
		llvm-isel-fuzzer llvm-jitlink-executor llvm-lit llvm-locstats llvm-min-tblgen \
		llvm-opt-fuzzer llvm-PerfectShuffle not obj2yaml prepare_builtins split-file \
		UnicodeNameMappingGenerator yaml2obj yaml-bench \
		; do \
		$(INSTALL_BIN) $(HOST_BUILD_DIR)/build/bin/$$$${file} $(CMAKE_HOST_INSTALL_PREFIX)/bin ; \
	done
	ln -s $(LLVM_MESA_PREFIX) $(STAGING_DIR_HOST)/llvm-mesa
	STRIP_KMOD= PATCHELF= STRIP=strip $(SCRIPT_DIR)/rstrip.sh $(STAGING_DIR_HOST)/llvm-mesa
	echo "$(PKG_VERSION)" > $(CMAKE_HOST_INSTALL_PREFIX)/.llvm-version
	$(INSTALL_DIR) $(STAGING_DIR_HOSTPKG)/lib/pkgconfig
	$(INSTALL_DATA) $(STAGING_DIR_HOST)/llvm-mesa/share/pkgconfig/libclc.pc $(STAGING_DIR_HOSTPKG)/lib/pkgconfig
	$(INSTALL_DATA) $(STAGING_DIR_HOST)/llvm-mesa/lib/pkgconfig/LLVMSPIRVLib.pc $(STAGING_DIR_HOSTPKG)/lib/pkgconfig
	$(INSTALL_DATA) $(STAGING_DIR_HOST)/llvm-mesa/lib/libLLVMSPIRVLib.a $(STAGING_DIR_HOSTPKG)/lib
endef

define Host/Uninstall
	rm -rf $(STAGING_DIR_HOST)/llvm-mesa*
	rm -rf $(STAGING_DIR_HOSTPKG)/include/clc
	rm -rf $(STAGING_DIR_HOSTPKG)/share/clc
	rm -f $(STAGING_DIR_HOSTPKG)/lib/pkgconfig/libclc.pc
	rm -f $(STAGING_DIR_HOSTPKG)/lib/pkgconfig/LLVMSPIRVLib.pc
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(STAGING_DIR_HOST)/bin/libdeflate-gzip -dc $(DL_DIR)/$(SPIRV_LLVM_TRANSLATOR_FILE) | $(TAR) -C $(PKG_BUILD_DIR)/llvm/projects $(TAR_OPTIONS)
endef

define Build/Install
	$(call Build/Install/Default)
# hack: install SPIRV OpenCL libs also to host build to avoid having to build LLVM three times
	$(INSTALL_DIR) $(CMAKE_HOST_INSTALL_PREFIX)/share/clc
	$(CP) $(PKG_INSTALL_DIR)/usr/share/clc/spirv{,64}-mesa3d-.spv $(CMAKE_HOST_INSTALL_PREFIX)/share/clc
endef

define Package/libclang/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libclang.so* $(1)/usr/lib
endef

define Package/libclang-cpp/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libclang-cpp.so* $(1)/usr/lib
endef

define Package/liblto/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libLTO.so* $(1)/usr/lib
endef

define Package/libremarks/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libRemarks.so* $(1)/usr/lib
endef

$(eval $(call Download,spirv-llvm-translator))
$(eval $(call BuildPackage,llvm))
$(eval $(call BuildPackage,libclang))
$(eval $(call BuildPackage,libclang-cpp))
$(eval $(call BuildPackage,liblto))
$(eval $(call BuildPackage,libremarks))
$(eval $(call HostBuild))
