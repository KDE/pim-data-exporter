/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef IMPORTMAILJOBINTERFACETESTIMPL_H
#define IMPORTMAILJOBINTERFACETESTIMPL_H

#include "exportimportutil.h"

#include "mail/importmailjobinterface.h"

class ImportMailJobInterfaceTestImpl : public ImportMailJobInterface, public ExportImportUtil
{
    Q_OBJECT
public:
    explicit ImportMailJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportMailJobInterfaceTestImpl();
    void start() override;

protected:
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    void synchronizeResource(const QStringList &lst) override;

    Q_REQUIRED_RESULT QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree = false) override;
    void registerSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId) override;
    void importFilters(const QVector<MailCommon::MailFilter *> &filters) override;

    Q_REQUIRED_RESULT QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) override;
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    void addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentities, int oldUid) override;
    void importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId) override;
    void importSmtpMailTransport(const SmtpMailTransport &smtpMailTransport, int defaultTransport, int transportId) override;
private:
    Q_REQUIRED_RESULT QString uniqueIdentityName(const QString &name);
    uint mIdentityUoid = 1;
    uint mMailTransportId = 1;
};

#endif // IMPORTMAILJOBINTERFACETESTIMPL_H