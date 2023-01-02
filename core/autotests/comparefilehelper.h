/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>

namespace CompareFileHelper
{
void compareFile(const QString &referenceFile, const QString &archiveFile);
}
