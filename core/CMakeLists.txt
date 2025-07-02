# SPDX-FileCopyrightText: 2011-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_library(pimdataexporterprivate)

target_sources(
    pimdataexporterprivate
    PRIVATE
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
        calendar/exportcalendarjobinterface.cpp
        calendar/importcalendarjobinterface.cpp
        addressbook/exportaddressbookjobinterface.cpp
        addressbook/importaddressbookjobinterface.cpp
        alarm/importalarmjobinterface.cpp
        alarm/exportalarmjobinterface.cpp
        mail/exportmailjobinterface.cpp
        mail/importmailjobinterface.cpp
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
        utils.h
        alarm/exportalarmjobinterface.h
        alarm/importalarmjobinterfaceimpl.h
        alarm/exportalarmjobinterfaceimpl.h
        alarm/importalarmjobinterface.h
        abstractimportexportjob.h
        backupresourcefilejobimpl.h
        akregator/importakregatorjobinterface.h
        akregator/exportakregatorjobinterface.h
        pimdatabackupthread.h
        xml/templateselection.h
        pimdataimportdatainfofile.h
        mail/smtpmailtransport.h
        mail/importmailjobinterfaceimpl.h
        mail/importmailjobinterface.h
        mail/exportmailjobinterfaceimpl.h
        mail/importexportmailutil.h
        mail/exportmailjobinterface.h
        resourceconverterbase.h
        storeresourcejob.h
        calendar/exportcalendarjobinterfaceimpl.h
        calendar/importcalendarjobinterfaceimpl.h
        calendar/exportcalendarjobinterface.h
        calendar/importcalendarjobinterface.h
        pimdataexporterkernel.h
        importexportprogressindicatorbase.h
        resourceconverterimpl.h
        archivestorage.h
        pimdataexportercore_private_export.h
        pimdatabackuprestore.h
        addressbook/exportaddressbookjobinterface.h
        addressbook/exportaddressbookjobinterfaceimpl.h
        addressbook/importaddressbookjobinterfaceimpl.h
        addressbook/importaddressbookjobinterface.h
        exportresourcearchivejob.h
        synchronizeresourcejob.h
        backupresourcefilejobbase.h
        mail/exportmailfolderattributejob.h
        mail/exportmailfolderattributejob.cpp
        mail/exportmailfolderattributejobimpl.h
        mail/exportmailfolderattributejobimpl.cpp
        mail/importmailfolderattributejob.h
        mail/importmailfolderattributejob.cpp
        mail/importmailfolderattributejobimpl.h
        mail/importmailfolderattributejobimpl.cpp
)

ecm_qt_declare_logging_category(pimdataexporterprivate HEADER pimdataexportcore_debug.h IDENTIFIER PIMDATAEXPORTERCORE_LOG CATEGORY_NAME org.kde.pim.pimdataexportercore
        DESCRIPTION "pimdataexporter (pimdataexporter lib core)"
        OLD_CATEGORY_NAMES log_pimsettingexportercore org.kde.pim.pimsettingexportercore
        EXPORT PIMDATAEXPORTER
)

if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(
        pimdataexporterprivate
        PROPERTIES
            UNITY_BUILD
                ON
    )
endif()

generate_export_header(pimdataexporterprivate BASE_NAME pimdataexporter)

target_link_libraries(
    pimdataexporterprivate
    PRIVATE
        KPim6::AkonadiCore
        KPim6::Mime
        KPim6::MailTransport
        KPim6::MailCommon
        KF6::Archive
        KPim6::PimCommonAkonadi
        KF6::I18n
        KF6::CalendarCore
)

set_target_properties(
    pimdataexporterprivate
    PROPERTIES
        VERSION
            ${KDEPIM_LIB_VERSION}
        SOVERSION
            ${KDEPIM_LIB_SOVERSION}
)

install(
    TARGETS
        pimdataexporterprivate
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
    LIBRARY
        NAMELINK_SKIP
)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()
