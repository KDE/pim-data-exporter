/*
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <QObject>
#include "pimdataexporter_export.h"
class KZip;

class PIMDATAEXPORTER_EXPORT ArchiveStorage : public QObject
{
    Q_OBJECT
public:
    explicit ArchiveStorage(const QString &filename, QObject *parent = nullptr);
    ~ArchiveStorage() override;

    void closeArchive();
    Q_REQUIRED_RESULT bool openArchive(bool write);

    KZip *archive() const;
    Q_REQUIRED_RESULT QString filename() const;

Q_SIGNALS:
    void error(const QString &);

private:
    KZip *mArchive = nullptr;
};

