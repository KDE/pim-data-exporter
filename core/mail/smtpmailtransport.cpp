/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "smtpmailtransport.h"
#include "pimdataexportcore_debug.h"
#include <MailTransport/TransportManager>

SmtpMailTransport::SmtpMailTransport(const KConfigGroup &group)
{
    setName(group.readEntry(QStringLiteral("name")));
    const QString hostStr(QStringLiteral("host"));
    if (group.hasKey(hostStr)) {
        setHost(group.readEntry(hostStr));
    }
    const QString portStr(QStringLiteral("port"));
    if (group.hasKey(portStr)) {
        setPort(group.readEntry(portStr, -1));
    }
    const QString userNameStr(QStringLiteral("user"));
    if (group.hasKey(userNameStr)) {
        setUserName(group.readEntry(userNameStr));
    }
    const QString precommandStr(QStringLiteral("precommand"));
    if (group.hasKey(precommandStr)) {
        setPrecommand(group.readEntry(precommandStr));
    }
    const QString requiresAuthenticationStr(QStringLiteral("auth"));
    if (group.hasKey(requiresAuthenticationStr)) {
        setRequiresAuthentication(group.readEntry(requiresAuthenticationStr, false));
    }
    const QString specifyHostnameStr(QStringLiteral("specifyHostname"));
    if (group.hasKey(specifyHostnameStr)) {
        setSpecifyHostname(group.readEntry(specifyHostnameStr, false));
    }
    const QString localHostnameStr(QStringLiteral("localHostname"));
    if (group.hasKey(localHostnameStr)) {
        setLocalHostname(group.readEntry(localHostnameStr));
    }
    const QString specifySenderOverwriteAddressStr(QStringLiteral("specifySenderOverwriteAddress"));
    if (group.hasKey(specifySenderOverwriteAddressStr)) {
        setSpecifySenderOverwriteAddress(group.readEntry(specifySenderOverwriteAddressStr, false));
    }
    const QString storePasswordStr(QStringLiteral("storepass"));
    if (group.hasKey(storePasswordStr)) {
        setStorePassword(group.readEntry(storePasswordStr, false));
    }
    const QString senderOverwriteAddressStr(QStringLiteral("senderOverwriteAddress"));
    if (group.hasKey(senderOverwriteAddressStr)) {
        setSenderOverwriteAddress(group.readEntry(senderOverwriteAddressStr));
    }
    const QString encryptionStr(QStringLiteral("encryption"));
    if (group.hasKey(encryptionStr)) {
        const QString encryptionType = group.readEntry(encryptionStr, QString());
        if (!encryptionType.isEmpty()) {
            if (encryptionType == QLatin1StringView("TLS")) {
                setEncryption(static_cast<int>(MailTransport::TransportBase::EnumEncryption::TLS));
            } else if (encryptionType == QLatin1StringView("SSL")) {
                setEncryption(static_cast<int>(MailTransport::TransportBase::EnumEncryption::SSL));
            } else if (encryptionType == QLatin1StringView("None")) {
                setEncryption(static_cast<int>(MailTransport::TransportBase::EnumEncryption::None));
            } else {
                qCWarning(PIMDATAEXPORTERCORE_LOG) << "Unknown encryption type " << encryptionType;
            }
        } else {
            qCWarning(PIMDATAEXPORTERCORE_LOG) << "Encryption type is empty. It's a bug";
        }
        setEncryption(group.readEntry(encryptionStr, 1)); // TODO verify
    }
    const QString authenticationTypeStr(QStringLiteral("authtype"));
    if (group.hasKey(authenticationTypeStr)) {
        setAuthenticationType(group.readEntry(authenticationTypeStr, 1)); // TODO verify
    }
}

QString SmtpMailTransport::name() const
{
    return mName;
}

void SmtpMailTransport::setName(const QString &name)
{
    mName = name;
}

QString SmtpMailTransport::host() const
{
    return mHost;
}

void SmtpMailTransport::setHost(const QString &host)
{
    mHost = host;
}

int SmtpMailTransport::port() const
{
    return mPort;
}

void SmtpMailTransport::setPort(int port)
{
    mPort = port;
}

QString SmtpMailTransport::userName() const
{
    return mUserName;
}

void SmtpMailTransport::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString SmtpMailTransport::precommand() const
{
    return mPrecommand;
}

void SmtpMailTransport::setPrecommand(const QString &precommand)
{
    mPrecommand = precommand;
}

bool SmtpMailTransport::requiresAuthentication() const
{
    return mRequiresAuthentication;
}

void SmtpMailTransport::setRequiresAuthentication(bool requiresAuthentication)
{
    mRequiresAuthentication = requiresAuthentication;
}

bool SmtpMailTransport::specifyHostname() const
{
    return mSpecifyHostname;
}

void SmtpMailTransport::setSpecifyHostname(bool specifyHostname)
{
    mSpecifyHostname = specifyHostname;
}

QString SmtpMailTransport::localHostname() const
{
    return mLocalHostname;
}

void SmtpMailTransport::setLocalHostname(const QString &localHostname)
{
    mLocalHostname = localHostname;
}

bool SmtpMailTransport::specifySenderOverwriteAddress() const
{
    return mSpecifySenderOverwriteAddress;
}

void SmtpMailTransport::setSpecifySenderOverwriteAddress(bool specifySenderOverwriteAddress)
{
    mSpecifySenderOverwriteAddress = specifySenderOverwriteAddress;
}

bool SmtpMailTransport::storePassword() const
{
    return mStorePassword;
}

void SmtpMailTransport::setStorePassword(bool storePassword)
{
    mStorePassword = storePassword;
}

QString SmtpMailTransport::senderOverwriteAddress() const
{
    return mSenderOverwriteAddress;
}

void SmtpMailTransport::setSenderOverwriteAddress(const QString &senderOverwriteAddress)
{
    mSenderOverwriteAddress = senderOverwriteAddress;
}

int SmtpMailTransport::encryption() const
{
    return mEncryption;
}

void SmtpMailTransport::setEncryption(int encryption)
{
    mEncryption = encryption;
}

int SmtpMailTransport::authenticationType() const
{
    return mAuthenticationType;
}

void SmtpMailTransport::setAuthenticationType(int authenticationType)
{
    mAuthenticationType = authenticationType;
}
