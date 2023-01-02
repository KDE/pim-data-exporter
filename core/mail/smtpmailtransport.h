/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "pimdataexportercore_private_export.h"
#include <KConfigGroup>
#include <QString>

class PIMDATAEXPORTER_TESTS_EXPORT SmtpMailTransport
{
public:
    explicit SmtpMailTransport(const KConfigGroup &group);
    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QString host() const;
    void setHost(const QString &host);

    Q_REQUIRED_RESULT int port() const;
    void setPort(int port);

    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &userName);

    Q_REQUIRED_RESULT QString precommand() const;
    void setPrecommand(const QString &precommand);

    Q_REQUIRED_RESULT bool requiresAuthentication() const;
    void setRequiresAuthentication(bool requiresAuthentication);

    Q_REQUIRED_RESULT bool specifyHostname() const;
    void setSpecifyHostname(bool specifyHostname);

    Q_REQUIRED_RESULT QString localHostname() const;
    void setLocalHostname(const QString &localHostname);

    Q_REQUIRED_RESULT bool specifySenderOverwriteAddress() const;
    void setSpecifySenderOverwriteAddress(bool specifySenderOverwriteAddress);

    Q_REQUIRED_RESULT bool storePassword() const;
    void setStorePassword(bool storePassword);

    Q_REQUIRED_RESULT QString senderOverwriteAddress() const;
    void setSenderOverwriteAddress(const QString &senderOverwriteAddress);

    Q_REQUIRED_RESULT int encryption() const;
    void setEncryption(int encryption);

    Q_REQUIRED_RESULT int authenticationType() const;
    void setAuthenticationType(int authenticationType);

private:
    QString mName;
    QString mHost;
    QString mUserName;
    QString mPrecommand;
    QString mLocalHostname;
    QString mSenderOverwriteAddress;
    int mPort = -1;
    int mEncryption = -1;
    int mAuthenticationType = 1;
    bool mRequiresAuthentication = false;
    bool mSpecifyHostname = false;
    bool mSpecifySenderOverwriteAddress = false;
    bool mStorePassword = false;
};
