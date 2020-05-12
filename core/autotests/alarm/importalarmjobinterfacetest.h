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

#ifndef IMPORTALARMJOBINTERFACETEST_H
#define IMPORTALARMJOBINTERFACETEST_H

#include <QObject>
#include "exportimportutil.h"

#include "alarm/importalarmjobinterface.h"

class ImportAlarmJobInterfaceTestImpl : public ImportAlarmJobInterface, public ExportImportUtil
{
    Q_OBJECT
public:
    explicit ImportAlarmJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportAlarmJobInterfaceTestImpl();

protected:
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    Q_REQUIRED_RESULT QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree) override;
    void synchronizeResource(const QStringList &lst) override;
    Q_REQUIRED_RESULT QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) override;
};

class ImportAlarmJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportAlarmJobInterfaceTest(QObject *parent = nullptr);
    ~ImportAlarmJobInterfaceTest() = default;
private Q_SLOTS:
    void importCalendarConfig();
    void importCalendarConfig_data();
};

#endif // IMPORTALARMJOBINTERFACETEST_H
