/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "compareimportfile.h"
#include "comparefilehelper.h"
#include "generatelistfilefromarchive.h"
#include "loadlistfromfile.h"
#include <QTest>

CompareImportFile::CompareImportFile() = default;

CompareImportFile::~CompareImportFile() = default;

void CompareImportFile::compareFile()
{
    GenerateListFileFromArchive archive(mArchiveFilePath);
    // qDebug() << " archive " << archive.listFile();

    LoadListFromFile f(mListFilePath + QStringLiteral("/list.txt"));
    const QStringList archiveList = archive.listFile();
    const bool equal = (f.fileList() == archiveList);
    if (!equal) {
        qDebug() << "Requested : " << f.fileList();
        qDebug() << "List File : " << archiveList;
    }
    QVERIFY(equal);
    for (QString file : archiveList) {
        file.replace(QStringLiteral("configs/"), QStringLiteral("config/"));
        if (file == QLatin1StringView("information/exportdatatype.xml") || file == QLatin1StringView("information/VERSION_2")) {
            continue;
        }
        // TODO verify with qt6 path
        const QString kxmguiPath{QStringLiteral("share/kxmlgui5/")};
        file.replace(QStringLiteral("config/kleopatra.rc"), kxmguiPath + QStringLiteral("kleopatra/kleopatra.rc"));
        file.replace(QStringLiteral("config/akonadiconsoleui.rc"), kxmguiPath + QStringLiteral("akonadiconsole/akonadiconsoleui.rc"));
        file.replace(QStringLiteral("config/kmail_part.rc"), kxmguiPath + QStringLiteral("kmail2/kmail_part.rc"));
        file.replace(QStringLiteral("config/kmcomposerui.rc"), kxmguiPath + QStringLiteral("kmail2/kmcomposerui.rc"));
        file.replace(QStringLiteral("config/kmmainwin.rc"), kxmguiPath + QStringLiteral("kmail2/kmmainwin.rc"));
        file.replace(QStringLiteral("config/kmreadermainwin.rc"), kxmguiPath + QStringLiteral("kmail2/kmmainwin.rc"));
        file.replace(QStringLiteral("config/kontactsummary_part.rc"), kxmguiPath + QStringLiteral("kontactsummary/kontactsummary_part.rc"));
        file.replace(QStringLiteral("config/kwatchgnupgui.rc"), kxmguiPath + QStringLiteral("kwatchgnupg/kwatchgnupgui.rc"));
        if (file.endsWith(QLatin1StringView("ui.rc")) && !file.contains(QStringLiteral("kmcomposerui.rc"))
            && !file.contains(QStringLiteral("akonadiconsoleui.rc")) && !file.contains(QStringLiteral("kwatchgnupgui.rc"))) {
            QString fileName = file;
            fileName.remove(QStringLiteral("config/"));
            const QString fileuirc = fileName;
            fileName.remove(QStringLiteral("ui.rc"));
            file = kxmguiPath + fileName + QLatin1Char('/') + fileuirc;
        }

        // file.replace(QStringLiteral("identities/"), QStringLiteral("config/"));
        file.replace(QStringLiteral("transports/"), QStringLiteral("config/"));
        // Allow to remove some file from resources path
        if (file.startsWith(mExcludePath)) {
            continue;
        }
        CompareFileHelper::compareFile(mListFilePath + QStringLiteral("/references/") + file, mInstallPath + QLatin1Char('/') + file);
    }
}

QString CompareImportFile::archiveFilePath() const
{
    return mArchiveFilePath;
}

void CompareImportFile::setArchiveFilePath(const QString &archiveFilePath)
{
    mArchiveFilePath = archiveFilePath;
}

QString CompareImportFile::listFilePath() const
{
    return mListFilePath;
}

void CompareImportFile::setListFilePath(const QString &listFilePath)
{
    mListFilePath = listFilePath;
}

QString CompareImportFile::installPath() const
{
    return mInstallPath;
}

void CompareImportFile::setInstallPath(const QString &installPath)
{
    mInstallPath = installPath;
}

QString CompareImportFile::excludePath() const
{
    return mExcludePath;
}

void CompareImportFile::setExcludePath(const QString &excludePath)
{
    mExcludePath = excludePath;
}
