#
# Copyright (C) 2026 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt6declarative
PKG_RELEASE:=1
PKG_HASH:=5a071b227229afbf5c976b7b59a0d850818d06ae861fcdf6d690351ca3f8a260

PKG_MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
PKG_LICENSE:=LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
# Qt's own licenses, followed by those of the bundled third-party code
# (masm in QtQml, the Material style's elevation effect)
PKG_LICENSE_FILES:= \
  LICENSES/LicenseRef-Qt-Commercial.txt \
  LICENSES/LGPL-3.0-only.txt \
  LICENSES/GPL-2.0-only.txt \
  LICENSES/GPL-3.0-only.txt \
  LICENSES/BSD-2-Clause.txt \
  LICENSES/MIT.txt

PKG_BUILD_DEPENDS:=qt6quick/host qt6shadertools
HOST_BUILD_DEPENDS:=qt6shadertools/host qt6svg/host # qt6svg/host is an optional dependency, needed to build tool "svgtoqml" needed for svg support for QtQuick required by e.g. QtVirtualkeyboard

QT_MODULE:=qtdeclarative

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/nls.mk
include ../cmake.mk

PKG_CONFIG_DEPENDS:= \
    CONFIG_BUILD_qt6base-gui_OPENGL_OPENGLES2 \
    CONFIG_BUILD_qt6base-network_SSL \
    CONFIG_PACKAGE_qt6quick-widgets

# qtdeclarative builds parts depending on which optional Qt modules and
# libraries it finds, so the lookup of each of them is required or disabled
# explicitly.
CMAKE_OPTIONS += \
	-DCMAKE_REQUIRE_FIND_PACKAGE_Qt6Gui=ON \
	-DCMAKE_REQUIRE_FIND_PACKAGE_Qt6Network=ON \
	-DCMAKE_REQUIRE_FIND_PACKAGE_Qt6Test=ON \
	-DCMAKE_REQUIRE_FIND_PACKAGE_Qt6ShaderTools=ON \
	-DCMAKE_REQUIRE_FIND_PACKAGE_Qt6ShaderToolsTools=ON \
	-DCMAKE_REQUIRE_FIND_PACKAGE_Qt6OpenGL=$(if $(CONFIG_BUILD_qt6base-gui_OPENGL_OPENGLES2),ON,OFF) \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6OpenGL=$(if $(CONFIG_BUILD_qt6base-gui_OPENGL_OPENGLES2),OFF,ON) \
	-DCMAKE_REQUIRE_FIND_PACKAGE_Qt6Widgets=$(if $(CONFIG_PACKAGE_qt6quick-widgets),ON,OFF) \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Widgets=$(if $(CONFIG_PACKAGE_qt6quick-widgets),OFF,ON) \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6OpenGLWidgets=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Concurrent=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Sql=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Svg=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6LinguistTools=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6LanguageServerPrivate=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6TestInternalsPrivate=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6ExamplesAssetDownloaderPrivate=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_LTTngUST=ON

