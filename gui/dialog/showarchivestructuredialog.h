/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SHOWARCHIVESTRUCTUREDIALOG_H
#define SHOWARCHIVESTRUCTUREDIALOG_H

#include <QDialog>
class QTreeWidget;
class QTreeWidgetItem;
class KArchiveEntry;
class KArchiveDirectory;
class QPushButton;
class KZip;
class QTemporaryDir;
class ShowArchiveStructureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShowArchiveStructureDialog(const QString &filename, QWidget *parent = nullptr);
    ~ShowArchiveStructureDialog();

    enum File {
        FullPath = Qt::UserRole + 1
    };

private:
    void slotExportAsLogFile();
    Q_REQUIRED_RESULT bool fillTree();
    void readConfig();
    void writeConfig();
    void slotItemClicked(QTreeWidgetItem *item, int column);
    void addSubItems(const QString &topLevelPath, QTreeWidgetItem *parent, const KArchiveEntry *entry, int indent, const QString &fullpath = QString());
    void slotExtractFile();
    void slotOpenFile();

    Q_REQUIRED_RESULT bool searchArchiveElement(const QString &path, const KArchiveDirectory *topDirectory, const QString &name);
    QTreeWidgetItem *addTopItem(const QString &name);
    QTreeWidgetItem *addItem(QTreeWidgetItem *parent, const QString &name, const QString &fillFullPath);
    QString mFileName;
    QString mLogFile;
    QTreeWidget *mTreeWidget = nullptr;
    QPushButton *mExtractFile = nullptr;
    QPushButton *mOpenFile = nullptr;
    KZip *mZip = nullptr;
    QTemporaryDir *mTempDir = nullptr;
};

#endif // SHOWARCHIVESTRUCTUREDIALOG_H
