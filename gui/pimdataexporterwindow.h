/*
   SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "utils.h"
#include <KXmlGuiWindow>
class LogWidget;
class KRecentFilesMenu;
class QAction;
class QCommandLineParser;
class PimDataBackupRestoreUI;
class PimDataTrayIcon;
class PimDataExporterWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit PimDataExporterWindow(QWidget *parent = nullptr);
    ~PimDataExporterWindow() override;
    void handleCommandLine(const QCommandLineParser &parser);

private:
    void slotBackupData();
    void slotRestoreData();
    void slotAddInfo(const QString &info);
    void slotAddError(const QString &info);
    void slotAddTitle(const QString &info);
    void slotAddEndLine();
    void slotSaveLog();
    void slotShowStructureInfos();
    void slotRestoreFile(const QUrl &url);
    void slotShowArchiveInformations();
    void slotUpdateActions(bool inAction);
    void slotShowBackupFinishDialogInformation();
    void slotJobFailed();
    void slotJobFinished();
    void slotShowCurrentArchiveInformations();
    void slotAddResourceToSync(const QString &name, const QString &identifier);
    void slotFullSyncFinished();
    void slotFullSyncInstanceDone(const QString &identifier);
    void slotFullSyncInstanceFailed(const QString &identifier);
    void slotRestoreDone();
    void slotConfigure();

private:
    void initializeBackupRestoreUi();
    void backupData(const QString &filename = QString(), const QString &templateFile = QString());
    void loadData(const QString &filename = QString(), const QString &templateFile = QString());
    void setupActions(bool canZipFile);
    void showFinishInformation();
    QString mLastArchiveFileName;
    // Name, identifier
    QHash<QString, QString> mNeedToSyncResources;
    LogWidget *mLogWidget = nullptr;
    KRecentFilesMenu *mRecentFilesMenu = nullptr;
    QAction *mBackupAction = nullptr;
    QAction *mRestoreAction = nullptr;
    QAction *mSaveLogAction = nullptr;
    QAction *mArchiveStructureInfo = nullptr;
    QAction *mShowArchiveInformationsAction = nullptr;
    QAction *mShowArchiveInformationsAboutCurrentArchiveAction = nullptr;
    PimDataBackupRestoreUI *mPimDataBackupRestoreUI = nullptr;
    PimDataTrayIcon *mTrayIcon = nullptr;
};
