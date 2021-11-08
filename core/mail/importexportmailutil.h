/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexportercore_private_export.h"
#include <KConfigGroup>
namespace ImportExportMailUtil
{
PIMDATAEXPORTER_TESTS_EXPORT void cleanupFolderSettings(KConfigGroup &oldGroup);
}
