#
# Copyright (C) 2015 OpenWrt
# Author: Mirko Vogt <mirko@openwrt.org>
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=qt5quick2
PKG_VERSION:=5.9.1
PKG_RELEASE:=1
PKG_HASH:=d2fe6c9c1a9f19af6e96553c6d75366ab8f397bc232acd67d31476955fee94ff

PKG_SYS_NAME:=qtdeclarative-opensource-src-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_SYS_NAME).tar.xz
PKG_SOURCE_URL:=http://download.qt-project.org/official_releases/qt/$(basename $(PKG_VERSION))/$(PKG_VERSION)/submodules

PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_SYS_NAME)
PKG_BUILD_PARALLEL:=1
PKG_BUILD_DEPENDS:=qt5base
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk
-include $(STAGING_DIR)/host/mk/qmake.mk

define Package/qt5quick2/Default
  SECTION:=video-frameworks
  CATEGORY:=Video
  SUBMENU:=Frameworks and Toolkits
  TITLE:=Qt5quick2
#  DEPENDS+=qt5quick2
  URL:=http://qt.io
  MAINTAINER:=Mirko Vogt <mirko@openwrt.org>
endef

# define Package/qt5quick2
#   $(call Package/qt5quick2/Default)
#   DEPENDS:=
#   MENU:=1
# endef

define Package/qt5quick2-qml
  $(call Package/qt5quick2/Default)
  TITLE+=qml
  DEPENDS+=+qt5base-network
endef

define Package/qt5quick2-quick
  $(call Package/qt5quick2/Default)
  TITLE+=quick
  DEPENDS+=+qt5quick2-qml +qt5base-gui
endef

define Package/qt5quick2-test
  $(call Package/qt5quick2/Default)
  TITLE+=test
  DEPENDS+=+qt5quick2-quick +qt5base-gui +qt5base-widgets +qt5base-test
endef

define Package/qt5quick2-particles
  $(call Package/qt5quick2/Default)
  TITLE+=particles
  DEPENDS+=+qt5quick2-quick
endef

define Package/qt5quick2-widgets
  $(call Package/qt5quick2/Default)
  TITLE+=widgets
  DEPENDS+=+qt5quick2-quick +qt5base-gui +qt5base-widgets
endef

###

define Package/qt5quick2-qml-statemachine
  $(call Package/qt5quick2/Default)
  TITLE+=QML statemachine
  DEPENDS:=+qt5quick2-qml
endef

define Package/qt5quick2-qml-models
  $(call Package/qt5quick2/Default)
  TITLE+=QML models
  DEPENDS+=+qt5quick2-qml
endef

define Package/qt5quick2-qml-folderlistmodel
  $(call Package/qt5quick2/Default)
  TITLE+=QML folderlistmodel
  DEPENDS:=+qt5quick2-qml
endef

define Package/qt5quick2-qml-settings
  $(call Package/qt5quick2/Default)
  TITLE+=QML settings
  DEPENDS:=+qt5quick2-qml
endef

define Package/qt5quick2-qml-test
  $(call Package/qt5quick2/Default)
  TITLE+=QML folderlistmodel
  DEPENDS+=+qt5quick2-quick +qt5base-gui +qt5quick2-test
endef

define Package/qt5quick2-qml-layouts
  $(call Package/qt5quick2/Default)
  TITLE+=QML layouts
  DEPENDS+=+qt5quick2-quick
endef

define Package/qt5quick2-qml-particles
  $(call Package/qt5quick2/Default)
  TITLE+=QML particles
  DEPENDS+=+qt5quick2-particles
endef

define Package/qt5quick2-qml-window
  $(call Package/qt5quick2/Default)
  TITLE+=QML window
  DEPENDS+=+qt5quick2-quick
endef

define Package/qt5quick2-qml-localstorage
  $(call Package/qt5quick2/Default)
  TITLE+=QML localstorage
  DEPENDS+=+qt5quick2-qml +qt5base-sql
endef

define Package/qt5quick2-qml-quick
  $(call Package/qt5quick2/Default)
  TITLE+=QML quick2
  DEPENDS+=+qt5quick2-quick
endef

####

define Build/InstallDev
	$(call Build/Install/HostFiles,$(1))
	$(call Build/Install/Headers,$(1))
	$(call Build/Install/Libs,$(1),*)
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

define Package/qt5quick2-qml/install
	$(call Build/Install/Libs,$(1),libQt5Qml)
endef

define Package/qt5quick2-quick/install
	$(call Build/Install/Libs,$(1),libQt5Quick)
endef

define Package/qt5quick2-test/install
	$(call Build/Install/Libs,$(1),libQt5QuickTest)
endef

define Package/qt5quick2-particles/install
	$(call Build/Install/Libs,$(1),libQt5QuickParticles)
endef

define Package/qt5quick2-widgets/install
	$(call Build/Install/Libs,$(1),libQt5QuickWidgets)
endef


define Package/qt5quick2-qml-statemachine/install
	$(call Build/Install/QMLplugin,$(1),/QtQml/StateMachine,*)
endef

define Package/qt5quick2-qml-models/install
	$(call Build/Install/QMLplugin,$(1),QtQml/Models.2,*)
endef

define Package/qt5quick2-qml-folderlistmodel/install
	$(call Build/Install/QMLplugin,$(1),Qt/labs/folderlistmodel,*)
endef

define Package/qt5quick2-qml-settings/install
	$(call Build/Install/QMLplugin,$(1),Qt/labs/settings/,*)
endef

define Package/qt5quick2-qml-test/install
	$(call Build/Install/QMLplugin,$(1),QtTest,*)
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/qmltestrunner $(1)/usr/bin/
endef

define Package/qt5quick2-qml-layouts/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Layouts,*)
endef

define Package/qt5quick2-qml-particles/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Particles.2,*)
endef

define Package/qt5quick2-qml-window/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/Window.2,*)
endef

define Package/qt5quick2-qml-localstorage/install
	$(call Build/Install/QMLplugin,$(1),QtQuick/LocalStorage,*)
endef

define Package/qt5quick2-qml-quick/install
	$(call Build/Install/QMLplugin,$(1),QtQuick.2,*)
endef


#$(eval $(call BuildPackage,qt5quick2))
$(eval $(call BuildPackage,qt5quick2-qml))
$(eval $(call BuildPackage,qt5quick2-quick))
$(eval $(call BuildPackage,qt5quick2-test))
$(eval $(call BuildPackage,qt5quick2-particles))
$(eval $(call BuildPackage,qt5quick2-widgets))
$(eval $(call BuildPackage,qt5quick2-qml-statemachine))
$(eval $(call BuildPackage,qt5quick2-qml-models))
$(eval $(call BuildPackage,qt5quick2-qml-folderlistmodel))
$(eval $(call BuildPackage,qt5quick2-qml-settings))
$(eval $(call BuildPackage,qt5quick2-qml-test))
$(eval $(call BuildPackage,qt5quick2-qml-layouts))
$(eval $(call BuildPackage,qt5quick2-qml-particles))
$(eval $(call BuildPackage,qt5quick2-qml-window))
$(eval $(call BuildPackage,qt5quick2-qml-localstorage))
$(eval $(call BuildPackage,qt5quick2-qml-quick))
