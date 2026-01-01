/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "compareimportfile.h"
using namespace Qt::Literals::StringLiterals;

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

    LoadListFromFile f(mListFilePath + u"/list.txt"_s);
    const QStringList archiveList = archive.listFile();
    const bool equal = (f.fileList() == archiveList);
    if (!equal) {
        qDebug() << "Requested : " << f.fileList();
        qDebug() << "List File : " << archiveList;
    }
    QVERIFY(equal);
    for (QString file : archiveList) {
        file.replace(u"configs/"_s, u"config/"_s);
        if (file == QLatin1StringView("information/exportdatatype.xml") || file == QLatin1StringView("information/VERSION_2")) {
            continue;
        }
        // TODO verify with qt6 path
        const QString kxmguiPath{u"share/kxmlgui5/"_s};
        file.replace(u"config/kleopatra.rc"_s, kxmguiPath + u"kleopatra/kleopatra.rc"_s);
        file.replace(u"config/akonadiconsoleui.rc"_s, kxmguiPath + u"akonadiconsole/akonadiconsoleui.rc"_s);
        file.replace(u"config/kmail_part.rc"_s, kxmguiPath + u"kmail2/kmail_part.rc"_s);
        file.replace(u"config/kmcomposerui.rc"_s, kxmguiPath + u"kmail2/kmcomposerui.rc"_s);
        file.replace(u"config/kmmainwin.rc"_s, kxmguiPath + u"kmail2/kmmainwin.rc"_s);
        file.replace(u"config/kmreadermainwin.rc"_s, kxmguiPath + u"kmail2/kmmainwin.rc"_s);
        file.replace(u"config/kontactsummary_part.rc"_s, kxmguiPath + u"kontactsummary/kontactsummary_part.rc"_s);
        file.replace(u"config/kwatchgnupgui.rc"_s, kxmguiPath + u"kwatchgnupg/kwatchgnupgui.rc"_s);
        if (file.endsWith(QLatin1StringView("ui.rc")) && !file.contains(u"kmcomposerui.rc"_s) && !file.contains(u"akonadiconsoleui.rc"_s)
            && !file.contains(u"kwatchgnupgui.rc"_s)) {
            QString fileName = file;
            fileName.remove(u"config/"_s);
            const QString fileuirc = fileName;
            fileName.remove(u"ui.rc"_s);
            file = kxmguiPath + fileName + u'/' + fileuirc;
        }

        // file.replace(u"identities/"_s, u"config/"_s);
        file.replace(u"transports/"_s, u"config/"_s);
        // Allow to remove some file from resources path
        if (file.startsWith(mExcludePath)) {
            continue;
        }
        CompareFileHelper::compareFile(mListFilePath + u"/references/"_s + file, mInstallPath + u'/' + file);
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
