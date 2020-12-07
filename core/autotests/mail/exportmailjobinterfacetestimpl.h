/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXPORTMAILJOBINTERFACETESTIMPL_H
#define EXPORTMAILJOBINTERFACETESTIMPL_H

#include "exportimportutil.h"

#include "mail/exportmailjobinterface.h"

class ExportMailJobInterfaceTestImpl : public ExportMailJobInterface, public ExportImportUtil
{
    Q_OBJECT
public:
    explicit ExportMailJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportMailJobInterfaceTestImpl() override;

protected:
    void convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey) override;
    void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey) override;

    void convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection) override;

    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;

    Q_REQUIRED_RESULT QVector<MailCommon::MailFilter *> filters() override;

    Q_REQUIRED_RESULT QString convertToFullCollectionPath(const qlonglong collectionValue) override;
    void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) override;
    Q_REQUIRED_RESULT QVector<Utils::AkonadiInstanceInfo> listOfResource() override;
    Q_REQUIRED_RESULT QString storeResources(KZip *archive, const QString &identifier, const QString &path) override;
    Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const override;
    void backupMailResourceFile(const QString &agentIdentifier, const QString &defaultPath) override;
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    Q_REQUIRED_RESULT QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree) override;
    Q_REQUIRED_RESULT QVector<uint> listIdentityUoid() const override;
    void exportFilters() override;
};
#endif // EXPORTMAILJOBINTERFACETESTIMPL_H
