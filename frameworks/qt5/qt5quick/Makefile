#
# Copyright (C) 2020 OpenWrt
# Author: Mirko Vogt <mirko-openwrt@nanl.de>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5declarative
PKG_RELEASE:=1
PKG_HASH:=4e05de0747176a8a86a8c098ad215c2cdf1825a572b62a70b2e91cd81357408d

include ../common.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
include ../qmake.mk

define Package/qt5quick/Default
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Qt5Quick2
#  DEPENDS+=qt5quick
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>
endef

# define Package/qt5quick
#   $(call Package/qt5quick/Default)
#   DEPENDS:=
#   MENU:=1
# endef

define Package/qt5quick-qml
  $(call Package/qt5quick/Default)
  TITLE+=Qml
  DEPENDS+=+qt5base-network
endef

define Package/qt5quick-qml-models
  $(call Package/qt5quick/Default)
  TITLE+=QmlModels
  DEPENDS+=+qt5quick-qml
endef

define Package/qt5quick-qml-worker-script
  $(call Package/qt5quick/Default)
  TITLE+=QmlWorkerScript
  DEPENDS+=+qt5quick-qml
endef

define Package/qt5quick-quick
  $(call Package/qt5quick/Default)
  TITLE+=Quick
  DEPENDS+=+qt5base-gui +qt5quick-qml-models
endef

define Package/qt5quick-quick-test
  $(call Package/qt5quick/Default)
  TITLE+=QuickTest
  DEPENDS+=+qt5quick-quick +qt5base-widgets +qt5base-test
endef

#define Package/qt5quick-particles
#  $(call Package/qt5quick/Default)
#  TITLE+=particles
#  DEPENDS+=+qt5quick-quick
#endef

define Package/qt5quick-quick-widgets
  $(call Package/qt5quick/Default)
  TITLE+=QuickWidgets
  DEPENDS+=+qt5quick-quick +qt5base-widgets
endef

define Package/qt5quick-quick-shapes
  $(call Package/qt5quick/Default)
  TITLE+=QuickShapes
  DEPENDS+=+qt5quick-quick
endef

###

define Package/qt5quick-qml-module-qtqml-models2
  $(call Package/qt5quick/Default)
  TITLE+=QML QtQml models
  DEPENDS+=+qt5quick-qml-models
endef

define Package/qt5quick-qml-module-qtqml-statemachine
  $(call Package/qt5quick/Default)
  TITLE+=QML StateMachine
  DEPENDS:=+qt5quick-qml
endef

define Package/qt5quick-qml-module-qtqml-workerscript2
  $(call Package/qt5quick/Default)
  TITLE+=QML WorkerScript
  DEPENDS:=+qt5quick-qml-worker-script
endef

###

define Package/qt5quick-qml-module-qtlabs-animation
  $(call Package/qt5quick/Default)
  TITLE+=QML QtLabs animation
  DEPENDS:=+qt5quick-qml +qt5quick-quick
endef

define Package/qt5quick-qml-module-qtlabs-folderlistmodel
  $(call Package/qt5quick/Default)
  TITLE+=QML QtLabs folderlistmodel
  DEPENDS:=+qt5quick-qml
endef

define Package/qt5quick-qml-module-qtlabs-models
  $(call Package/qt5quick/Default)
  TITLE+=QML QtLabs qmlmodels
  DEPENDS+=+qt5quick-qml-models
endef

define Package/qt5quick-qml-module-qtlabs-settings
  $(call Package/qt5quick/Default)
  TITLE+=QML QtLabs settings
  DEPENDS:=+qt5quick-qml
endef

define Package/qt5quick-qml-module-qtlabs-sharedimage
  $(call Package/qt5quick/Default)
  TITLE+=QML QtLabs sharedimage
  DEPENDS:=+qt5quick-quick
endef

define Package/qt5quick-qml-module-qtlabs-wavefrontmesh
  $(call Package/qt5quick/Default)
  TITLE+=QML QtLabs wavefrontmesh
  DEPENDS:=+qt5quick-quick
endef


