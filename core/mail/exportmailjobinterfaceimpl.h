/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "exportmailjobinterface.h"
#include "pimdataexportercore_private_export.h"
#include <KSharedConfig>
#include <ctime>
class ArchiveStorage;
namespace KIdentityManagement
{
class Identity;
class IdentityManager;
}
class PIMDATAEXPORTER_TESTS_EXPORT ExportMailJobInterfaceImpl : public ExportMailJobInterface
{
    Q_OBJECT
public:
    explicit ExportMailJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportMailJobInterfaceImpl() override;

protected:
    void convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection = QString()) override;
    void convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey) override;
    void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey) override;
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    Q_REQUIRED_RESULT QVector<MailCommon::MailFilter *> filters() override;
    Q_REQUIRED_RESULT QString convertToFullCollectionPath(const qlonglong collectionValue) override;
    void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) override;
    Q_REQUIRED_RESULT QVector<Utils::AkonadiInstanceInfo> listOfResource() override;
    Q_REQUIRED_RESULT QString storeResources(KZip *archive, const QString &identifier, const QString &path) override;
    Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const override;
    void backupMailResourceFile(const QString &agentIdentifier, const QString &defaultPath) override;
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    Q_REQUIRED_RESULT QString createResource(const QString &resources,
                                             const QString &name,
                                             const QMap<QString, QVariant> &settings,
                                             bool synchronizeTree) override;
    void exportFilters() override;
    Q_REQUIRED_RESULT QVector<uint> listIdentityUoid() const override;
    void exportFolderAttributes() override;

private:
    KIdentityManagement::IdentityManager *const mIdentityManager;
};
