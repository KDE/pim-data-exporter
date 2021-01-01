/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdatabackuprestoreui.h"
#include "importexportprogressindicatorgui.h"
#include "abstractimportexportjob.h"
#include <KMessageBox>
#include <KLocalizedString>

PimDataBackupRestoreUI::PimDataBackupRestoreUI(QWidget *parentWidget, QObject *parent)
    : PimDataBackupRestore(parent)
    , mParentWidget(parentWidget)
{
}

PimDataBackupRestoreUI::~PimDataBackupRestoreUI()
{
}

bool PimDataBackupRestoreUI::continueToRestore()
{
    if (KMessageBox::No
        == KMessageBox::questionYesNo(mParentWidget,
                                      i18n("The archive was created by a newer version of this program. It might contain additional data which will be skipped during import. Do you want to import it?"),
                                      i18n("Not correct version"))) {
        return false;
    }
    return true;
}

void PimDataBackupRestoreUI::addExportProgressIndicator()
{
    mImportExportData->setImportExportProgressIndicator(new ImportExportProgressIndicatorGui(mParentWidget, this));
}
