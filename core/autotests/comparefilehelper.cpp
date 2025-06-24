/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "comparefilehelper.h"
using namespace Qt::Literals::StringLiterals;

#include <QProcess>
#include <QTest>

void CompareFileHelper::compareFile(const QString &referenceFile, const QString &archiveFile)
{
    QProcess proc;
    // qDebug() << "referenceFile " << referenceFile << " archiveFile " << archiveFile;
#ifdef _WIN32
    QStringList args = QStringList() << u"Compare-Object"_s << QString(u"(Get-Content %1)"_s).arg(referenceFile)
                                     << QString(u"(Get-Content %1)"_s).arg(archiveFile);

    proc.start(u"powershell"_s, args);
    QVERIFY(proc.waitForFinished());

    auto pStdOut = proc.readAllStandardOutput();
    if (!pStdOut.isEmpty()) {
        qDebug() << "Files are different, diff output message:\n" << pStdOut.toStdString().c_str();
    }

    QCOMPARE(pStdOut.size(), 0);
#else
    // compare to reference file
    QStringList args = QStringList() << u"-u"_s << referenceFile << archiveFile;

    proc.setProcessChannelMode(QProcess::ForwardedChannels);
    proc.start(u"diff"_s, args);
    QVERIFY(proc.waitForFinished());
    QCOMPARE(proc.exitCode(), 0);
#endif
}