# All features are set explicitly, except for the QML JIT, which Qt enables
# depending on the target architecture only.
CMAKE_OPTIONS += \
	-DFEATURE_qml_network=ON \
	-DFEATURE_qml_ssl=$(if $(CONFIG_BUILD_qt6base-network_SSL),ON,OFF) \
	-DFEATURE_qml_debug=ON \
	-DFEATURE_qml_profiler=ON \
	-DFEATURE_qml_preview=ON \
	-DFEATURE_qml_xml_http_request=ON \
	-DFEATURE_qml_locale=ON \
	-DFEATURE_qml_animation=ON \
	-DFEATURE_qml_worker_script=ON \
	-DFEATURE_qml_itemmodel=ON \
	-DFEATURE_qml_xmllistmodel=ON \
	-DFEATURE_qml_type_loader_thread=ON \
	-DFEATURE_qml_python=ON \
	-DFEATURE_qml_object_model=ON \
	-DFEATURE_qml_list_model=ON \
	-DFEATURE_qml_delegate_model=ON \
	-DFEATURE_qml_table_model=ON \
	-DFEATURE_quick_animatedimage=ON \
	-DFEATURE_quick_canvas=ON \
	-DFEATURE_quick_designer=ON \
	-DFEATURE_quick_flipable=ON \
	-DFEATURE_quick_gridview=ON \
	-DFEATURE_quick_itemview=ON \
	-DFEATURE_quick_viewtransitions=ON \
	-DFEATURE_quick_listview=ON \
	-DFEATURE_quick_tableview=ON \
	-DFEATURE_quick_treeview=ON \
	-DFEATURE_quick_particles=ON \
	-DFEATURE_quick_path=ON \
	-DFEATURE_quick_pathview=ON \
	-DFEATURE_quick_positioners=ON \
	-DFEATURE_quick_repeater=ON \
	-DFEATURE_quick_shadereffect=ON \
	-DFEATURE_quick_sprite=ON \
	-DFEATURE_quick_draganddrop=ON \
	-DFEATURE_quick_pixmap_cache_threaded_download=ON \
	-DFEATURE_quickcontrols2_basic=ON \
	-DFEATURE_quickcontrols2_fusion=ON \
	-DFEATURE_quickcontrols2_imagine=ON \
	-DFEATURE_quickcontrols2_material=ON \
	-DFEATURE_quickcontrols2_universal=ON \
	-DFEATURE_quickcontrols2_fluentwinui3=ON \
	-DFEATURE_quickcontrols2_macos=OFF \
	-DFEATURE_quickcontrols2_ios=OFF \
	-DFEATURE_quickcontrols2_windows=OFF \
	-DFEATURE_quicktemplates2_hover=ON \
	-DFEATURE_quicktemplates2_multitouch=ON \
	-DFEATURE_quicktemplates2_calendar=ON \
	-DFEATURE_quicktemplates2_container=ON

# Svg is needed on the host for the svgtoqml tool.
CMAKE_HOST_OPTIONS += \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Widgets=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6OpenGLWidgets=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6Sql=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6LinguistTools=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6LanguageServerPrivate=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6TestInternalsPrivate=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_Qt6ExamplesAssetDownloaderPrivate=ON \
	-DCMAKE_DISABLE_FIND_PACKAGE_LTTngUST=ON

define Host/Compile
	$(call Host/Compile/Default, host_tools)
endef

