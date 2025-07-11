/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generatelistfilefromarchive.h"
using namespace Qt::Literals::StringLiterals;

#include "core/utils.h"
#include <KZip>
#include <QTest>

GenerateListFileFromArchive::GenerateListFileFromArchive(const QString &fileName)
    : mFileName(fileName)
{
    generateList();
}

GenerateListFileFromArchive::~GenerateListFileFromArchive()
{
    delete mZip;
    mZip = nullptr;
}

void GenerateListFileFromArchive::generateList()
{
    mZip = new KZip(mFileName);
    // qDebug() << " mFileName" << mFileName;
    bool result = mZip->open(QIODevice::ReadOnly);
    QVERIFY(result);
    const KArchiveDirectory *topDirectory = mZip->directory();
    const bool isAValidArchive = searchArchiveElement(Utils::infoPath(), topDirectory);
    QVERIFY(isAValidArchive);
    (void)searchArchiveElement(Utils::mailsPath(), topDirectory);
    (void)searchArchiveElement(Utils::alarmPath(), topDirectory);
    (void)searchArchiveElement(Utils::calendarPath(), topDirectory);
    (void)searchArchiveElement(Utils::addressbookPath(), topDirectory);
    (void)searchArchiveElement(Utils::identitiesPath(), topDirectory);
    (void)searchArchiveElement(Utils::resourcesPath(), topDirectory);
    (void)searchArchiveElement(Utils::configsPath(), topDirectory);
    (void)searchArchiveElement(Utils::transportsPath(), topDirectory);
    (void)searchArchiveElement(Utils::dataPath(), topDirectory);
    (void)searchArchiveElement(Utils::notePath(), topDirectory);
    mListFile.sort();
}

QStringList GenerateListFileFromArchive::listFile() const
{
    return mListFile;
}

bool GenerateListFileFromArchive::searchArchiveElement(const QString &path, const KArchiveDirectory *topDirectory)
{
    const KArchiveEntry *topEntry = topDirectory->entry(path);
    bool result = true;
    if (topEntry) {
        addSubItems(path, topEntry, 0);
    } else {
        result = false;
    }
    return result;
}

void GenerateListFileFromArchive::addSubItems(const QString &topLevelPath, const KArchiveEntry *entry, int indent, const QString &fullpath)
{
    const auto dir = static_cast<const KArchiveDirectory *>(entry);
    const QString space = QString(indent * 2, u' ');
    const QStringList lst = dir->entries();
    for (const QString &entryName : lst) {
        const KArchiveEntry *archiveEntry = dir->entry(entryName);
        if (archiveEntry) {
            if (archiveEntry->isDirectory()) {
                const auto dirEntry = static_cast<const KArchiveDirectory *>(archiveEntry);
                // mListFile += space + dirEntry->name();
                addSubItems(topLevelPath, archiveEntry, indent, (fullpath.isEmpty() ? QString() : fullpath + u'/') + dirEntry->name());
            } else if (archiveEntry->isFile()) {
                const auto file = static_cast<const KArchiveFile *>(archiveEntry);
                const QString fileFullPath = topLevelPath + (fullpath.isEmpty() ? QString() : fullpath + u'/') + file->name();
                mListFile += space + fileFullPath;
            }
        }
    }
}
