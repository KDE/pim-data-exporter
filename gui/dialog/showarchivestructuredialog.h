/*
   Copyright (C) 2013-2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
    void exportAsLogFile();
    bool fillTree();
    void readConfig();
    void writeConfig();
    void slotItemClicked(QTreeWidgetItem *item, int column);
    void addSubItems(const QString &topLevelPath, QTreeWidgetItem *parent, const KArchiveEntry *entry, int indent, const QString &fullpath = QString());
    void slotExtractFile();

    bool searchArchiveElement(const QString &path, const KArchiveDirectory *topDirectory, const QString &name);
    QTreeWidgetItem *addTopItem(const QString &name);
    QTreeWidgetItem *addItem(QTreeWidgetItem *parent, const QString &name, const QString &fillFullPath);
    QString mFileName;
    QString mLogFile;
    QTreeWidget *mTreeWidget = nullptr;
    QPushButton *mExtractFile = nullptr;
    KZip *mZip = nullptr;
};

#endif // SHOWARCHIVESTRUCTUREDIALOG_H
