include_directories( 
    ${pim-data-exporter_SOURCE_DIR}/core/
    ${pim-data-exporter_BINARY_DIR}/core/
    )

set(pimdataexporter_calendar_SRCS
    calendar/exportcalendarjobinterface.cpp
    calendar/importcalendarjob.cpp
    )

set(pimdataexporter_addressbook_SRCS
    addressbook/exportaddressbookjob.cpp
    addressbook/importaddressbookjob.cpp
    )

set(pimdataexporter_mail_SRCS
    mail/exportmailjob.cpp
    mail/importmailjob.cpp
    mail/importexportmailutil.cpp
    )

set(pimdataexporter_alarm_SRCS
    alarm/exportalarmjob.cpp
    alarm/importalarmjob.cpp
    )

set(pimdataexporter_interfaces_SRCS
    notes/exportnotesjobinterface.cpp
    notes/importnotesjobinterface.cpp
   )

set(pimdataexporter_notes_SRCS
    notes/exportnotesjobinterfaceimpl.cpp
    notes/importnotesjobinterfaceimpl.cpp
    )

set(pimdataexporter_xml_SRCS
    xml/templateselection.cpp
    )

set(pimdataexporter_akregator_SRCS
    akregator/exportakregatorjobinterface.cpp
    akregator/importakregatorjob.cpp
    )


set(pimdataexporter_LIB_SRCS
    ${pimdataexporter_xml_SRCS}
    ${pimdataexporter_notes_SRCS}
    ${pimdataexporter_alarm_SRCS}
    ${pimdataexporter_mail_SRCS}
    ${pimdataexporter_calendar_SRCS}
    ${pimdataexporter_addressbook_SRCS}
    ${pimdataexporter_jot_SRCS}
    ${pimdataexporter_akregator_SRCS}
    ${pimdataexporter_interfaces_SRCS}
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
    )

ecm_qt_declare_logging_category(pimdataexporter_LIB_SRCS HEADER pimdataexportcore_debug.h IDENTIFIER PIMDATAEXPORTERCORE_LOG CATEGORY_NAME org.kde.pim.pimdataexportercore
        DESCRIPTION "pimdataexporter (pimdataexporter lib core)"
        OLD_CATEGORY_NAMES log_pimsettingexportercore org.kde.pim.pimsettingexportercore
        EXPORT PIMDATAEXPORTER
    )

add_library(pimdataexporterprivate ${pimdataexporter_LIB_SRCS})
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
    add_subdirectory(notes/autotests)
endif()

