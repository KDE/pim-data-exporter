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


#include "compareimportfile.h"

#include <QProcess>
#include <QTest>

CompareImportFile::CompareImportFile()
{

}

CompareImportFile::~CompareImportFile()
{

}

void CompareImportFile::compareFile()
{
    QProcess proc;
    const QStringList args = QStringList()
            << QStringLiteral("-u")
            << mReferenceFilePath
            << mTempFilePath;
    proc.setProcessChannelMode(QProcess::ForwardedChannels);
    proc.start(QStringLiteral("diff"), args);
    QVERIFY(proc.waitForFinished());
    QCOMPARE(proc.exitCode(), 0);
}

QString CompareImportFile::tempFilePath() const
{
    return mTempFilePath;
}

void CompareImportFile::setTempFilePath(const QString &tempFilePath)
{
    mTempFilePath = tempFilePath;
}

QString CompareImportFile::referenceFilePath() const
{
    return mReferenceFilePath;
}

void CompareImportFile::setReferenceFilePath(const QString &referenceFilePath)
{
    mReferenceFilePath = referenceFilePath;
}
