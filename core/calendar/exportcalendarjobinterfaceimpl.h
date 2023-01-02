/*
   SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "exportcalendarjobinterface.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ExportCalendarJobInterfaceImpl : public ExportCalendarJobInterface
{
    Q_OBJECT
public:
    explicit ExportCalendarJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportCalendarJobInterfaceImpl() override;

protected:
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    Q_REQUIRED_RESULT QString convertToFullCollectionPath(const qlonglong collectionValue) override;
    void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey) override;
    void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) override;
    Q_REQUIRED_RESULT QVector<Utils::AkonadiInstanceInfo> listOfResource() override;
    Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const override;
    void backupCalendarResourceFile(const QString &agentIdentifier, const QString &defaultPath) override;
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    Q_REQUIRED_RESULT QString createResource(const QString &resources,
                                             const QString &name,
                                             const QMap<QString, QVariant> &settings,
                                             bool synchronizeTree) override;
};
