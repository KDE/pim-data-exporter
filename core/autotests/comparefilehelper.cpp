/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "comparefilehelper.h"

#include <QProcess>
#include <QTest>

void CompareFileHelper::compareFile(const QString &referenceFile, const QString &archiveFile)
{
    QProcess proc;
    // qDebug() << "referenceFile " << referenceFile << " archiveFile " << archiveFile;
#ifdef _WIN32
    QStringList args = QStringList() << QStringLiteral("Compare-Object") << QString(QStringLiteral("(Get-Content %1)")).arg(referenceFile)
                                     << QString(QStringLiteral("(Get-Content %1)")).arg(archiveFile);

    proc.start(QStringLiteral("powershell"), args);
    QVERIFY(proc.waitForFinished());

    auto pStdOut = proc.readAllStandardOutput();
    if (pStdOut.size()) {
        qDebug() << "Files are different, diff output message:\n" << pStdOut.toStdString().c_str();
    }

    QCOMPARE(pStdOut.size(), 0);
#else
    // compare to reference file
    QStringList args = QStringList() << QStringLiteral("-u") << referenceFile << archiveFile;

    proc.setProcessChannelMode(QProcess::ForwardedChannels);
    proc.start(QStringLiteral("diff"), args);
    QVERIFY(proc.waitForFinished());
    QCOMPARE(proc.exitCode(), 0);
#endif
}
