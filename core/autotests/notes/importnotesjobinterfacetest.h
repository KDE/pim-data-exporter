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

#ifndef IMPORTNOTESJOBINTERFACETEST_H
#define IMPORTNOTESJOBINTERFACETEST_H

#include <QObject>

#include "notes/importnotesjobinterface.h"

class ImportNotesJobInterfaceTestImpl : public ImportNotesJobInterface
{
    Q_OBJECT
public:
    explicit ImportNotesJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportNotesJobInterfaceTestImpl();

protected:
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    Q_REQUIRED_RESULT QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree) override;
    void synchronizeResource(const QStringList &lst) override;
};

class ImportNotesJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportNotesJobInterfaceTest(QObject *parent = nullptr);
    ~ImportNotesJobInterfaceTest() = default;
private Q_SLOTS:
    void importNoteConfig();
    void importNoteConfig_data();
    void importNoteConfigAndResources();
    void importNoteConfigAndResources_data();
};

#endif // IMPORTNOTESJOBINTERFACETEST_H
