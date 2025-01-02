/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importakregatorjobinterface.h"
#include "archivestorage.h"

#include <PimCommonAkonadi/CreateResource>

#include <KArchive>
#include <KLocalizedString>
#include <KZip>
#include <QTimer>

#include <QStandardPaths>

ImportAkregatorJobInterface::ImportAkregatorJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
    initializeImportJob();
}

ImportAkregatorJobInterface::~ImportAkregatorJobInterface() = default;

void ImportAkregatorJobInterface::start()
{
    Q_EMIT title(i18n("Starting to import Akregator settings..."));
    mArchiveDirectory = archive()->directory();
    initializeListStep();
    createProgressDialog(i18n("Import Akregator settings"));
    QTimer::singleShot(0, this, &ImportAkregatorJobInterface::slotNextStep);
}

Akonadi::Collection::Id ImportAkregatorJobInterface::convertFolderPathToCollectionId(const QString &path)
{
    Q_UNUSED(path);
    Q_UNREACHABLE();
    // Unused here.
    return -1;
}

QString
ImportAkregatorJobInterface::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNUSED(resources);
    Q_UNUSED(name);
    Q_UNUSED(settings);
    Q_UNUSED(synchronizeTree);

    Q_UNREACHABLE();
    return {};
}

QString ImportAkregatorJobInterface::applicationName() const
{
    return QStringLiteral("[Akregator]");
}

QString ImportAkregatorJobInterface::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    Q_UNUSED(overwriteResources);
    Q_UNUSED(resourceConfig);
    Q_UNUSED(storePath);
    Q_UNREACHABLE();
    // Unused
    return {};
}

void ImportAkregatorJobInterface::slotNextStep()
{
    ++mIndex;
    if (mIndex < mListStep.count()) {
        const Utils::StoredType type = mListStep.at(mIndex);
        if (type == Utils::Config) {
            restoreConfig();
        } else if (type == Utils::Data) {
            restoreData();
        } else {
            qCDebug(PIMDATAEXPORTERCORE_LOG) << Q_FUNC_INFO << " not supported type " << type;
            slotNextStep();
        }
    } else {
        Q_EMIT jobFinished();
    }
}

void ImportAkregatorJobInterface::restoreConfig()
{
    const QString akregatorStr(QStringLiteral("akregatorrc"));
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore configs..."));
    restoreConfigFile(akregatorStr);
    restoreUiRcFile(QStringLiteral("akregator_part.rc"), QStringLiteral("akregator"));
    restoreUiRcFile(QStringLiteral("akregator_shell.rc"), QStringLiteral("akregator"));
    restoreConfigFile(QStringLiteral("akregator.notifyrc"));

    emitInfo(i18n("Config restored."));
    QTimer::singleShot(0, this, &ImportAkregatorJobInterface::slotNextStep);
}

void ImportAkregatorJobInterface::restoreData()
{
    increaseProgressDialog();
    setProgressDialogLabel(i18n("Restore data..."));
    const KArchiveEntry *akregatorEntry = mArchiveDirectory->entry(Utils::dataPath() + QStringLiteral("akregator/"));
    if (akregatorEntry && akregatorEntry->isDirectory()) {
        const QString akregatorPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + QStringLiteral("akregator/");
        overwriteDirectory(akregatorPath, akregatorEntry);
    }
    emitInfo(i18n("Data restored."));
    QTimer::singleShot(0, this, &ImportAkregatorJobInterface::slotNextStep);
}

#include "moc_importakregatorjobinterface.cpp"
