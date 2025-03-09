# SPDX-FileCopyrightText: 2011-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "6.3.80")
project(pim-data-exporter VERSION ${PIM_VERSION})
set(RELEASE_SERVICE_VERSION "25.03.80")
set(KF_MIN_VERSION "6.9.0")
# Update it for each release
set(PIMDATAEXPORTER_RELEASE_VERSION_DATE "25.04.10") # yy.MM.dd

find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(ECMInstallIcons)


include(ECMSetupVersion)
include(ECMAddTests)

include(GenerateExportHeader)
include(ECMGenerateHeaders)

include(CheckFunctionExists)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMAddAppIcon)
include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(ECMFeatureSummary)
include(KDEGitCommitHooks)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES
gui/*.cpp
gui/*.h
core/*.cpp
core/*.h
console/*.cpp
console/*.h
)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
include(ECMCheckOutboundLicense)
file(GLOB_RECURSE ALL_SOURCE_FILES
gui/*.cpp
gui/*.h
core/*.cpp
core/*.h
console/*.cpp
console/*.h
)
ecm_check_outbound_license(LICENSES GPL-2.0-only  FILES ${ALL_SOURCE_FILES})

# Do NOT add quote
set(KDEPIM_DEV_VERSION beta)

# add an extra space
if(DEFINED KDEPIM_DEV_VERSION)
    set(KDEPIM_DEV_VERSION " ${KDEPIM_DEV_VERSION}")
endif()

set(KDEPIM_VERSION "${PIM_VERSION}${KDEPIM_DEV_VERSION} (${RELEASE_SERVICE_VERSION})")

set(KDEPIM_LIB_VERSION "${PIM_VERSION}")
set(KDEPIM_LIB_SOVERSION "6")

set(QT_REQUIRED_VERSION "6.7.0")
if(BUILD_TESTING)
    find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets Test)
endif()

set(AKONADI_VERSION "6.3.80")
set(IDENTITYMANAGEMENT_LIB_VERSION "6.3.80")
set(KMAILTRANSPORT_LIB_VERSION "6.3.80")
set(KMIME_LIB_VERSION "6.3.80")

set(MAILCOMMON_LIB_VERSION "6.3.80")
set(PIMCOMMON_LIB_VERSION "6.3.80")
set(KTEXTADDONS_MIN_VERSION "1.5.4")

# Find KF6 package
find_package(KF6Archive ${KF_MIN_VERSION} REQUIRED)
find_package(KF6Config ${KF_MIN_VERSION} REQUIRED)
find_package(KF6CoreAddons ${KF_MIN_VERSION} REQUIRED)
find_package(KF6Crash ${KF_MIN_VERSION} REQUIRED)
find_package(KF6ItemViews ${KF_MIN_VERSION} REQUIRED)
find_package(KF6KIO ${KF_MIN_VERSION} REQUIRED)
find_package(KF6Notifications ${KF_MIN_VERSION} REQUIRED)
find_package(KF6WidgetsAddons ${KF_MIN_VERSION} REQUIRED)
find_package(KF6XmlGui ${KF_MIN_VERSION} REQUIRED)
find_package(KF6StatusNotifierItem ${KF_MIN_VERSION} REQUIRED)
find_package(KF6IconThemes ${KF_MIN_VERSION} QUIET)

find_package(KF6DocTools ${KF_MIN_VERSION})
set_package_properties(KF6DocTools PROPERTIES DESCRIPTION
    "Tools to generate documentation"
    TYPE OPTIONAL
)

if(NOT WIN32 AND NOT APPLE)
    find_package(KF6DBusAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
endif()

# Find KdepimLibs Package
find_package(KPim6Akonadi ${AKONADI_VERSION} CONFIG REQUIRED)
find_package(KF6Contacts ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KPim6IdentityManagementCore ${IDENTITYMANAGEMENT_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6MailCommon ${MAILCOMMON_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6MailTransport ${KMAILTRANSPORT_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6Mime ${KMIME_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim6PimCommonAkonadi ${PIMCOMMON_LIB_VERSION} CONFIG REQUIRED)
find_package(KF6CalendarCore ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6TextCustomEditor ${KTEXTADDONS_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6UserFeedback ${KF_MIN_VERSION} CONFIG)
set_package_properties(KF6UserFeedback PROPERTIES DESCRIPTION "User Feedback lib" TYPE OPTIONAL PURPOSE "Allow to send Telemetry Information (optional). It can be disable in apps.")

if(TARGET KF6::UserFeedbackWidgets)
	set(PIMDATAEXPORTER_WITH_KUSERFEEDBACK true)
endif()

include_directories(${pim-data-exporter_SOURCE_DIR} ${pim-data-exporter_BINARY_DIR})

add_definitions(-DTRANSLATION_DOMAIN=\"pimdataexporter\")

add_definitions(-DQT_NO_CONTEXTLESS_CONNECT)
ecm_set_disabled_deprecation_versions(QT 6.9.0 KF 6.12.0)


option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if(USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()

configure_file(pimdataexporter-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/pimdataexporter-version.h @ONLY)

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif()
configure_file(config-pimdataexporter.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-pimdataexporter.h)

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(console)

ecm_qt_install_logging_categories(
        EXPORT PIMDATAEXPORTER
        FILE pimdataexporter.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)


install(FILES org.kde.pimdataexporter.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

if(KF6DocTools_FOUND)
    kdoctools_install(po)
    add_subdirectory(doc)
endif()
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)
ecm_feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
