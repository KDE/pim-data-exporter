/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PIMDATACOMMANDLINEOPTION_H
#define PIMDATACOMMANDLINEOPTION_H

#include <QObject>
#include <QCommandLineParser>

class QApplication;
class PimDataExporterWindow;
class PimDataCommandLineOption : public QObject
{
    Q_OBJECT
public:
    explicit PimDataCommandLineOption(QObject *parent = nullptr);
    ~PimDataCommandLineOption();

    void createParser(const QApplication &app);
    void setExportWindow(PimDataExporterWindow *exporterWindow);
    void handleCommandLine();

    Q_REQUIRED_RESULT bool parseUserFeedback() const;

public Q_SLOTS:
    void slotActivateRequested(const QStringList &arguments, const QString &workingDirectory);

private:
    QCommandLineParser mParser;
    PimDataExporterWindow *mExporterWindow = nullptr;
};

#endif // PIMDATACOMMANDLINEOPTION_H
