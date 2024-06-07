# SPDX-FileCopyrightText: 2011-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(pimdataexporter)

if (TARGET KF6::UserFeedbackWidgets)
    add_definitions(-DWITH_KUSERFEEDBACK)
endif()

if (TARGET KF6::DBusAddons)
    add_definitions(-DHAVE_KDBUSADDONS)
endif()

set(pimdataexporter_userfeedback_SRCS)
if (TARGET KF6::UserFeedbackWidgets)
    target_sources(pimdataexporter PRIVATE
        userfeedback/userfeedbackmanager.cpp
        userfeedback/pimdataexporteduserfeedbackprovider.cpp
        userfeedback/userfeedbackmanager.h
        userfeedback/pimdataexporteduserfeedbackprovider.h

        )
endif()

include_directories( 
    ${pim-data-exporter_SOURCE_DIR}/core/
    )


kconfig_add_kcfg_files(pimdataexporter settings/pimdataexporterglobalconfig.kcfgc)


target_sources(pimdataexporter PRIVATE
    widgets/selectiontypetreewidget.cpp
    widgets/logwidget.cpp
    widgets/pimdataexporterconfigurewidget.cpp

    dialog/selectiontypedialog.cpp
    dialog/backupfilestructureinfodialog.cpp
    dialog/showarchivestructuredialog.cpp
    dialog/synchronizeresourcedialog.cpp
    dialog/pimdataexporterconfiguredialog.cpp

    job/fullsynchronizeresourcesjob.cpp

    trayicon/pimdatatrayicon.cpp

    pimdatabackuprestoreui.cpp
    importexportprogressindicatorgui.cpp
    pimdataexporterwindow.cpp
    pimdatacommandlineoption.cpp
    main.cpp
    pimdataexporter.qrc
    pimdatacommandlineoption.h
    job/fullsynchronizeresourcesjob.h
    trayicon/pimdatatrayicon.h
    dialog/backupfilestructureinfodialog.h
    dialog/pimdataexporterconfiguredialog.h
    dialog/selectiontypedialog.h
    dialog/showarchivestructuredialog.h
    dialog/synchronizeresourcedialog.h
    pimdataexporterwindow.h
    widgets/selectiontypetreewidget.h
    widgets/pimdataexporterconfigurewidget.h
    widgets/logwidget.h
    pimdatabackuprestoreui.h
    importexportprogressindicatorgui.h
    )

#TODO create lib

ecm_qt_declare_logging_category(pimdataexporter HEADER pimdataexportgui_debug.h IDENTIFIER PIMDATAEXPORTERGUI_LOG CATEGORY_NAME org.kde.pim.pimdataexportergui)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(pimdataexporter PROPERTIES UNITY_BUILD ON)
endif()

if (TARGET KF6::UserFeedbackWidgets)
        target_link_libraries(pimdataexporter PRIVATE KF6::UserFeedbackWidgets)
endif()
if(TARGET KF6::IconThemes)
    target_link_libraries(pimdataexporter PRIVATE KF6::IconThemes)
endif()
target_link_libraries(pimdataexporter
    PRIVATE
    KF6::XmlGui
    KPim6::MailCommon
    KPim6::PimCommon
    pimdataexporterprivate
    KF6::Archive
    KF6::ItemViews
    KF6::KIOFileWidgets
    KF6::Notifications
    KF6::I18n
    KF6::Crash
    KF6::StatusNotifierItem
    ${pimdataexporter_userfeedback_LIB}
    KF6::WidgetsAddons
    KF6::TextCustomEditor
    )

if(TARGET KF6::DBusAddons)
    target_link_libraries(pimdataexporter PRIVATE KF6::DBusAddons)
endif()


install(TARGETS pimdataexporter ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

install(PROGRAMS org.kde.pimdataexporter.desktop DESTINATION ${KDE_INSTALL_APPDIR})

install(FILES settings/pimdataexporterglobalconfig.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})

if(BUILD_TESTING)
    add_subdirectory(tests)
    add_subdirectory(autotests)
endif()

