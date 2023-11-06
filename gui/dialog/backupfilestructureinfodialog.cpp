/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "backupfilestructureinfodialog.h"

#include <TextCustomEditor/PlainTextEditor>
#include <TextCustomEditor/PlainTextEditorWidget>

#include <KLocalizedString>
#include <KSharedConfig>

#include <KMessageBox>

#include <KConfigGroup>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myBackupFileStructureInfoDialogGroupName[] = "BackupFileStructureInfoDialog";
}
BackupFileStructureInfoDialog::BackupFileStructureInfoDialog(QWidget *parent)
    : QDialog(parent)
    , mEditor(new TextCustomEditor::PlainTextEditorWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Archive File Structure"));
    auto mainLayout = new QVBoxLayout(this);
    auto lab = new QLabel(i18n("Backup Archive Structure:"));
    mainLayout->addWidget(lab);
    mEditor->editor()->setWebShortcutSupport(false);
    mEditor->setReadOnly(true);
    mainLayout->addWidget(mEditor);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
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

void BackupFileStructureInfoDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(600, 400));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myBackupFileStructureInfoDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void BackupFileStructureInfoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myBackupFileStructureInfoDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}

#include "moc_backupfilestructureinfodialog.cpp"
