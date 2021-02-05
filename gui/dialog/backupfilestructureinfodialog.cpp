/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "backupfilestructureinfodialog.h"

#include <KPIMTextEdit/PlainTextEditorWidget>
#include <KPIMTextEdit/PlainTextEditor>

#include <KLocalizedString>
#include <KSharedConfig>

#include <KMessageBox>

#include <QFile>
#include <QLabel>
#include <QVBoxLayout>
#include <QStandardPaths>
#include <QDialogButtonBox>
#include <KConfigGroup>
#include <QPushButton>

BackupFileStructureInfoDialog::BackupFileStructureInfoDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Archive File Structure"));
    auto mainLayout = new QVBoxLayout(this);
    QLabel *lab = new QLabel(i18n("Backup Archive Structure:"));
    mainLayout->addWidget(lab);
    mEditor = new KPIMTextEdit::PlainTextEditorWidget;
    mEditor->editor()->setWebShortcutSupport(false);
    mEditor->setReadOnly(true);
    mainLayout->addWidget(mEditor);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &BackupFileStructureInfoDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &BackupFileStructureInfoDialog::reject);
    buttonBox->button(QDialogButtonBox::Close)->setDefault(true);
    setModal(true);
    mainLayout->addWidget(buttonBox);

    loadStructure();
    readConfig();
}

BackupFileStructureInfoDialog::~BackupFileStructureInfoDialog()
{
    writeConfig();
}

void BackupFileStructureInfoDialog::loadStructure()
{
    QFile f(QStringLiteral(":/structure/backup-structure.txt"));
    if (!f.open(QIODevice::ReadOnly)) {
        KMessageBox::error(this, i18n("backup-structure.txt file was not found."));
        return;
    }
    mEditor->setPlainText(QString::fromLatin1(f.readAll()));
}

void BackupFileStructureInfoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "BackupFileStructureInfoDialog");
    group.writeEntry("Size", size());
}

void BackupFileStructureInfoDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "BackupFileStructureInfoDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(600, 400));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}
