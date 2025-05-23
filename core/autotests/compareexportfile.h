/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>
class KZip;
class QTemporaryDir;
class CompareExportFile
{
public:
    CompareExportFile();
    ~CompareExportFile();
    void compareFiles();

    [[nodiscard]] QString tempFilePath() const;
    void setTempFilePath(const QString &tempFilePath);

    [[nodiscard]] QString listFilePath() const;
    void setListFilePath(const QString &listFilePath);

private:
    QString mTempFilePath;
    QString mListFilePath;
    KZip *mZip = nullptr;
    QTemporaryDir *mTempDir = nullptr;
};
