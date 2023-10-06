/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "importmailjobinterface.h"
#include "pimdataexportercore_private_export.h"
#include <Akonadi/Collection>
#include <Akonadi/SpecialMailCollections>
#include <KArchive>
#include <MailTransport/TransportManager>
#include <QStringList>
class ArchiveStorage;
namespace KIdentityManagementCore
{
class Identity;
class IdentityManager;
}
namespace MailTransport
{
class Transport;
}
class PIMDATAEXPORTER_TESTS_EXPORT ImportMailJobInterfaceImpl : public ImportMailJobInterface
{
    Q_OBJECT
public:
    explicit ImportMailJobInterfaceImpl(QObject *widget, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportMailJobInterfaceImpl() override;

protected:
    void registerSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId) override;
    [[nodiscard]] QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree) override;
    [[nodiscard]] QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) override;
    void synchronizeResource(const QStringList &lst) override;
    void importFilters(const QString &filename) override;
    [[nodiscard]] Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    [[nodiscard]] QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    void addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentities, int oldUid) override;
    void importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId) override;
    void importSmtpMailTransport(const SmtpMailTransport &smtpMailTransport, int defaultTransport, int transportId) override;
    void addMailTransport(MailTransport::Transport *mt, int defaultTransport, int transportId);
    [[nodiscard]] QString configLocation() const override;
    void importFolderAttributes() override final;

private:
    [[nodiscard]] int convertEncryptionEnum(int val);
    [[nodiscard]] QString uniqueIdentityName(const QString &name);
    KIdentityManagementCore::IdentityManager *const mIdentityManager;
};
