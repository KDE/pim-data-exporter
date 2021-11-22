cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "5.18.90")
project(pim-data-exporter VERSION ${PIM_VERSION})
set(RELEASE_SERVICE_VERSION "21.11.90")
set(KF5_MIN_VERSION "5.87.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
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
include(KDEGitCommitHooks)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

# Do NOT add quote
set(KDEPIM_DEV_VERSION rc)

# add an extra space
if(DEFINED KDEPIM_DEV_VERSION)
    set(KDEPIM_DEV_VERSION " ${KDEPIM_DEV_VERSION}")
endif()

set(KDEPIM_VERSION "${PIM_VERSION}${KDEPIM_DEV_VERSION} (${RELEASE_SERVICE_VERSION})")

set(KDEPIM_LIB_VERSION "${PIM_VERSION}")
set(KDEPIM_LIB_SOVERSION "5")

set(QT_REQUIRED_VERSION "5.15.2")
if(BUILD_TESTING)
    find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets Test)
endif()

set(AKONADI_VERSION "5.18.90")
set(IDENTITYMANAGEMENT_LIB_VERSION "5.18.90")
set(KMAILTRANSPORT_LIB_VERSION "5.18.90")
set(KMIME_LIB_VERSION "5.18.90")
set(KPIMTEXTEDIT_LIB_VERSION "5.18.90")

set(MAILCOMMON_LIB_VERSION "5.18.90")
set(PIMCOMMON_LIB_VERSION "5.18.90")
set(AKONADINOTES_LIB_VERSION "5.18.90")

# Find KF5 package
find_package(KF5Archive ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5Config ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5ConfigWidgets ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5CoreAddons ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5Crash ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5DBusAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5ItemViews ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5KIO ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5Notifications ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5WidgetsAddons ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5XmlGui ${KF5_MIN_VERSION} REQUIRED)
find_package(KF5WidgetsAddons ${KF5_MIN_VERSION} REQUIRED)

find_package(KF5DocTools ${KF5_MIN_VERSION})
set_package_properties(KF5DocTools PROPERTIES DESCRIPTION
    "Tools to generate documentation"
    TYPE OPTIONAL
)


# Find KdepimLibs Package
find_package(KF5Akonadi ${AKONADI_VERSION} CONFIG REQUIRED)
find_package(KF5Contacts ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5IdentityManagement ${IDENTITYMANAGEMENT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5MailCommon ${MAILCOMMON_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5MailTransport ${KMAILTRANSPORT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Mime ${KMIME_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5PimCommonAkonadi ${PIMCOMMON_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5PimTextEdit ${KPIMTEXTEDIT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5CalendarCore ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5AkonadiNotes ${AKONADINOTES_LIB_VERSION} CONFIG REQUIRED)

find_package(KUserFeedback 1.0.0 CONFIG)
set_package_properties(KUserFeedback PROPERTIES DESCRIPTION "User Feedback lib" TYPE OPTIONAL PURPOSE "Allow to send Telemetry Information (optional). It can be disable in apps.")


include_directories(${pim-data-exporter_SOURCE_DIR} ${pim-data-exporter_BINARY_DIR})

add_definitions(-DTRANSLATION_DOMAIN=\"pimdataexporter\")

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f02)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x055800)

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

if(KF5DocTools_FOUND)
    kdoctools_install(po)
    add_subdirectory(doc)
endif()

ki18n_install(po)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
