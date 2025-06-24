/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "smtpmailtransport.h"
using namespace Qt::Literals::StringLiterals;

#include "pimdataexportcore_debug.h"
#include <MailTransport/TransportManager>

SmtpMailTransport::SmtpMailTransport(const KConfigGroup &group)
{
    setName(group.readEntry(u"name"_s));
    const QString hostStr(u"host"_s);
    if (group.hasKey(hostStr)) {
        setHost(group.readEntry(hostStr));
    }
    const QString portStr(u"port"_s);
    if (group.hasKey(portStr)) {
        setPort(group.readEntry(portStr, -1));
    }
    const QString userNameStr(u"user"_s);
    if (group.hasKey(userNameStr)) {
        setUserName(group.readEntry(userNameStr));
    }
    const QString precommandStr(u"precommand"_s);
    if (group.hasKey(precommandStr)) {
        setPrecommand(group.readEntry(precommandStr));
    }
    const QString requiresAuthenticationStr(u"auth"_s);
    if (group.hasKey(requiresAuthenticationStr)) {
        setRequiresAuthentication(group.readEntry(requiresAuthenticationStr, false));
    }
    const QString specifyHostnameStr(u"specifyHostname"_s);
    if (group.hasKey(specifyHostnameStr)) {
        setSpecifyHostname(group.readEntry(specifyHostnameStr, false));
    }
    const QString localHostnameStr(u"localHostname"_s);
    if (group.hasKey(localHostnameStr)) {
        setLocalHostname(group.readEntry(localHostnameStr));
    }
    const QString specifySenderOverwriteAddressStr(u"specifySenderOverwriteAddress"_s);
    if (group.hasKey(specifySenderOverwriteAddressStr)) {
        setSpecifySenderOverwriteAddress(group.readEntry(specifySenderOverwriteAddressStr, false));
    }
    const QString storePasswordStr(u"storepass"_s);
    if (group.hasKey(storePasswordStr)) {
        setStorePassword(group.readEntry(storePasswordStr, false));
    }
    const QString senderOverwriteAddressStr(u"senderOverwriteAddress"_s);
    if (group.hasKey(senderOverwriteAddressStr)) {
        setSenderOverwriteAddress(group.readEntry(senderOverwriteAddressStr));
    }
    const QString encryptionStr(u"encryption"_s);
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
    const QString authenticationTypeStr(u"authtype"_s);
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
