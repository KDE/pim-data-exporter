/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ImportMailJobINTERFACEIMPL_H
#define ImportMailJobINTERFACEIMPL_H

#include "importmailjobinterface.h"
#include "pimdataexportercore_private_export.h"
#include <AkonadiCore/Collection>
#include <Akonadi/KMime/SpecialMailCollections>
#include <QStringList>
#include <QHash>
#include <KArchive>
class ArchiveStorage;
namespace KIdentityManagement {
class Identity;
class IdentityManager;
}
namespace MailTransport {
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
    Q_REQUIRED_RESULT QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree = false) override;
    Q_REQUIRED_RESULT QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) override;
    void synchronizeResource(const QStringList &lst) override;
    void importFilters(const QString &filename) override;
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    void addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentities, int oldUid) override;
    void importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId) override;
    void importSmtpMailTransport(const SmtpMailTransport &smtpMailTransport, int defaultTransport, int transportId) override;
    void addMailTransport(MailTransport::Transport *mt, int defaultTransport, int transportId);
    Q_REQUIRED_RESULT QString configLocation() const override;
private:
    Q_REQUIRED_RESULT QString uniqueIdentityName(const QString &name);
    KIdentityManagement::IdentityManager *const mIdentityManager;
};

#endif // ImportMailJob_H
