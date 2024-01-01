/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "compareloggingfile.h"
#include "comparefilehelper.h"
#include <QDebug>

CompareLoggingFile::CompareLoggingFile() = default;

void CompareLoggingFile::compare()
{
    qDebug() << " mListFilePath " << mListFilePath << " mLoggingFilePath " << mLoggingFilePath;
    CompareFileHelper::compareFile(mListFilePath + QStringLiteral("/references/loggingfile.txt"), mLoggingFilePath);
}

QString CompareLoggingFile::loggingFilePath() const
{
    return mLoggingFilePath;
}

void CompareLoggingFile::setLoggingFilePath(const QString &loggingFilePath)
{
    mLoggingFilePath = loggingFilePath;
}

QString CompareLoggingFile::listFilePath() const
{
    return mListFilePath;
}

void CompareLoggingFile::setListFilePath(const QString &listFilePath)
{
    mListFilePath = listFilePath;
}
