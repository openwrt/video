#
# Copyright (C) 2026 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt6tools
PKG_RELEASE:=1
PKG_HASH:=0cf7ab0e975fc57f5ce1375576a0a76e9ede25e6b01db3cf2339cd4d9750b4e9

PKG_MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
# license of the tools built (Qt Linguist tools, kmap2qmap)
PKG_LICENSE:=LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
PKG_LICENSE_FILES:= \
  LICENSES/LicenseRef-Qt-Commercial.txt \
  LICENSES/GPL-3.0-only.txt \
  LICENSES/Qt-GPL-exception-1.0.txt

PKG_BUILD_DEPENDS:=qt6base qt6tools/host
HOST_BUILD_DEPENDS:=qt6base/host

QT_MODULE:=qttools

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/nls.mk
include ../cmake.mk

# The tools are host tools: the host build provides the Qt Linguist tools
# (lupdate, lrelease, lconvert) and kmap2qmap. The target build compiles
# nothing, as a cross build takes the tools from the host build. Every other
# tool is disabled, as is the lookup of the optional modules and libraries
# they would otherwise be built against.
# lupdate's QML support would need Qml from qt6quick's host build.
QT6TOOLS_OPTIONS:= \
	-DFEATURE_linguist=ON \
	-DFEATURE_assistant=OFF \
	-DFEATURE_clang=OFF \
	-DFEATURE_clang_rtti=OFF \
	-DFEATURE_clangcpp=OFF \
	-DFEATURE_qdoc=OFF \
	-DFEATURE_designer=OFF \
	-DFEATURE_distancefieldgenerator=OFF \
	-DFEATURE_pixeltool=OFF \
	-DFEATURE_qdbus=OFF \
	-DFEATURE_qev=OFF \
	-DFEATURE_qtattributionsscanner=OFF \
	-DFEATURE_qtdiag=OFF \
	-DFEATURE_qtplugininfo=OFF \
	-DCMAKE_DISABLE_FIND_PACKAGE_WrapLibClang=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6DBus=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Network=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Xml=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Widgets=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Quick=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6QuickWidgets=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Qml=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6QmlLSPrivate=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Sql=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6PrintSupport=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6OpenGL=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6OpenGLWidgets=ON

CMAKE_OPTIONS += \
	$(QT6TOOLS_OPTIONS) \
	-DFEATURE_kmap2qmap=OFF \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6InputSupportPrivate=ON

CMAKE_HOST_OPTIONS += \
	$(QT6TOOLS_OPTIONS) \
	-DFEATURE_kmap2qmap=ON \
	-DCMAKE_REQUIRE_FIND_PACKAGE_Qt6InputSupportPrivate=ON

define Package/qt6tools
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Qt6Tools
  URL:=https://www.qt.io
  BUILDONLY:=1
endef

# The target build is still needed: it installs the CMake packages through
# which target builds use the tools, e.g. Qt6Linguist, without which
# qttranslations builds nothing.
Build/InstallDev = $(Build/InstallDev/Qt)

$(eval $(call BuildPackage,qt6tools))
$(eval $(call HostBuild))
