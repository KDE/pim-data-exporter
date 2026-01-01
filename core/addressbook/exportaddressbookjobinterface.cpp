/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportaddressbookjobinterface.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

#include <KConfigGroup>
#include <QFileInfo>
#include <QTemporaryFile>

#include "resourceconverterimpl.h"
#include <QStandardPaths>
#include <QTimer>

ExportAddressbookJobInterface::ExportAddressbookJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
}

ExportAddressbookJobInterface::~ExportAddressbookJobInterface() = default;

void ExportAddressbookJobInterface::start()
{
    Q_EMIT title(i18n("Start export KAddressBook settings..."));
    createProgressDialog(i18n("Export KAddressBook settings"));
    if (mTypeSelected & Utils::Resources) {
        QTimer::singleShot(0, this, &ExportAddressbookJobInterface::slotCheckBackupResource);
    } else if (mTypeSelected & Utils::Config) {
        QTimer::singleShot(0, this, &ExportAddressbookJobInterface::slotCheckBackupConfig);
    } else {
        Q_EMIT jobFinished();
    }
}

void ExportAddressbookJobInterface::slotCheckBackupResource()
{
    mAkonadiInstanceInfo = listOfResource();
    setProgressDialogLabel(i18n("Backing up resources..."));
    increaseProgressDialog();
    QTimer::singleShot(0, this, &ExportAddressbookJobInterface::slotWriteNextArchiveResource);
}

void ExportAddressbookJobInterface::slotCheckBackupConfig()
{
    if (mTypeSelected & Utils::Config) {
        backupConfig();
        increaseProgressDialog();
        if (wasCanceled()) {
            Q_EMIT jobFinished();
            return;
        }
    }
    Q_EMIT jobFinished();
}

QString ExportAddressbookJobInterface::applicationName() const
{
    return u"[KAddressBook]"_s;
}

void ExportAddressbookJobInterface::slotAddressbookJobTerminated()
{
    if (wasCanceled()) {
        Q_EMIT jobFinished();
        return;
    }
    mIndexIdentifier++;
    QTimer::singleShot(0, this, &ExportAddressbookJobInterface::slotWriteNextArchiveResource);
}

void ExportAddressbookJobInterface::slotWriteNextArchiveResource()
{
    if (mIndexIdentifier < mAkonadiInstanceInfo.count()) {
        const Utils::AkonadiInstanceInfo agent = mAkonadiInstanceInfo.at(mIndexIdentifier);
        const QString identifier = agent.identifier;
        if (identifier.contains(QLatin1StringView("akonadi_vcarddir_resource_")) || identifier.contains(QLatin1StringView("akonadi_contacts_resource_"))) {
            const QString archivePath = Utils::addressbookPath() + identifier + u'/';

            const QString url = resourcePath(identifier, QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + u"/contacts/"_s);
            if (!mAgentPaths.contains(url)) {
                if (!url.isEmpty()) {
                    mAgentPaths << url;
                    exportResourceToArchive(archivePath, url, identifier);
                } else {
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << "Url is empty for " << identifier;
                    QTimer::singleShot(0, this, &ExportAddressbookJobInterface::slotAddressbookJobTerminated);
                }
            } else {
                QTimer::singleShot(0, this, &ExportAddressbookJobInterface::slotAddressbookJobTerminated);
            }
        } else if (identifier.contains(QLatin1StringView("akonadi_vcard_resource_"))) {
            backupAddressBookResourceFile(identifier, Utils::addressbookPath());
            QTimer::singleShot(0, this, &ExportAddressbookJobInterface::slotAddressbookJobTerminated);
        } else {
            QTimer::singleShot(0, this, &ExportAddressbookJobInterface::slotAddressbookJobTerminated);
        }
    } else {
        emitInfo(i18n("Resources backup done."));
        QTimer::singleShot(0, this, &ExportAddressbookJobInterface::slotCheckBackupConfig);
    }
}

void ExportAddressbookJobInterface::backupConfig()
{
    setProgressDialogLabel(i18n("Backing up config..."));

    const QString kaddressbookStr(u"kaddressbookrc"_s);
    const QString kaddressbookrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u'/' + kaddressbookStr;
    if (QFileInfo::exists(kaddressbookrc)) {
        KSharedConfigPtr kaddressbook = KSharedConfig::openConfig(kaddressbookrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *kaddressBookConfig = kaddressbook->copyTo(tmp.fileName());

        const QString collectionViewCheckStateStr(u"CollectionViewCheckState"_s);
        if (kaddressBookConfig->hasGroup(collectionViewCheckStateStr)) {
            KConfigGroup group = kaddressBookConfig->group(collectionViewCheckStateStr);
            const QString selectionKey(u"Selection"_s);
            convertCollectionListToRealPath(group, selectionKey);
        }

        const QString collectionViewStateStr(u"CollectionViewState"_s);
        if (kaddressBookConfig->hasGroup(collectionViewStateStr)) {
            KConfigGroup group = kaddressBookConfig->group(collectionViewStateStr);
            QString currentKey(u"Current"_s);
            convertCollectionToRealPath(group, currentKey);

            currentKey = u"Expansion"_s;
            convertCollectionListToRealPath(group, currentKey);

            currentKey = u"Selection"_s;
            convertCollectionToRealPath(group, currentKey);
        }
        kaddressBookConfig->sync();
        backupFile(tmp.fileName(), Utils::configsPath(), kaddressbookStr);
        delete kaddressBookConfig;
    }
    backupUiRcFile(u"kaddressbookui.rc"_s, u"kaddressbook"_s);

    storeDirectory(u"/kaddressbook/csv-templates/"_s);
    storeDirectory(u"/kaddressbook/viewertemplates/"_s);
    storeDirectory(u"/kaddressbook/printing/"_s);

    emitInfo(i18n("Config backup done."));
}

#include "moc_exportaddressbookjobinterface.cpp"
