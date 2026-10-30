#
# Copyright (C) 2026 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt6declarative
PKG_RELEASE:=1
PKG_HASH:=a3175fa510847a136734f989e2bfea7f7bbb9dc9acc98b40b544d26f5ba20d3d

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
include $(INCLUDE_DIR)/cmake.mk
include ../cmake.mk

PKG_CONFIG_DEPENDS:= \
    CONFIG_BUILD_qt6base-gui_OPENGL_OPENGLES2

# needed for executables to load shared objects from $STAGING_DIR_HOSTPKG
HOST_LDFLAGS += -Wl,-rpath,$(STAGING_DIR_HOSTPKG)/lib

define Host/Compile
	$(call Host/Compile/Default, host_tools)
endef

define Host/Install
	$(INSTALL_DIR) \
	  $(QT_HOSTPKG_BINS) \
	  $(QT_HOSTPKG_LIBEXECS) \
	  $(QT_HOSTPKG_MKSPECS) \
	  $(QT_HOSTPKG_CMAKES) \
	  $(QT_HOSTPKG_PKGCONFIGS)

	$(CP) \
	  $(HOST_BUILD_DIR)/$(subst $(QT_HOSTPKG_PREFIX)/,,$(QT_HOSTPKG_BINS))/* \
	  $(QT_HOSTPKG_BINS)/

	$(CP) \
	  $(HOST_BUILD_DIR)/$(subst $(QT_HOSTPKG_PREFIX)/,,$(QT_HOSTPKG_LIBEXECS))/* \
	  $(QT_HOSTPKG_LIBEXECS)/

	$(CP) \
	  $(HOST_BUILD_DIR)/$(subst $(QT_HOSTPKG_PREFIX)/,,$(QT_HOSTPKG_MKSPECS))/* \
	  $(QT_HOSTPKG_MKSPECS)/

	$(CP) \
	  $(HOST_BUILD_DIR)/$(subst $(QT_HOSTPKG_PREFIX)/,,$(QT_HOSTPKG_CMAKES))/{Qt6QuickTools,Qt6QmlTools} \
	  $(QT_HOSTPKG_CMAKES)/

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
  DEPENDS+=+qt6quick-templates2 +qt6quick-controls2 # only needs libQt6QuickControls2Impl.so.6 from quick-controls2, but not seperately packaged (yet)
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
  TITLE+=network
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
  DEPENDS+=+qt6quick-controls2
endef

define Package/qt6quick-qml-mod-qtquick-dialogs
  $(call Package/qt6quick-qmlmod/Default)
  TITLE+=dialogs
  DEPENDS+=+qt6quick-dialogs2
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
