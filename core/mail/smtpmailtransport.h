/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] QString name() const;
    void setName(const QString &name);

    [[nodiscard]] QString host() const;
    void setHost(const QString &host);

    [[nodiscard]] int port() const;
    void setPort(int port);

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &userName);

    [[nodiscard]] QString precommand() const;
    void setPrecommand(const QString &precommand);

    [[nodiscard]] bool requiresAuthentication() const;
    void setRequiresAuthentication(bool requiresAuthentication);

    [[nodiscard]] bool specifyHostname() const;
    void setSpecifyHostname(bool specifyHostname);

    [[nodiscard]] QString localHostname() const;
    void setLocalHostname(const QString &localHostname);

    [[nodiscard]] bool specifySenderOverwriteAddress() const;
    void setSpecifySenderOverwriteAddress(bool specifySenderOverwriteAddress);

    [[nodiscard]] bool storePassword() const;
    void setStorePassword(bool storePassword);

    [[nodiscard]] QString senderOverwriteAddress() const;
    void setSenderOverwriteAddress(const QString &senderOverwriteAddress);

    [[nodiscard]] int encryption() const;
    void setEncryption(int encryption);

    [[nodiscard]] int authenticationType() const;
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
