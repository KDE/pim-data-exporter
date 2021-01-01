/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef BACKUPFILESTRUCTUREINFODIALOG_H
#define BACKUPFILESTRUCTUREINFODIALOG_H

#include <QDialog>
namespace KPIMTextEdit {
class PlainTextEditorWidget;
}
class BackupFileStructureInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BackupFileStructureInfoDialog(QWidget *parent = nullptr);
    ~BackupFileStructureInfoDialog();

private:
    void readConfig();
    void writeConfig();
    void loadStructure();
    KPIMTextEdit::PlainTextEditorWidget *mEditor = nullptr;
};

#endif // BACKUPFILESTRUCTUREINFODIALOG_H
