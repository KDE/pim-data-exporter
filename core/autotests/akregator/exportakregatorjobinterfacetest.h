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

#ifndef EXPORTAKREGATORJOBINTERFACETEST_H
#define EXPORTAKREGATORJOBINTERFACETEST_H

#include <QObject>

#include "akregator/exportakregatorjobinterface.h"

class ExportAkregatorJobInterfaceTestImpl : public ExportAkregatorJobInterface
{
    Q_OBJECT
public:
    explicit ExportAkregatorJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportAkregatorJobInterfaceTestImpl();
protected:
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
};

class ExportAkregatorJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportAkregatorJobInterfaceTest(QObject *parent = nullptr);
    ~ExportAkregatorJobInterfaceTest() = default;
private Q_SLOTS:
    void exportAkregatorConfig();
    void exportAkregatorConfig_data();
    void exportAkregatorConfigAndResource_data();
    void exportAkregatorConfigAndResource();
};

#endif // EXPORTAKREGATORJOBINTERFACETEST_H
