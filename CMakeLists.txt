# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "5.23.40")
project(pim-data-exporter VERSION ${PIM_VERSION})
set(RELEASE_SERVICE_VERSION "23.07.40")
set(KF_MIN_VERSION "5.104.0")

find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(ECMInstallIcons)


include(ECMSetupVersion)
include(ECMAddTests)

include(GenerateExportHeader)
include(ECMGenerateHeaders)

include(FeatureSummary)
include(CheckFunctionExists)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMAddAppIcon)
include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(KDEGitCommitHooks)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

# Do NOT add quote
set(KDEPIM_DEV_VERSION alpha)

# add an extra space
if(DEFINED KDEPIM_DEV_VERSION)
    set(KDEPIM_DEV_VERSION " ${KDEPIM_DEV_VERSION}")
endif()

set(KDEPIM_VERSION "${PIM_VERSION}${KDEPIM_DEV_VERSION} (${RELEASE_SERVICE_VERSION})")

set(KDEPIM_LIB_VERSION "${PIM_VERSION}")
set(KDEPIM_LIB_SOVERSION "5")

set(QT_REQUIRED_VERSION "5.15.2")
if (QT_MAJOR_VERSION STREQUAL "6")
    set(QT_REQUIRED_VERSION "6.4.0")
    set(KF_MIN_VERSION "5.240.0")
    set(KF_MAJOR_VERSION "6")
else()
    set(KF_MAJOR_VERSION "5")
endif()
if(BUILD_TESTING)
    find_package(Qt${QT_MAJOR_VERSION} ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets Test)
endif()

set(AKONADI_VERSION "5.23.40")
set(IDENTITYMANAGEMENT_LIB_VERSION "5.23.40")
set(KMAILTRANSPORT_LIB_VERSION "5.23.40")
set(KMIME_LIB_VERSION "5.23.40")
set(KPIMTEXTEDIT_LIB_VERSION "5.23.40")

set(MAILCOMMON_LIB_VERSION "5.23.40")
set(PIMCOMMON_LIB_VERSION "5.23.40")
set(AKONADINOTES_LIB_VERSION "5.23.40")

# Find KF5 package
find_package(KF${KF_MAJOR_VERSION}Archive ${KF_MIN_VERSION} REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Config ${KF_MIN_VERSION} REQUIRED)
find_package(KF${KF_MAJOR_VERSION}CoreAddons ${KF_MIN_VERSION} REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Crash ${KF_MIN_VERSION} REQUIRED)
find_package(KF${KF_MAJOR_VERSION}DBusAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}ItemViews ${KF_MIN_VERSION} REQUIRED)
find_package(KF${KF_MAJOR_VERSION}KIO ${KF_MIN_VERSION} REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Notifications ${KF_MIN_VERSION} REQUIRED)
find_package(KF${KF_MAJOR_VERSION}WidgetsAddons ${KF_MIN_VERSION} REQUIRED)
find_package(KF${KF_MAJOR_VERSION}XmlGui ${KF_MIN_VERSION} REQUIRED)
find_package(KF${KF_MAJOR_VERSION}WidgetsAddons ${KF_MIN_VERSION} REQUIRED)

find_package(KF${KF_MAJOR_VERSION}DocTools ${KF_MIN_VERSION})
set_package_properties(KF${KF_MAJOR_VERSION}DocTools PROPERTIES DESCRIPTION
    "Tools to generate documentation"
    TYPE OPTIONAL
)


# Find KdepimLibs Package
find_package(KPim${KF_MAJOR_VERSION}Akonadi ${AKONADI_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}Contacts ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}IdentityManagement ${IDENTITYMANAGEMENT_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}MailCommon ${MAILCOMMON_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}MailTransport ${KMAILTRANSPORT_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}Mime ${KMIME_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}PimCommonAkonadi ${PIMCOMMON_LIB_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}TextEdit ${KPIMTEXTEDIT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF${KF_MAJOR_VERSION}CalendarCore ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KPim${KF_MAJOR_VERSION}AkonadiNotes ${AKONADINOTES_LIB_VERSION} CONFIG REQUIRED)

set(KUSERFEEDBACK_SUFFIX)
if (QT_MAJOR_VERSION STREQUAL "6")
    set(KUSERFEEDBACK_SUFFIX "Qt6")
    find_package(KUserFeedback${KUSERFEEDBACK_SUFFIX} 1.2.1 CONFIG)
    add_definitions(-DUSE_KUSERFEEDBACK_QT6)
else()
    find_package(KUserFeedback${KUSERFEEDBACK_SUFFIX} 1.2.0 CONFIG)
endif()
set_package_properties(KUserFeedback${KUSERFEEDBACK_SUFFIX} PROPERTIES DESCRIPTION "User Feedback lib" TYPE OPTIONAL PURPOSE "Allow to send Telemetry Information (optional). It can be disable in apps.")


include_directories(${pim-data-exporter_SOURCE_DIR} ${pim-data-exporter_BINARY_DIR})

add_definitions(-DTRANSLATION_DOMAIN=\"pimdataexporter\")

ecm_set_disabled_deprecation_versions(QT 6.4  KF 5.104.0)


option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if (USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()

configure_file(pimdataexporter-version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/pimdataexporter-version.h @ONLY)

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif(BUILD_TESTING)

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(console)

ecm_qt_install_logging_categories(
        EXPORT PIMDATAEXPORTER
        FILE pimdataexporter.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
        )


install(FILES org.kde.pimdataexporter.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

if(KF${KF_MAJOR_VERSION}DocTools_FOUND)
    kdoctools_install(po)
    add_subdirectory(doc)
endif()
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
