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

#include "comparefilehelper.h"

#include <QProcess>
#include <QTest>

void CompareFileHelper::compareFile(const QString &referenceFile, const QString &archiveFile)
{
    QProcess proc;
    //qDebug() << "referenceFile " << referenceFile << " archiveFile " << archiveFile;
#ifdef _WIN32
    QStringList args = QStringList()
                       << QStringLiteral("Compare-Object")
                       << QString(QStringLiteral("(Get-Content %1)")).arg(referenceFile)
                       << QString(QStringLiteral("(Get-Content %1)")).arg(archiveFile);

    proc.start(QStringLiteral("powershell"), args);
    QVERIFY(proc.waitForFinished());

    auto pStdOut = proc.readAllStandardOutput();
    if (pStdOut.size()) {
        qDebug() << "Files are different, diff output message:\n"
                 << pStdOut.toStdString().c_str();
    }

    QCOMPARE(pStdOut.size(), 0);
#else
    // compare to reference file
    QStringList args = QStringList()
                       << QStringLiteral("-u")
                       << referenceFile
                       << archiveFile;

    proc.setProcessChannelMode(QProcess::ForwardedChannels);
    proc.start(QStringLiteral("diff"), args);
    QVERIFY(proc.waitForFinished());
    QCOMPARE(proc.exitCode(), 0);
#endif
}
