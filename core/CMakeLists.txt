add_library(pimdataexporterprivate)

target_sources(pimdataexporterprivate PRIVATE
    calendar/exportcalendarjobinterfaceimpl.cpp
    calendar/importcalendarjobinterfaceimpl.cpp

    addressbook/exportaddressbookjobinterfaceimpl.cpp
    addressbook/importaddressbookjobinterfaceimpl.cpp

    mail/exportmailjobinterfaceimpl.cpp
    mail/importmailjobinterfaceimpl.cpp
    mail/importexportmailutil.cpp
    mail/smtpmailtransport.cpp

    alarm/exportalarmjobinterfaceimpl.cpp
    alarm/importalarmjobinterfaceimpl.cpp

    notes/exportnotesjobinterface.cpp
    notes/importnotesjobinterface.cpp
    calendar/exportcalendarjobinterface.cpp
    calendar/importcalendarjobinterface.cpp
    addressbook/exportaddressbookjobinterface.cpp
    addressbook/importaddressbookjobinterface.cpp
    alarm/importalarmjobinterface.cpp
    alarm/exportalarmjobinterface.cpp
    mail/exportmailjobinterface.cpp
    mail/importmailjobinterface.cpp

    notes/exportnotesjobinterfaceimpl.cpp
    notes/importnotesjobinterfaceimpl.cpp
    xml/templateselection.cpp
    akregator/exportakregatorjobinterface.cpp
    akregator/importakregatorjobinterface.cpp

    abstractimportexportjob.cpp
    pimdataexporterkernel.cpp
    utils.cpp
    archivestorage.cpp
    synchronizeresourcejob.cpp
    pimdatabackuprestore.cpp
    importexportprogressindicatorbase.cpp
    pimdatabackupthread.cpp
    exportresourcearchivejob.cpp
    pimdataimportdatainfofile.cpp
    resourceconverterbase.cpp
    resourceconverterimpl.cpp
    storeresourcejob.cpp
    backupresourcefilejobbase.cpp
    backupresourcefilejobimpl.cpp
    )

ecm_qt_declare_logging_category(pimdataexporterprivate HEADER pimdataexportcore_debug.h IDENTIFIER PIMDATAEXPORTERCORE_LOG CATEGORY_NAME org.kde.pim.pimdataexportercore
        DESCRIPTION "pimdataexporter (pimdataexporter lib core)"
        OLD_CATEGORY_NAMES log_pimsettingexportercore org.kde.pim.pimsettingexportercore
        EXPORT PIMDATAEXPORTER
    )


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(pimdataexporterprivate PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(pimdataexporterprivate BASE_NAME pimdataexporter)



target_link_libraries(pimdataexporterprivate
    PRIVATE
    KF5::AkonadiCore
    KF5::Mime
    KF5::MailTransport
    KF5::MailCommon
    KF5::Archive
    KF5::PimCommonAkonadi
    KF5::I18n
    KF5::CalendarCore
    KF5::AkonadiNotes
    )

set_target_properties(pimdataexporterprivate PROPERTIES VERSION ${KDEPIM_LIB_VERSION} SOVERSION ${KDEPIM_LIB_SOVERSION})

install(TARGETS pimdataexporterprivate ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

