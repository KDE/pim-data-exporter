/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMPORTEXPORTMAILUTIL_H
#define IMPORTEXPORTMAILUTIL_H

#include <KConfigGroup>
#include "pimdataexportercore_private_export.h"
namespace ImportExportMailUtil {
PIMDATAEXPORTER_TESTS_EXPORT void cleanupFolderSettings(KConfigGroup &oldGroup);
}

#endif // IMPORTEXPORTMAILUTIL_H
