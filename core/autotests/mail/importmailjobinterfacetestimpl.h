/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "exportimportutil.h"

#include "mail/importmailjobinterface.h"

class ImportMailJobInterfaceTestImpl : public ImportMailJobInterface, public ExportImportUtil
{
    Q_OBJECT
public:
    explicit ImportMailJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportMailJobInterfaceTestImpl() override;
    void start() override;
    void setExistingPathConfig(const QString &path);

protected:
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    void synchronizeResource(const QStringList &lst) override;

    Q_REQUIRED_RESULT QString createResource(const QString &resources,
                                             const QString &name,
                                             const QMap<QString, QVariant> &settings,
                                             bool synchronizeTree = false) override;
    void registerSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId) override;
    void importFilters(const QString &filename) override;

    Q_REQUIRED_RESULT QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) override;
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    void addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentities, int oldUid) override;
    void importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId) override;
    void importSmtpMailTransport(const SmtpMailTransport &smtpMailTransport, int defaultTransport, int transportId) override;
    Q_REQUIRED_RESULT QString configLocation() const override;

private:
    Q_REQUIRED_RESULT QString uniqueIdentityName(const QString &name);
    uint mIdentityUoid = 1;
    uint mMailTransportId = 1;
    QString mExistingPathConfig;
};
