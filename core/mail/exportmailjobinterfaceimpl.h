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

#ifndef EXPORTMAILJOBINTERFACEIMPL_H
#define EXPORTMAILJOBINTERFACEIMPL_H

#include "exportmailjobinterface.h"
#include "pimdataexportercore_private_export.h"
#include <KSharedConfig>
#include <QDateTime>
#include <time.h>
class ArchiveStorage;
namespace KIdentityManagement {
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
    Q_REQUIRED_RESULT QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree) override;
    void exportFilters() override;
    Q_REQUIRED_RESULT QVector<uint> listIdentityUoid() const override;
private:
    KIdentityManagement::IdentityManager *mIdentityManager = nullptr;
};

#endif // ExportMailJob_H
