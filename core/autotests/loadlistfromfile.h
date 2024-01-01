/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QString>
#include <QStringList>

class LoadListFromFile
{
public:
    explicit LoadListFromFile(const QString &filename);
    ~LoadListFromFile();
    [[nodiscard]] QStringList fileList() const;

private:
    QStringList mFileList;
};
