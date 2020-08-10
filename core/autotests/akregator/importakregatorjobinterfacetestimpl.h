/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTAKREGATORJOBINTERFACETESTIMPL_H
#define IMPORTAKREGATORJOBINTERFACETESTIMPL_H

#include "akregator/importakregatorjobinterface.h"

class ImportAkregatorJobInterfaceTestImpl : public ImportAkregatorJobInterface
{
    Q_OBJECT
public:
    explicit ImportAkregatorJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportAkregatorJobInterfaceTestImpl();

protected:
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
};

#endif // IMPORTAKREGATORJOBINTERFACETESTIMPL_H
