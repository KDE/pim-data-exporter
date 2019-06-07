include_directories( 
    ${pim-data-exporter_SOURCE_DIR}/core/
    ${pim-data-exporter_BINARY_DIR}/core/
    )

set(pimsettingexporter_calendar_SRCS
    calendar/exportcalendarjob.cpp
    calendar/importcalendarjob.cpp
    )

set(pimsettingexporter_addressbook_SRCS
    addressbook/exportaddressbookjob.cpp
    addressbook/importaddressbookjob.cpp
    )

set(pimsettingexporter_mail_SRCS
    mail/exportmailjob.cpp
    mail/importmailjob.cpp
    )

set(pimsettingexporter_alarm_SRCS
    alarm/exportalarmjob.cpp
    alarm/importalarmjob.cpp
    )

set(pimsettingexporter_notes_SRCS
    notes/exportnotesjob.cpp
    notes/importnotesjob.cpp
    )

set(pimsettingexporter_xml_SRCS
    xml/templateselection.cpp
    )

set(pimsettingexporter_akregator_SRCS
    akregator/exportakregatorjob.cpp
    akregator/importakregatorjob.cpp
    )


set(pimsettingexporter_LIB_SRCS
    ${pimsettingexporter_xml_SRCS}
    ${pimsettingexporter_notes_SRCS}
    ${pimsettingexporter_alarm_SRCS}
    ${pimsettingexporter_mail_SRCS}
    ${pimsettingexporter_calendar_SRCS}
    ${pimsettingexporter_addressbook_SRCS}
    ${pimsettingexporter_jot_SRCS}
    ${pimsettingexporter_akregator_SRCS}
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
    pimsettingimportdatainfofile.cpp
    )

ecm_qt_declare_logging_category(pimsettingexporter_LIB_SRCS HEADER pimsettingexportcore_debug.h IDENTIFIER PIMSETTINGEXPORTERCORE_LOG CATEGORY_NAME org.kde.pim.pimsettingexportercore)

add_library(pimsettingexporterprivate ${pimsettingexporter_LIB_SRCS})
generate_export_header(pimsettingexporterprivate BASE_NAME pimsettingexporter)



target_link_libraries(pimsettingexporterprivate
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

set_target_properties(pimsettingexporterprivate PROPERTIES VERSION ${KDEPIM_LIB_VERSION} SOVERSION ${KDEPIM_LIB_SOVERSION})

install(TARGETS pimsettingexporterprivate ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

