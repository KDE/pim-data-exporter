/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXPORTAKREGATORJOBINTERFACETESTIMPL_H
#define EXPORTAKREGATORJOBINTERFACETESTIMPL_H

#include "akregator/exportakregatorjobinterface.h"

class ExportAkregatorJobInterfaceTestImpl : public ExportAkregatorJobInterface
{
    Q_OBJECT
public:
    explicit ExportAkregatorJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportAkregatorJobInterfaceTestImpl();
protected:
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    Q_REQUIRED_RESULT QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree) override;
};
#endif // EXPORTAKREGATORJOBINTERFACETESTIMPL_H
