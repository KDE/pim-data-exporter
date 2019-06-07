
include_directories( 
    ${pim-data-exporter_SOURCE_DIR}/core/
    )


set( pimsettingexporter_kcfg_SRCS settings/pimdataexporterglobalconfig.kcfgc )

kconfig_add_kcfg_files(pimdataexporter_kcfg_SRCS ${pimsettingexporter_kcfg_SRCS})


set(pimsettingexporter_widget_SRCS
    widgets/selectiontypetreewidget.cpp
    widgets/logwidget.cpp
    widgets/pimdataexporterconfigurewidget.cpp
    )

set(pimsettingexporter_dialog_SRCS
    dialog/selectiontypedialog.cpp
    dialog/backupfilestructureinfodialog.cpp
    dialog/showarchivestructuredialog.cpp
    dialog/synchronizeresourcedialog.cpp
    dialog/pimdataexporterconfiguredialog.cpp
    )

set(pimsettingexporter_job_SRCS
    job/fullsynchronizeresourcesjob.cpp
    )

set(pimsettingexporter_trayicon_SRCS
    trayicon/pimdatatrayicon.cpp
    )

set(pimsettingexporter_SRCS
    ${pimdataexporter_kcfg_SRCS}
    ${pimsettingexporter_widget_SRCS}
    ${pimsettingexporter_dialog_SRCS}
    ${pimsettingexporter_job_SRCS}
    ${pimsettingexporter_trayicon_SRCS}
    pimdatabackuprestoreui.cpp
    importexportprogressindicatorgui.cpp
    pimdataexporterwindow.cpp
    pimdatacommandlineoption.cpp
    main.cpp)

qt5_add_resources(pimsettingexporter_SRCS pimsettingexporter.qrc)

ecm_qt_declare_logging_category(pimsettingexporter_SRCS HEADER pimsettingexportgui_debug.h IDENTIFIER PIMSETTINGEXPORTERGUI_LOG CATEGORY_NAME org.kde.pim.pimsettingexportergui)

add_executable(pimsettingexporter ${pimsettingexporter_SRCS})

target_link_libraries(pimsettingexporter
    PRIVATE
    KF5::XmlGui
    KF5::DBusAddons
    KF5::MailCommon
    KF5::PimCommon
    KF5::Libkdepim
    pimdataexporterprivate
    KF5::Archive
    KF5::ItemViews
    KF5::KIOFileWidgets
    KF5::Notifications
    KF5::I18n
    KF5::Crash
    )

add_subdirectory(kconf_update)

install(TARGETS pimsettingexporter ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

install(PROGRAMS org.kde.pimsettingexporter.desktop DESTINATION ${KDE_INSTALL_APPDIR})

install(FILES settings/pimdataexporterglobalconfig.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})

if(BUILD_TESTING)
    add_subdirectory(tests)
    add_subdirectory(autotests)
endif()

