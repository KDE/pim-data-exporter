include_directories( 
    ${pim-data-exporter_SOURCE_DIR}/core/
    ${pim-data-exporter_BINARY_DIR}/core/
    )

set(pimdataexporter_calendar_SRCS
    calendar/exportcalendarjob.cpp
    calendar/importcalendarjob.cpp
    )

set(pimdataexporter_addressbook_SRCS
    addressbook/exportaddressbookjob.cpp
    addressbook/importaddressbookjob.cpp
    )

set(pimdataexporter_mail_SRCS
    mail/exportmailjob.cpp
    mail/importmailjob.cpp
    )

set(pimdataexporter_alarm_SRCS
    alarm/exportalarmjob.cpp
    alarm/importalarmjob.cpp
    )

set(pimdataexporter_notes_SRCS
    notes/exportnotesjob.cpp
    notes/importnotesjob.cpp
    )

set(pimdataexporter_xml_SRCS
    xml/templateselection.cpp
    )

set(pimdataexporter_akregator_SRCS
    akregator/exportakregatorjob.cpp
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
    pimsettingexportcore_debug.cpp
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
    )

ecm_qt_declare_logging_category(pimdataexporter_LIB_SRCS HEADER pimsettingexportcore_debug.h IDENTIFIER PIMSETTINGEXPORTERCORE_LOG CATEGORY_NAME org.kde.pim.pimdataexportercore)

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

