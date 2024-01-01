/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>
namespace TextCustomEditor
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
    TextCustomEditor::PlainTextEditorWidget *const mEditor;
};
