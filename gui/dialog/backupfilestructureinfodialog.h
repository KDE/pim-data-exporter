/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>
namespace KPIMTextEdit
{
class PlainTextEditorWidget;
}
class BackupFileStructureInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BackupFileStructureInfoDialog(QWidget *parent = nullptr);
    ~BackupFileStructureInfoDialog() override;

private:
    void readConfig();
    void writeConfig();
    void loadStructure();
    KPIMTextEdit::PlainTextEditorWidget *const mEditor;
};