# Only the host tools are built, so the regular install, which would build and
# install the whole module, is not used. The needed files are copied instead.
define Host/Install
	$(INSTALL_DIR) \
	  $(QT_HOSTPKG_BINS) \
	  $(QT_HOSTPKG_LIBS) \
	  $(QT_HOSTPKG_LIBEXECS) \
	  $(QT_HOSTPKG_MKSPECS) \
	  $(QT_HOSTPKG_CMAKES) \
	  $(QT_HOSTPKG_PKGCONFIGS)

	$(CP) \
	  $(HOST_BUILD_DIR)/$(subst $(QT_HOSTPKG_PREFIX)/,,$(QT_HOSTPKG_BINS))/* \
	  $(QT_HOSTPKG_BINS)/

	# the libraries the tools link against (e.g. libQt6QmlCompiler)
	$(CP) \
	  $(HOST_BUILD_DIR)/$(subst $(QT_HOSTPKG_PREFIX)/,,$(QT_HOSTPKG_LIBS))/libQt6*.so* \
	  $(QT_HOSTPKG_LIBS)/

	$(CP) \
	  $(HOST_BUILD_DIR)/$(subst $(QT_HOSTPKG_PREFIX)/,,$(QT_HOSTPKG_LIBEXECS))/* \
	  $(QT_HOSTPKG_LIBEXECS)/

	$(CP) \
	  $(HOST_BUILD_DIR)/$(subst $(QT_HOSTPKG_PREFIX)/,,$(QT_HOSTPKG_MKSPECS))/* \
	  $(QT_HOSTPKG_MKSPECS)/

	$(CP) \
	  $(HOST_BUILD_DIR)/$(subst $(QT_HOSTPKG_PREFIX)/,,$(QT_HOSTPKG_CMAKES))/{Qt6QuickTools,Qt6QmlTools} \
	  $(QT_HOSTPKG_CMAKES)/

	# The *Targets.cmake files above are the build tree's ones, which refer to
	# the tools in HOST_BUILD_DIR. Replace them by the ones CMake generates for
	# the install tree.
	$(CP) \
	  $(HOST_BUILD_DIR)/CMakeFiles/Export/*/Qt6QuickToolsTargets*.cmake \
	  $(QT_HOSTPKG_CMAKES)/Qt6QuickTools/

	$(CP) \
	  $(HOST_BUILD_DIR)/CMakeFiles/Export/*/Qt6QmlToolsTargets*.cmake \
	  $(QT_HOSTPKG_CMAKES)/Qt6QmlTools/

	$(CP) \
	  $(HOST_BUILD_DIR)/$(subst $(QT_HOSTPKG_PREFIX)/,,$(QT_HOSTPKG_PKGCONFIGS))/Qt6*.pc \
	  $(QT_HOSTPKG_PKGCONFIGS)/
endef

define Package/qt6quick/Default
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Qt6Quick
  URL:=https://www.qt.io
endef

define Package/qt6quick-lib/Default
  $(call Package/qt6quick/Default)
endef

define Package/qt6quick-qmllib/Default
  $(call Package/qt6quick/Default)
  TITLE+=Qml
endef

define Package/qt6quick-qmlmod/Default
  $(call Package/qt6quick/Default)
  TITLE+=Qml module
endef

define Package/qt6quick
  $(call Package/qt6quick-lib/Default)
  TITLE+=core
  DEPENDS+=+qt6base-gui +BUILD_qt6base-gui_OPENGL_OPENGLES2:qt6base-opengl +qt6quick-qml-meta
endef

define Package/qt6quick-controls2
  $(call Package/qt6quick-lib/Default)
  TITLE+=controls
  DEPENDS+=+qt6quick-templates2
endef

define Package/qt6quick-dialogs2
  $(call Package/qt6quick-lib/Default)
  TITLE+=dialogs
  DEPENDS+=+qt6quick-templates2 +qt6quick-controls2 # only needs libQt6QuickControls2Impl.so.6 from quick-controls2, but not separately packaged (yet)
endef

define Package/qt6quick-effects
  $(call Package/qt6quick-lib/Default)
  TITLE+=effects
  DEPENDS+=+qt6quick
endef

define Package/qt6quick-layouts
  $(call Package/qt6quick-lib/Default)
  TITLE+=layouts
  DEPENDS+=+qt6quick
endef

define Package/qt6quick-particles
  $(call Package/qt6quick-lib/Default)
  TITLE+=particles
  DEPENDS+=+qt6quick
endef

define Package/qt6quick-shapes
  $(call Package/qt6quick-lib/Default)
  TITLE+=shapes
  DEPENDS+=+qt6quick
endef

define Package/qt6quick-templates2
  $(call Package/qt6quick-lib/Default)
  TITLE+=templates
  DEPENDS+=+qt6quick
endef

define Package/qt6quick-test
  $(call Package/qt6quick-lib/Default)
  TITLE+=test
  DEPENDS+=+qt6quick +qt6base-test
endef

define Package/qt6quick-widgets
  $(call Package/qt6quick-lib/Default)
  TITLE+=widgets
  DEPENDS+=+qt6quick +qt6base-widgets
endef

###

define Package/qt6quick-qml
  $(call Package/qt6quick-qmllib/Default)
  TITLE+=core
  DEPENDS+=+qt6base-network
endef

define Package/qt6quick-qml-core
  $(call Package/qt6quick-qmllib/Default)
  TITLE+=core
  DEPENDS+=+qt6quick-qml
endef

define Package/qt6quick-qml-meta
  $(call Package/qt6quick-qmllib/Default)
  TITLE+=meta
  DEPENDS+=+qt6quick-qml-workerscript +qt6quick-qml-models
endef

define Package/qt6quick-qml-models
  $(call Package/qt6quick-qmllib/Default)
  TITLE+=models
  DEPENDS+=+qt6quick-qml
endef

define Package/qt6quick-qml-network
  $(call Package/qt6quick-qmllib/Default)
  TITLE+=network
  DEPENDS+=+qt6quick-qml
endef

define Package/qt6quick-qml-workerscript
  $(call Package/qt6quick-qmllib/Default)
  TITLE+=workerscript
  DEPENDS+=+qt6quick-qml
endef

define Package/qt6quick-qml-xmllistmodel
  $(call Package/qt6quick-qmllib/Default)
  TITLE+=xmllistmodel
  DEPENDS+=+qt6quick-qml-models
endef

define Package/qt6labs-folderlistmodel
  $(call Package/qt6quick-qmllib/Default)
  TITLE:=Qt6 Labs FolderListModel library
  DEPENDS:= \
        +qt6base-core \
        +qt6base-network \
        +qt6quick-qml \
        +qt6quick-qml-models \
        +libstdcpp
endef

###

define Package/qt6quick-qml-mod-qtcore
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=core
  DEPENDS+=+qt6quick-qml-core
endef

define Package/qt6quick-qml-mod-qtnetwork
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=network
  DEPENDS+=+qt6quick-qml-network
endef

define Package/qt6quick-qml-mod-qtqml-models
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=models
  DEPENDS+=+qt6quick-qml-models
endef

define Package/qt6quick-qml-mod-qtqml-workerscript
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=workerscript
  DEPENDS+=+qt6quick-qml-workerscript
endef

define Package/qt6quick-qml-mod-qtqml-xmllistmodel
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=xmllistmodel
  DEPENDS+=+qt6quick-qml-xmllistmodel
endef

define Package/qt6quick-qml-mod-qtlabs-folderlistmodel
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=QtLabs folderlistmodel
  DEPENDS+=+qt6labs-folderlistmodel
endef

define Package/qt6quick-qml-mod-qtquick-controls
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=controls
  DEPENDS+= \
	+qt6quick-controls2 \
	+qt6quick-qml-mod-qtquick-effects \
	+qt6quick-qml-mod-qtquick-layouts \
	+qt6quick-qml-mod-qtquick-shapes \
	+qt6quick-qml-mod-qtquick-templates \
	+qt6quick-qml-mod-qtquick-window
endef

define Package/qt6quick-qml-mod-qtquick-dialogs
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=dialogs
  DEPENDS+= \
	+qt6quick-dialogs2 \
	+qt6quick-qml-mod-qtlabs-folderlistmodel \
	+qt6quick-qml-mod-qtquick-controls
endef

define Package/qt6quick-qml-mod-qtquick-effects
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=effects
  DEPENDS+=+qt6quick-effects
endef

define Package/qt6quick-qml-mod-qtquick-layouts
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=layouts
  DEPENDS+=+qt6quick-layouts
endef

define Package/qt6quick-qml-mod-qtquick-particles
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=particles
  DEPENDS+=+qt6quick-particles
endef

define Package/qt6quick-qml-mod-qtquick-shapes
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=shapes
  DEPENDS+=+qt6quick-shapes
endef

define Package/qt6quick-qml-mod-qtquick-templates
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=templates
  DEPENDS+=+qt6quick-templates2
endef

define Package/qt6quick-qml-mod-qtquick-window
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=window
  DEPENDS+=+qt6quick-qml
endef

####

Build/InstallDev = $(Build/InstallDev/Qt)

define Package/qt6quick/install
	$(call Build/Install/Libs,$(1),libQt6Quick)
endef

define Package/qt6quick-controls2/install
	$(call Build/Install/Libs,$(1),libQt6QuickControls2*) # all
endef

define Package/qt6quick-dialogs2/install
	$(call Build/Install/Libs,$(1),libQt6QuickDialogs2*) # all
endef

define Package/qt6quick-effects/install
	$(call Build/Install/Libs,$(1),libQt6QuickEffects)
endef

define Package/qt6quick-layouts/install
	$(call Build/Install/Libs,$(1),libQt6QuickLayouts)
endef

define Package/qt6quick-particles/install
	$(call Build/Install/Libs,$(1),libQt6QuickParticles)
endef

define Package/qt6quick-shapes/install
	$(call Build/Install/Libs,$(1),libQt6QuickShapes)
endef

define Package/qt6quick-templates2/install
	$(call Build/Install/Libs,$(1),libQt6QuickTemplates2)
endef

define Package/qt6quick-test/install
	$(call Build/Install/Libs,$(1),libQt6QuickTest)
endef

define Package/qt6quick-widgets/install
	$(call Build/Install/Libs,$(1),libQt6QuickWidgets)
endef

### libQt6Qml*.so*

define Package/qt6quick-qml/install
	$(call Build/Install/Libs,$(1),libQt6Qml)

	$(INSTALL_DIR) \
	  $(1)/$(QT_INSTALL_QML)

	$(CP) \
	  $(PKG_INSTALL_DIR)/$(QT_INSTALL_QML)/builtins.qmltypes \
	  $(1)/$(QT_INSTALL_QML)/
endef

define Package/qt6quick-qml-core/install
	$(call Build/Install/Libs,$(1),libQt6QmlCore)
endef

define Package/qt6quick-qml-meta/install
	$(call Build/Install/Libs,$(1),libQt6QmlMeta)
endef

define Package/qt6quick-qml-models/install
	$(call Build/Install/Libs,$(1),libQt6QmlModels)
endef

define Package/qt6quick-qml-network/install
	$(call Build/Install/Libs,$(1),libQt6QmlNetwork)
endef

define Package/qt6quick-qml-workerscript/install
	$(call Build/Install/Libs,$(1),libQt6QmlWorkerScript)
endef

define Package/qt6quick-qml-xmllistmodel/install
	$(call Build/Install/Libs,$(1),libQt6QmlXmlListModel)
endef

define Package/qt6labs-folderlistmodel/install
	$(call Build/Install/Libs,$(1),libQt6LabsFolderListModel)
endef

### QML plugin

define Package/qt6quick-qml-mod-qtcore/install
	$(call Build/Install/QMLplugin,$(1),QtCore,*)
endef

define Package/qt6quick-qml-mod-qtnetwork/install
	$(call Build/Install/QMLplugin,$(1),QtNetwork,*)
endef

define Package/qt6quick-qml-mod-qtqml-models/install
	$(call Build/Install/QMLplugin,$(1),QtQml/Models,*)
endef

define Package/qt6quick-qml-mod-qtqml-workerscript/install
	$(call Build/Install/QMLplugin,$(1),QtQml/WorkerScript,*)
endef

define Package/qt6quick-qml-mod-qtqml-xmllistmodel/install
	$(call Build/Install/QMLplugin,$(1),QtQml/XmlListModel,*)
endef

define Package/qt6quick-qml-mod-qtlabs-folderlistmodel/install
	$(call Build/Install/QMLplugin,$(1),Qt/labs/folderlistmodel,*)
endef

define Package/qt6quick-qml-mod-qtquick-controls/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Controls,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Controls/impl,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Controls/Basic,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Controls/FluentWinUI3,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Controls/Fusion,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Controls/Imagine,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Controls/Material,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Controls/Universal,*)
endef

define Package/qt6quick-qml-mod-qtquick-dialogs/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Dialogs,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Dialogs/quickimpl,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Dialogs/quickimpl/qml,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Dialogs/quickimpl/qml/+Fusion,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Dialogs/quickimpl/qml/+Imagine,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Dialogs/quickimpl/qml/+Material,*)
	$(call Build/Install/QMLplugin,$(1),QtQuick/Dialogs/quickimpl/qml/+Universal,*)
endef

define Package/qt6quick-qml-mod-qtquick-effects/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Effects,*)
endef

define Package/qt6quick-qml-mod-qtquick-layouts/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Layouts,*)
endef

define Package/qt6quick-qml-mod-qtquick-particles/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Particles,*)
endef

define Package/qt6quick-qml-mod-qtquick-shapes/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Shapes,*)
endef

define Package/qt6quick-qml-mod-qtquick-templates/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Templates,*)
endef

define Package/qt6quick-qml-mod-qtquick-window/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Window,*)
endef

$(eval $(call BuildPackage,qt6quick))
$(eval $(call BuildPackage,qt6quick-controls2))
$(eval $(call BuildPackage,qt6quick-dialogs2))
$(eval $(call BuildPackage,qt6quick-effects))
$(eval $(call BuildPackage,qt6quick-layouts))
$(eval $(call BuildPackage,qt6quick-particles))
$(eval $(call BuildPackage,qt6quick-shapes))
$(eval $(call BuildPackage,qt6quick-templates2))
$(eval $(call BuildPackage,qt6quick-test))
$(eval $(call BuildPackage,qt6quick-widgets))

$(eval $(call BuildPackage,qt6quick-qml))
$(eval $(call BuildPackage,qt6quick-qml-core))
$(eval $(call BuildPackage,qt6quick-qml-meta))
$(eval $(call BuildPackage,qt6quick-qml-models))
$(eval $(call BuildPackage,qt6quick-qml-network))
$(eval $(call BuildPackage,qt6quick-qml-workerscript))
$(eval $(call BuildPackage,qt6quick-qml-xmllistmodel))
$(eval $(call BuildPackage,qt6labs-folderlistmodel))

$(eval $(call BuildPackage,qt6quick-qml-mod-qtcore))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtnetwork))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtqml-models))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtqml-workerscript))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtqml-xmllistmodel))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtlabs-folderlistmodel))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtquick-controls))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtquick-dialogs))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtquick-effects))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtquick-layouts))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtquick-particles))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtquick-shapes))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtquick-templates))
$(eval $(call BuildPackage,qt6quick-qml-mod-qtquick-window))

$(eval $(call HostBuild))
