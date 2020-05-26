/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#ifndef SMTPMAILTRANSPORT_H
#define SMTPMAILTRANSPORT_H
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

#endif // SMTPMAILTRANSPORT_H
