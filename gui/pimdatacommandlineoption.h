/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QCommandLineParser>
#include <QObject>

class QApplication;
class PimDataExporterWindow;
class PimDataCommandLineOption : public QObject
{
    Q_OBJECT
public:
    explicit PimDataCommandLineOption(QObject *parent = nullptr);
    ~PimDataCommandLineOption() override;

    void createParser(const QApplication &app);
    void setExportWindow(PimDataExporterWindow *exporterWindow);
    void handleCommandLine();

    [[nodiscard]] bool parseUserFeedback() const;

public Q_SLOTS:
    void slotActivateRequested(const QStringList &arguments, const QString &workingDirectory);

private:
    QCommandLineParser mParser;
    PimDataExporterWindow *mExporterWindow = nullptr;
};
