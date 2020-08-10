/*
   SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ARCHIVESTORAGE_H
#define ARCHIVESTORAGE_H
#include <QObject>
#include "pimdataexporter_export.h"
class KZip;

class PIMDATAEXPORTER_EXPORT ArchiveStorage : public QObject
{
    Q_OBJECT
public:
    explicit ArchiveStorage(const QString &filename, QObject *parent = nullptr);
    ~ArchiveStorage();

    void closeArchive();
    Q_REQUIRED_RESULT bool openArchive(bool write);

    KZip *archive() const;
    Q_REQUIRED_RESULT QString filename() const;

Q_SIGNALS:
    void error(const QString &);

private:
    KZip *mArchive = nullptr;
};

#endif // ARCHIVESTORAGE_H
