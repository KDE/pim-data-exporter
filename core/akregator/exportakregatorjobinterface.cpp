/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportakregatorjobinterface.h"
#include <KLocalizedString>
#include <KZip>

#include <QDir>
#include <QStandardPaths>
#include <QTimer>

ExportAkregatorJobInterface::ExportAkregatorJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
}

ExportAkregatorJobInterface::~ExportAkregatorJobInterface() = default;

void ExportAkregatorJobInterface::start()
{
    Q_EMIT title(i18n("Start export Akregator settings..."));
    createProgressDialog(i18n("Export Akregator settings"));
    if (mTypeSelected & Utils::Config) {
        QTimer::singleShot(0, this, &ExportAkregatorJobInterface::slotCheckBackupConfig);
    } else if (mTypeSelected & Utils::Data) {
        QTimer::singleShot(0, this, &ExportAkregatorJobInterface::slotCheckBackupData);
    } else {
        Q_EMIT jobFinished();
    }
}

Akonadi::Collection::Id ExportAkregatorJobInterface::convertFolderPathToCollectionId(const QString &path)
{
    Q_UNUSED(path);
    Q_UNREACHABLE();
    // Unusued
    return -1;
}

QString ExportAkregatorJobInterface::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    Q_UNUSED(overwriteResources);
    Q_UNUSED(resourceConfig);
    Q_UNUSED(storePath);
    Q_UNREACHABLE();
    // Unused
    return {};
}

QString
ExportAkregatorJobInterface::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNUSED(resources);
    Q_UNUSED(name);
    Q_UNUSED(settings);
    Q_UNUSED(synchronizeTree);
    Q_UNREACHABLE();
    return {};
}

QString ExportAkregatorJobInterface::applicationName() const
{
    return QStringLiteral("[Akregator]");
}

void ExportAkregatorJobInterface::slotCheckBackupConfig()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Backing up config..."));

    backupConfigFile(QStringLiteral("akregatorrc"));
    backupUiRcFile(QStringLiteral("akregator_part.rc"), QStringLiteral("akregator"));
    backupUiRcFile(QStringLiteral("akregator_shell.rc"), QStringLiteral("akregator"));
    backupConfigFile(QStringLiteral("akregator.notifyrc"));

    emitInfo(i18n("Config backup done."));
    QTimer::singleShot(0, this, &ExportAkregatorJobInterface::slotCheckBackupData);
}

void ExportAkregatorJobInterface::slotCheckBackupData()
{
    if (mTypeSelected & Utils::Data) {
        increaseProgressDialog();
        setProgressDialogLabel(i18n("Backing up data..."));

        const QString akregatorDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/akregator");
        QDir akregatorDirectory(akregatorDir);
        if (akregatorDirectory.exists()) {
            const bool akregatorDirAdded = archive()->addLocalDirectory(akregatorDir, Utils::dataPath() + QStringLiteral("akregator"));
            if (!akregatorDirAdded) {
                Q_EMIT error(i18n("\"%1\" directory cannot be added to backup file.", akregatorDir));
            }
        }
        emitInfo(i18n("Data backup done."));
    }
    Q_EMIT jobFinished();
}

#include "moc_exportakregatorjobinterface.cpp"
