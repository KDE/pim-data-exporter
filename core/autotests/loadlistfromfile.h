/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef LOADLISTFROMFILE_H
#define LOADLISTFROMFILE_H

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

#endif // LOADLISTFROMFILE_H
