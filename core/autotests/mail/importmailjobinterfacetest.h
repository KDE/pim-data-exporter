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

#ifndef IMPORTMAILJOBINTERFACETEST_H
#define IMPORTMAILJOBINTERFACETEST_H

#include <QObject>

#include "mail/importmailjobinterface.h"

class ImportMailJobInterfaceTestImpl : public ImportMailJobInterface
{
    Q_OBJECT
public:
    explicit ImportMailJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportMailJobInterfaceTestImpl();

protected:
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    void synchronizeResource(const QStringList &lst) override;

    Q_REQUIRED_RESULT QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings) override;
    void registerSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId) override;
    void importFilters(const QVector<MailCommon::MailFilter *> &filters) override;
};

class ImportMailJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ImportMailJobInterfaceTest(QObject *parent = nullptr);
    ~ImportMailJobInterfaceTest() = default;
private Q_SLOTS:
    void importMailConfig();
    void importMailConfig_data();
};

#endif // IMPORTMAILJOBINTERFACETEST_H
