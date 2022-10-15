/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdatabackuprestoreui.h"
#include "abstractimportexportjob.h"
#include "importexportprogressindicatorgui.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <kwidgetsaddons_version.h>

PimDataBackupRestoreUI::PimDataBackupRestoreUI(QWidget *parentWidget, QObject *parent)
    : PimDataBackupRestore(parent)
    , mParentWidget(parentWidget)
{
}

PimDataBackupRestoreUI::~PimDataBackupRestoreUI() = default;

bool PimDataBackupRestoreUI::continueToRestore()
{
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
    const int answer =
        KMessageBox::questionTwoActions(mParentWidget,
#else
    const int answer = KMessageBox::questionYesNo(mParentWidget,
#endif
                                        i18n("The archive was created by a newer version of this program. It might contain additional data which "
                                             "will be skipped during import. Do you want to import it?"),
                                        i18n("Not correct version"),
                                        KGuiItem(i18nc("@action:button", "Import"), QStringLiteral("document-import")),
                                        KStandardGuiItem::cancel());
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
    if (answer == KMessageBox::ButtonCode::SecondaryAction) {
#else
    if (answer == KMessageBox::No) {
#endif
        return false;
    }
    return true;
}

void PimDataBackupRestoreUI::addExportProgressIndicator()
{
    mImportExportData->setImportExportProgressIndicator(new ImportExportProgressIndicatorGui(mParentWidget, this));
}
