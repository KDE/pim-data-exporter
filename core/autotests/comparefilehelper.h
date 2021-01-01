/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COMPAREFILEHELPER_H
#define COMPAREFILEHELPER_H

#include <QString>

namespace CompareFileHelper {
void compareFile(const QString &referenceFile, const QString &archiveFile);
}

#endif // COMPAREFILEHELPER_H
