# SPDX-FileCopyrightText: 2011-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(pimdataexporter)

if (TARGET KUserFeedbackWidgets)
    add_definitions(-DWITH_KUSERFEEDBACK)
endif()

set(pimdataexporter_userfeedback_SRCS)
if (TARGET KUserFeedbackWidgets)
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

if (TARGET KUserFeedbackWidgets)
        target_link_libraries(pimdataexporter PRIVATE KUserFeedbackWidgets)
endif()

target_link_libraries(pimdataexporter
    PRIVATE
    KF${KF_MAJOR_VERSION}::XmlGui
    KF${KF_MAJOR_VERSION}::DBusAddons
    KPim${KF_MAJOR_VERSION}::MailCommon
    KF5::PimCommon
    pimdataexporterprivate
    KF${KF_MAJOR_VERSION}::Archive
    KF${KF_MAJOR_VERSION}::ItemViews
    KF${KF_MAJOR_VERSION}::KIOFileWidgets
    KF${KF_MAJOR_VERSION}::Notifications
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::Crash
    ${pimdataexporter_userfeedback_LIB}
    KF${KF_MAJOR_VERSION}::WidgetsAddons
    )


install(TARGETS pimdataexporter ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

install(PROGRAMS org.kde.pimdataexporter.desktop DESTINATION ${KDE_INSTALL_APPDIR})

install(FILES settings/pimdataexporterglobalconfig.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})

if(BUILD_TESTING)
    add_subdirectory(tests)
    add_subdirectory(autotests)
endif()

