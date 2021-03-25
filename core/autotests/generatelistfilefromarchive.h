/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>
#include <QStringList>
class KZip;
class KArchiveDirectory;
class KArchiveEntry;
class GenerateListFileFromArchive
{
public:
    explicit GenerateListFileFromArchive(const QString &fileName);
    ~GenerateListFileFromArchive();
    Q_REQUIRED_RESULT QStringList listFile() const;

private:
    Q_REQUIRED_RESULT bool searchArchiveElement(const QString &path, const KArchiveDirectory *topDirectory);
    void addSubItems(const QString &topLevelPath, const KArchiveEntry *entry, int indent, const QString &fullpath = QString());
    void generateList();
    QString mFileName;
    QStringList mListFile;
    KZip *mZip = nullptr;
};

