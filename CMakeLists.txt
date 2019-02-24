cmake_minimum_required(VERSION 3.1)
set(KDEPIM_VERSION_NUMBER "5.10.3")
project(pim-data-exporter VERSION ${KDEPIM_VERSION_NUMBER})

set(KF5_VERSION "5.51.0")

find_package(ECM ${KF5_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(ECMInstallIcons)


include(ECMSetupVersion)
include(ECMAddTests)

include(GenerateExportHeader)
include(ECMGenerateHeaders)

include(FeatureSummary)
include(CheckFunctionExists)
include(ECMGeneratePriFile)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(ECMAddAppIcon)
include(ECMQtDeclareLoggingCategory)

# Do NOT add quote
set(KDEPIM_DEV_VERSION)

# add an extra space
if(DEFINED KDEPIM_DEV_VERSION)
    set(KDEPIM_DEV_VERSION " ${KDEPIM_DEV_VERSION}")
endif()

set(KDEPIM_VERSION "${KDEPIM_VERSION_NUMBER}${KDEPIM_DEV_VERSION}")

set(KDEPIM_LIB_VERSION "${KDEPIM_VERSION_NUMBER}")
set(KDEPIM_LIB_SOVERSION "5")

set(QT_REQUIRED_VERSION "5.9.0")
if(BUILD_TESTING)
    find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets Test)
endif()

set(AKONADI_VERSION "5.10.3")
set(IDENTITYMANAGEMENT_LIB_VERSION "5.10.3")
set(KCONTACTS_LIB_VERSION "5.10.3")
set(KMAILTRANSPORT_LIB_VERSION "5.10.3")
set(KMIME_LIB_VERSION "5.10.3")
set(KPIMTEXTEDIT_LIB_VERSION "5.10.3")
set(LIBKDEPIM_LIB_VERSION_LIB "5.10.3")
set(MAILCOMMON_LIB_VERSION_LIB "5.10.3")
set(PIMCOMMON_LIB_VERSION_LIB "5.10.3")

# Find KF5 package
find_package(KF5Archive ${KF5_VERSION} REQUIRED)
find_package(KF5Config ${KF5_VERSION} REQUIRED)
find_package(KF5ConfigWidgets ${KF5_VERSION} REQUIRED)
find_package(KF5CoreAddons ${KF5_VERSION} REQUIRED)
find_package(KF5Crash ${KF5_VERSION} REQUIRED)
find_package(KF5DBusAddons ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5DocTools ${KF5_VERSION} REQUIRED)
find_package(KF5ItemViews ${KF5_VERSION} REQUIRED)
find_package(KF5KIO ${KF5_VERSION} REQUIRED)
find_package(KF5Notifications ${KF5_VERSION} REQUIRED)
find_package(KF5WidgetsAddons ${KF5_VERSION} REQUIRED)
find_package(KF5XmlGui ${KF5_VERSION} REQUIRED)

# Find KdepimLibs Package
find_package(KF5Akonadi ${AKONADI_VERSION} CONFIG REQUIRED)
find_package(KF5Contacts ${KCONTACTS_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5IdentityManagement ${IDENTITYMANAGEMENT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Libkdepim ${LIBKDEPIM_LIB_VERSION_LIB} CONFIG REQUIRED)
find_package(KF5MailCommon ${MAILCOMMON_LIB_VERSION_LIB} CONFIG REQUIRED)
find_package(KF5MailTransport ${KMAILTRANSPORT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Mime ${KMIME_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5PimCommonAkonadi ${PIMCOMMON_LIB_VERSION_LIB} CONFIG REQUIRED)
find_package(KF5PimTextEdit ${KPIMTEXTEDIT_LIB_VERSION} CONFIG REQUIRED)

include_directories(${pim-data-exporter_SOURCE_DIR} ${pim-data-exporter_BINARY_DIR})

add_definitions(-DTRANSLATION_DOMAIN=\"pimsettingexporter\")

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

configure_file(pimsettingexporter-version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/pimsettingexporter-version.h @ONLY)

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif(BUILD_TESTING)

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(console)

install( FILES pimsettingexporter.renamecategories pimsettingexporter.categories DESTINATION ${KDE_INSTALL_CONFDIR} )
install(FILES org.kde.pimsettingexporter.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})


add_subdirectory(doc)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
