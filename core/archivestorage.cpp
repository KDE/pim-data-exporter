/*
   SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "archivestorage.h"
#include <KLocalizedString>
#include <KZip>

ArchiveStorage::ArchiveStorage(const QString &filename, QObject *parent)
    : QObject(parent)
    , mArchive(new KZip(filename))
{
}

ArchiveStorage::~ArchiveStorage()
{
    closeArchive();
    delete mArchive;
}

void ArchiveStorage::closeArchive()
{
    if (mArchive && mArchive->isOpen()) {
        mArchive->close();
    }
}

QString ArchiveStorage::filename() const
{
    if (mArchive) {
        return mArchive->fileName();
    }
    return QString();
}

bool ArchiveStorage::openArchive(bool write)
{
    const bool result = mArchive->open(write ? QIODevice::WriteOnly : QIODevice::ReadOnly);
    if (!result) {
        if (write) {
            Q_EMIT error(i18n("Archive cannot be opened in write mode."));
        } else {
            Q_EMIT error(i18n("Archive cannot be opened in read mode."));
        }
    }
    return result;
}

KZip *ArchiveStorage::archive() const
{
    return mArchive;
}
