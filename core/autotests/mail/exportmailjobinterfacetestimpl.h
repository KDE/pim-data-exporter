/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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

    [[nodiscard]] Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;

    [[nodiscard]] QList<MailCommon::MailFilter *> filters() override;

    [[nodiscard]] QString convertToFullCollectionPath(const qlonglong collectionValue) override;
    void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) override;
    [[nodiscard]] QList<Utils::AkonadiInstanceInfo> listOfResource() override;
    [[nodiscard]] QString storeResources(KZip *archive, const QString &identifier, const QString &path) override;
    [[nodiscard]] QString resourcePath(const QString &identifier) const override;
    void backupMailResourceFile(const QString &agentIdentifier, const QString &defaultPath) override;
    [[nodiscard]] QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    [[nodiscard]] QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree) override;
    [[nodiscard]] QList<uint> listIdentityUoid() const override;
    void exportFilters() override;
    void exportFolderAttributes() override;
};