define Package/qt5quick-qml-module-qttest
  $(call Package/qt5quick/Default)
  TITLE+=QML QtTest
  DEPENDS+=+qt5quick-quick +qt5quick-quick-test
endef

define Package/qt5quick-qml-module-qtquick-layouts
  $(call Package/qt5quick/Default)
  TITLE+=QML QtQuick Layouts
  DEPENDS+=+qt5quick-quick
endef

#define Package/qt5quick-qml-module-qtquick-particles2
#  $(call Package/qt5quick/Default)
#  TITLE+=QML particles
#  DEPENDS+=+qt5quick-particles
#endef

define Package/qt5quick-qml-module-qtquick-localstorage
  $(call Package/qt5quick/Default)
  TITLE+=QML QtQuick LocalStorage
  DEPENDS+=+qt5quick-qml +qt5base-sql
endef

define Package/qt5quick-qml-module-qtquick-shapes
  $(call Package/qt5quick/Default)
  TITLE+=QML QtQuick Shapes
  DEPENDS+=+qt5quick-quick-shapes
endef

define Package/qt5quick-qml-module-qtquick-window2
  $(call Package/qt5quick/Default)
  TITLE+=QML QtQuick Window.2
  DEPENDS+=+qt5quick-quick
endef

define Package/qt5quick-qml-module-qtquick
  $(call Package/qt5quick/Default)
  TITLE+=QML QtQuick.2
  DEPENDS+=+qt5quick-quick +qt5quick-qml-worker-script
endef

####

define Build/InstallDev
	$(call Build/Install/HostFiles,$(1))
	$(call Build/Install/Headers,$(1))
	$(call Build/Install/Cmakes,$(1))
	$(call Build/Install/Pkgconfigs,$(1))
	$(call Build/Install/Libs,$(1),*)
	$(call Build/Install/Plugins,$(1),*,*)

	ln -sf /bin/true $(STAGING_DIR)/host/bin/qmlplugindump
endef

define Build/Install/QMLplugin
	$(INSTALL_DIR) \
	  $(1)/$(QT_INSTALL_QML)/$(2)

	$(CP) \
	  $(PKG_INSTALL_DIR)/$(QT_INSTALL_QML)/$(2)/{plugins.qmltypes,qmldir,$(3).{so,qml,js}*} \
	  $(1)/$(QT_INSTALL_QML)/$(2)/ \
	  || true
endef

###

define Package/qt5quick-qml/install
	$(call Build/Install/Libs,$(1),libQt5Qml)

	$(INSTALL_DIR) \
	  $(1)/$(QT_INSTALL_QML)

	$(CP) \
	  $(PKG_INSTALL_DIR)/$(QT_INSTALL_QML)/builtins.qmltypes \
	  $(1)/$(QT_INSTALL_QML)/
endef

define Package/qt5quick-qml-models/install
	$(call Build/Install/Libs,$(1),libQt5QmlModels)
endef

define Package/qt5quick-qml-worker-script/install
	$(call Build/Install/Libs,$(1),libQt5QmlWorkerScript)
endef

define Package/qt5quick-quick/install
	$(call Build/Install/Libs,$(1),libQt5Quick)
endef

define Package/qt5quick-quick-test/install
	$(call Build/Install/Libs,$(1),libQt5QuickTest)
endef

#define Package/qt5quick-particles/install
#	$(call Build/Install/Libs,$(1),libQt5QuickParticles)
#endef

define Package/qt5quick-quick-widgets/install
	$(call Build/Install/Libs,$(1),libQt5QuickWidgets)
endef

define Package/qt5quick-quick-shapes/install
	$(call Build/Install/Libs,$(1),libQt5QuickShapes)
endef

define Package/qt5quick-qml-module-qtqml-models2/install
	$(call Build/Install/QMLplugin,$(1),QtQml/Models.2,*)
endef

define Package/qt5quick-qml-module-qtqml-statemachine/install
	$(call Build/Install/QMLplugin,$(1),QtQml/StateMachine,*)
endef

