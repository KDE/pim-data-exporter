/*
   SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importexportmailutil.h"

void ImportExportMailUtil::cleanupFolderSettings(KConfigGroup &oldGroup)
{
    const bool mailingListEnabled = oldGroup.readEntry("MailingListEnabled", false);
    if (!mailingListEnabled) {
        oldGroup.deleteEntry("MailingListEnabled");
    }
    const int mailingListFeatures = oldGroup.readEntry("MailingListFeatures", 0);
    if (mailingListFeatures == 0) {
        oldGroup.deleteEntry("MailingListFeatures");
    }
    const int mailingListHandler = oldGroup.readEntry("MailingListHandler", 0);
    if (mailingListHandler == 0) {
        oldGroup.deleteEntry("MailingListHandler");
    }
    const QString mailingListId = oldGroup.readEntry("MailingListId");
    if (mailingListId.isEmpty()) {
        oldGroup.deleteEntry("MailingListId");
    }

    const bool putRepliesInSameFolder = oldGroup.readEntry("PutRepliesInSameFolder", false);
    if (!putRepliesInSameFolder) {
        oldGroup.deleteEntry("PutRepliesInSameFolder");
    }

    const bool folderHtmlLoadExtPreference = oldGroup.readEntry("htmlLoadExternalOverride", false);
    if (!folderHtmlLoadExtPreference) {
        oldGroup.deleteEntry("htmlLoadExternalOverride");
    }
    const bool useDefaultIdentity = oldGroup.readEntry("UseDefaultIdentity", false);
    if (useDefaultIdentity) {
        oldGroup.deleteEntry("UseDefaultIdentity");
    }
}
