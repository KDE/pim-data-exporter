
add_executable(pimdataexporter)

if (TARGET KUserFeedbackWidgets)
    add_definitions(-DWITH_KUSERFEEDBACK)
endif()

set(pimdataexporter_userfeedback_SRCS)
if (TARGET KUserFeedbackWidgets)
    target_sources(pimdataexporter PRIVATE
        userfeedback/userfeedbackmanager.cpp
        userfeedback/pimdataexporteduserfeedbackprovider.cpp
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
    userfeedback/userfeedbackmanager.h
    userfeedback/pimdataexporteduserfeedbackprovider.h
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
    KF5::XmlGui
    KF5::DBusAddons
    KF5::MailCommon
    KF5::PimCommon
    pimdataexporterprivate
    KF5::Archive
    KF5::ItemViews
    KF5::KIOFileWidgets
    KF5::Notifications
    KF5::I18n
    KF5::Crash
    ${pimdataexporter_userfeedback_LIB}
    KF5::WidgetsAddons
    )


install(TARGETS pimdataexporter ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

install(PROGRAMS org.kde.pimdataexporter.desktop DESTINATION ${KDE_INSTALL_APPDIR})

install(FILES settings/pimdataexporterglobalconfig.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})

if(BUILD_TESTING)
    add_subdirectory(tests)
    add_subdirectory(autotests)
endif()

