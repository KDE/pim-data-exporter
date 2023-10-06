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
    [[nodiscard]] Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    [[nodiscard]] QString convertToFullCollectionPath(const qlonglong collectionValue) override;
    void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey) override;
    void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) override;
    [[nodiscard]] QList<Utils::AkonadiInstanceInfo> listOfResource() override;
    [[nodiscard]] QString resourcePath(const QString &identifier) const override;
    void backupCalendarResourceFile(const QString &agentIdentifier, const QString &defaultPath) override;
    [[nodiscard]] QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    [[nodiscard]] QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree) override;
};
