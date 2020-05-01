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

#ifndef EXPORTMAILJOBINTERFACETEST_H
#define EXPORTMAILJOBINTERFACETEST_H

#include <QObject>

#include "mail/exportmailjobinterface.h"

class ExportMailJobInterfaceTestImpl : public ExportMailJobInterface
{
    Q_OBJECT
public:
    explicit ExportMailJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportMailJobInterfaceTestImpl();

protected:
    void exportArchiveResource() override;

protected:
    void convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey) override;
    void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey) override;

    void convertCollectionIdsToRealPath(KConfigGroup &group, const QString &currentKey, const QString &prefixCollection) override;
    void backupResources() override;

    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
};

class ExportMailJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportMailJobInterfaceTest(QObject *parent = nullptr);
    ~ExportMailJobInterfaceTest() = default;
private Q_SLOTS:
    void exportMailConfigTest1();
};

#endif // EXPORTMAILJOBINTERFACETEST_H