define Package/qt5quick-qml-module-qtqml-workerscript2/install
	$(call Build/Install/QMLplugin,$(1),QtQml/WorkerScript.2,*)
endef

define Package/qt5quick-qml-module-qtlabs-animation/install
	$(call Build/Install/QMLplugin,$(1),Qt/labs/animation/,*)
endef

define Package/qt5quick-qml-module-qtlabs-folderlistmodel/install
	$(call Build/Install/QMLplugin,$(1),Qt/labs/folderlistmodel,*)
endef

define Package/qt5quick-qml-module-qtlabs-models/install
	$(call Build/Install/QMLplugin,$(1),Qt/labs/qmlmodels,*)
endef

define Package/qt5quick-qml-module-qtlabs-settings/install
	$(call Build/Install/QMLplugin,$(1),Qt/labs/settings/,*)
endef

define Package/qt5quick-qml-module-qtlabs-sharedimage/install
	$(call Build/Install/QMLplugin,$(1),Qt/labs/sharedimage/,*)
endef

define Package/qt5quick-qml-module-qtlabs-wavefrontmesh/install
	$(call Build/Install/QMLplugin,$(1),Qt/labs/wavefrontmesh/,*)
endef

define Package/qt5quick-qml-module-qttest/install
	$(call Build/Install/QMLplugin,$(1),QtTest,*)
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/qmltestrunner $(1)/usr/bin/
endef

define Package/qt5quick-qml-module-qtquick-layouts/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Layouts,*)
endef

#define Package/qt5quick-qml-module-qtquick-particles2/install
#	$(call Build/Install/QMLplugin,$(1),QtQuick/Particles.2,*)
#endef

define Package/qt5quick-qml-module-qtquick-localstorage/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/LocalStorage,*)
endef

define Package/qt5quick-qml-module-qtquick-shapes/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Shapes,*)
endef

define Package/qt5quick-qml-module-qtquick-window2/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Window.2,*)
endef

define Package/qt5quick-qml-module-qtquick/install
	$(call Build/Install/QMLplugin,$(1),QtQuick.2,*)
endef


#$(eval $(call BuildPackage,qt5quick))
$(eval $(call BuildPackage,qt5quick-qml))
$(eval $(call BuildPackage,qt5quick-qml-models))
$(eval $(call BuildPackage,qt5quick-qml-worker-script))
$(eval $(call BuildPackage,qt5quick-quick))
$(eval $(call BuildPackage,qt5quick-quick-test))
#$(eval $(call BuildPackage,qt5quick-particles))
$(eval $(call BuildPackage,qt5quick-quick-widgets))
$(eval $(call BuildPackage,qt5quick-quick-shapes))
$(eval $(call BuildPackage,qt5quick-qml-module-qtqml-models2))
$(eval $(call BuildPackage,qt5quick-qml-module-qtqml-statemachine))
$(eval $(call BuildPackage,qt5quick-qml-module-qtqml-workerscript2))
$(eval $(call BuildPackage,qt5quick-qml-module-qtlabs-models))
$(eval $(call BuildPackage,qt5quick-qml-module-qtlabs-folderlistmodel))
$(eval $(call BuildPackage,qt5quick-qml-module-qtlabs-settings))
$(eval $(call BuildPackage,qt5quick-qml-module-qtlabs-animation))
$(eval $(call BuildPackage,qt5quick-qml-module-qtlabs-sharedimage))
$(eval $(call BuildPackage,qt5quick-qml-module-qtlabs-wavefrontmesh))
$(eval $(call BuildPackage,qt5quick-qml-module-qtquick))
$(eval $(call BuildPackage,qt5quick-qml-module-qtquick-layouts))
#$(eval $(call BuildPackage,qt5quick-qml-module-qtquick-particles2))
$(eval $(call BuildPackage,qt5quick-qml-module-qtquick-localstorage))
$(eval $(call BuildPackage,qt5quick-qml-module-qtquick-shapes))
$(eval $(call BuildPackage,qt5quick-qml-module-qtquick-window2))
$(eval $(call BuildPackage,qt5quick-qml-module-qttest))
