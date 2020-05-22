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

#ifndef ImportMailJobINTERFACEIMPL_H
#define ImportMailJobINTERFACEIMPL_H

#include "importmailjobinterface.h"
#include "pimdataexportercore_private_export.h"
#include <AkonadiCore/Collection>
#include <Akonadi/KMime/SpecialMailCollections>
#include <QStringList>
#include <QHash>
#include <KArchive>
class KArchiveDirectory;
class KArchiveFile;
class ArchiveStorage;
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
    void importFilters(const QVector<MailCommon::MailFilter *> &filters) override;
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    void addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentities, int oldUid) override;
    void importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId) override;
    void importSmtpMailTransport(const KConfigGroup &group, int defaultTransport, int transportId) override;
private:
    Q_REQUIRED_RESULT QString uniqueIdentityName(const QString &name);
};

#endif // ImportMailJob_H
