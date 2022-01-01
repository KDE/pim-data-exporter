/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

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
    Q_REQUIRED_RESULT QStringList fileList() const;

private:
    QStringList mFileList;
};
