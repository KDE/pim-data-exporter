/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdatabackuprestoreui.h"
#include "abstractimportexportjob.h"
#include "importexportprogressindicatorgui.h"
#include <KLocalizedString>
#include <KMessageBox>

PimDataBackupRestoreUI::PimDataBackupRestoreUI(QWidget *parentWidget, QObject *parent)
    : PimDataBackupRestore(parent)
    , mParentWidget(parentWidget)
{
}

PimDataBackupRestoreUI::~PimDataBackupRestoreUI() = default;

bool PimDataBackupRestoreUI::continueToRestore()
{
    const int answer =
        KMessageBox::questionTwoActions(mParentWidget,
                                        i18n("The archive was created by a newer version of this program. It might contain additional data which "
                                             "will be skipped during import. Do you want to import it?"),
                                        i18nc("@title:window", "Not correct version"),
                                        KGuiItem(i18nc("@action:button", "Import"), QStringLiteral("document-import")),
                                        KStandardGuiItem::cancel());
    if (answer == KMessageBox::ButtonCode::SecondaryAction) {
        return false;
    }
    return true;
}

void PimDataBackupRestoreUI::addExportProgressIndicator()
{
    mImportExportData->setImportExportProgressIndicator(new ImportExportProgressIndicatorGui(mParentWidget, this));
}

#include "moc_pimdatabackuprestoreui.cpp"
