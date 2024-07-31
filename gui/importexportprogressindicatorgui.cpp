/*
   SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importexportprogressindicatorgui.h"
#include "pimdataexporterglobalconfig.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KStandardGuiItem>
#include <QProgressDialog>

ImportExportProgressIndicatorGui::ImportExportProgressIndicatorGui(QWidget *parentWidget, QObject *parent)
    : ImportExportProgressIndicatorBase(parent)
    , mParentWidget(parentWidget)
{
}

ImportExportProgressIndicatorGui::~ImportExportProgressIndicatorGui()
{
    delete mProgressDialog;
}

void ImportExportProgressIndicatorGui::increaseProgressDialog()
{
    if (mProgressDialog) {
        mProgressDialog->setValue(mProgressDialog->value() + 1);
    }
}

void ImportExportProgressIndicatorGui::createProgressDialog(const QString &title)
{
    if (!mProgressDialog) {
        mProgressDialog = new QProgressDialog(QString(), i18n("Cancel"), 0, mNumberOfStep, mParentWidget);
        mProgressDialog->setWindowTitle(title);
        mProgressDialog->setWindowModality(Qt::WindowModal);
        connect(mProgressDialog, &QProgressDialog::canceled, this, &ImportExportProgressIndicatorBase::canceled);
    }
    mProgressDialog->show();
    mProgressDialog->setValue(0);
}

void ImportExportProgressIndicatorGui::setProgressDialogLabel(const QString &text)
{
    if (mProgressDialog) {
        mProgressDialog->setLabelText(text);
    }
    Q_EMIT info(text);
}

bool ImportExportProgressIndicatorGui::wasCanceled() const
{
    if (mProgressDialog) {
        return mProgressDialog->wasCanceled();
    }
    return false;
}

int ImportExportProgressIndicatorGui::mergeConfigMessageBox(const QString &configName) const
{
    if (PimDataExportGlobalConfig::self()->alwaysMergeConfigFile()) {
        return KMessageBox::ButtonCode::PrimaryAction;
    }
    return KMessageBox::warningTwoActionsCancel(mParentWidget,
                                                i18n("\"%1\" already exists. Do you want to overwrite it or merge it?", configName),
                                                i18nc("@title:window", "Restore"),
                                                KStandardGuiItem::overwrite(),
                                                KGuiItem(i18nc("@action:button", "Merge")));
}

bool ImportExportProgressIndicatorGui::overwriteConfigMessageBox(const QString &configName) const
{
    if (PimDataExportGlobalConfig::self()->alwaysOverrideFile()) {
        return true;
    }
    const int answer = KMessageBox::warningTwoActions(mParentWidget,
                                                      i18n("\"%1\" already exists. Do you want to overwrite it?", configName),
                                                      i18nc("@title:window", "Restore"),
                                                      KStandardGuiItem::overwrite(),
                                                      KStandardGuiItem::cancel());
    return (answer == KMessageBox::ButtonCode::PrimaryAction);
}

bool ImportExportProgressIndicatorGui::overwriteDirectoryMessageBox(const QString &directory) const
{
    if (PimDataExportGlobalConfig::self()->alwaysOverrideDirectory()) {
        return true;
    }
    const int answer = KMessageBox::warningTwoActions(mParentWidget,
                                                      i18n("Directory \"%1\" already exists. Do you want to overwrite it?", directory),
                                                      i18nc("@title:window", "Restore"),
                                                      KStandardGuiItem::overwrite(),
                                                      KStandardGuiItem::cancel());
    return (answer == KMessageBox::ButtonCode::PrimaryAction);
}

void ImportExportProgressIndicatorGui::showErrorMessage(const QString &message, const QString &title)
{
    KMessageBox::error(mParentWidget, message, title);
}

#include "moc_importexportprogressindicatorgui.cpp"
