/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexportercore_private_export.h"
#include <KConfigGroup>
namespace ImportExportMailUtil
{
PIMDATAEXPORTER_TESTS_EXPORT void cleanupFolderSettings(KConfigGroup &oldGroup);

struct AttributeInfo {
    QByteArray displayAttribute;
    QByteArray expireAttribute;
    QByteArray favoriteAttribute;
    Q_REQUIRED_RESULT bool isValid() const
    {
        return !displayAttribute.isEmpty() || !expireAttribute.isEmpty() || !favoriteAttribute.isEmpty();
    }
};
}
