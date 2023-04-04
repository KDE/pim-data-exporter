/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "exportnotesjobinterface.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ExportNotesJobInterfaceImpl : public ExportNotesJobInterface
{
    Q_OBJECT
public:
    explicit ExportNotesJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportNotesJobInterfaceImpl() override;

protected:
    void convertCollectionIdsToRealPath(KConfigGroup &selectFolderNoteGroup, const QString &selectFolderNoteGroupStr) override;
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) override;
    Q_REQUIRED_RESULT QList<Utils::AkonadiInstanceInfo> listOfResource() override;
    Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const override;
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    Q_REQUIRED_RESULT QString createResource(const QString &resources,
                                             const QString &name,
                                             const QMap<QString, QVariant> &settings,
                                             bool synchronizeTree) override;
};
