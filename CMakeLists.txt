project(pim-data-exporter)

cmake_minimum_required(VERSION 3.1)
set(KF5_VERSION "5.32.0")
find_package(ECM ${KF5_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(ECMInstallIcons)

include(ECMOptionalAddSubdirectory)
include(ECMSetupVersion)
include(ECMAddTests)
include(ECMMarkNonGuiExecutable)
include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(CMakePackageConfigHelpers)
include(FeatureSummary)
include(CheckFunctionExists)
include(ECMGeneratePriFile)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMAddAppIcon)
include(ECMQtDeclareLoggingCategory)
include(ECMCoverageOption)
# Do NOT add quote
set(KDEPIM_DEV_VERSION alpha1)

# add an extra space
if(DEFINED KDEPIM_DEV_VERSION)
    set(KDEPIM_DEV_VERSION " ${KDEPIM_DEV_VERSION}")
endif()


set(KDEPIM_VERSION_NUMBER "5.4.40")
set(KDEPIM_VERSION "${KDEPIM_VERSION_NUMBER}${KDEPIM_DEV_VERSION}")


set(AKONADI_MIMELIB_VERSION "5.4.40")
set(AKONADI_CONTACT_VERSION "5.4.40")
set(KMAILTRANSPORT_LIB_VERSION "5.4.46")
set(CALENDARSUPPORT_LIB_VERSION_LIB "5.4.40")
set(KPIMTEXTEDIT_LIB_VERSION "5.4.40")
set(AKONADI_VERSION "5.4.40")


set(KDEPIM_LIB_VERSION "${KDEPIM_VERSION_NUMBER}")
set(KDEPIM_LIB_SOVERSION "5")


set(QT_REQUIRED_VERSION "5.6.0")
find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets Xml Test)
set(PIMCOMMON_LIB_VERSION "5.4.48")
set(MAILCOMMON_LIB_VERSION_LIB "5.4.40")
set(LIBKDEPIM_LIB_VERSION_LIB "5.4.40")
set(AKONADIKALARM_LIB_VERSION "5.4.40")
set(KCALENDARCORE_LIB_VERSION "5.4.40")
set(KCONTACTS_LIB_VERSION "5.4.40")

# Find KF5 package
find_package(KF5DBusAddons ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5DocTools ${KF5_VERSION} REQUIRED)
find_package(KF5Crash ${KF5_VERSION} REQUIRED)

# Find KdepimLibs Package
find_package(KF5PimTextEdit ${KPIMTEXTEDIT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Akonadi ${AKONADI_VERSION} CONFIG REQUIRED)
find_package(KF5Contacts ${KCONTACTS_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5CalendarCore ${KCALENDARCORE_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5AkonadiContact ${AKONADI_CONTACT_VERSION} CONFIG REQUIRED)
find_package(KF5CalendarSupport ${CALENDARSUPPORT_LIB_VERSION_LIB} CONFIG REQUIRED)
find_package(KF5AkonadiMime ${AKONADI_MIMELIB_VERSION} CONFIG REQUIRED)
find_package(KF5AlarmCalendar ${AKONADIKALARM_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5PimCommonAkonadi ${PIMCOMMON_LIB_VERSION_LIB} CONFIG REQUIRED)

find_package(KF5MailCommon ${MAILCOMMON_LIB_VERSION_LIB} CONFIG REQUIRED)
find_package(KF5Libkdepim ${LIBKDEPIM_LIB_VERSION_LIB} CONFIG REQUIRED)
find_package(KF5MailTransport ${KMAILTRANSPORT_LIB_VERSION} CONFIG REQUIRED)
find_package(Boost 1.34.0)
set_package_properties(Boost PROPERTIES DESCRIPTION "Boost C++ Libraries" URL "http://www.boost.org" TYPE REQUIRED PURPOSE "Boost is required for building most KDEPIM applications")

include_directories(${pim-data-exporter_SOURCE_DIR} ${pim-data-exporter_BINARY_DIR})

add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
add_definitions(-DQT_USE_QSTRINGBUILDER)

add_definitions(-DTRANSLATION_DOMAIN=\"pimsettingexporter\")

add_definitions( -DQT_NO_CAST_FROM_ASCII )
add_definitions( -DQT_NO_CAST_TO_ASCII )

configure_file(pimsettingexporter-version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/pimsettingexporter-version.h @ONLY)

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(console)

install( FILES pimsettingexporter.renamecategories pimsettingexporter.categories DESTINATION ${KDE_INSTALL_CONFDIR} )


add_subdirectory(doc)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
