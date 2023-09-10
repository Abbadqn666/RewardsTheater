// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2023, Lev Leontev

#pragma once

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4702)
#endif
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <QObject>
#include <chrono>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <thread>

#include "Settings.h"

/**
 * A class for Twitch authentication using the Implicit grant flow.
 * Read more here: https://dev.twitch.tv/docs/authentication/getting-tokens-oauth/#implicit-grant-flow
 */
class TwitchAuth : public QObject {
    Q_OBJECT

public:
    TwitchAuth(
        Settings& settings,
        const std::string& clientId,
        const std::set<std::string>& scopes,
        std::uint16_t authServerPort
    );
    ~TwitchAuth();

    std::optional<std::string> getAccessToken() const;
    bool isAuthenticated() const;
    const std::string& getClientId() const;

    void authenticate();
    void authenticateWithToken(const std::string& token);
    boost::asio::awaitable<void> asyncAuthenticateWithToken(std::string token, boost::asio::io_context& ioContext);
    void logOut();

    enum class AuthenticationFailureReason {
        AUTH_TOKEN_INVALID,
        NETWORK_ERROR,
    };

signals:
    void onAuthenticationSuccess();
    void onAuthenticationFailure(AuthenticationFailureReason reason);
    void onUsernameChanged(std::optional<std::string> username);

private:
    boost::asio::awaitable<std::chrono::seconds>
    asyncTokenExpiresIn(std::string token, boost::asio::io_context& ioContext);

    bool tokenHasNeededScopes(const boost::property_tree::ptree& oauthValidateResponse);
    boost::asio::awaitable<std::optional<std::string>> asyncGetUsername(boost::asio::io_context& ioContext);
    void startAuthServer(std::uint16_t port);
    std::string getAuthUrl();

    std::optional<std::string> accessToken;
    mutable std::mutex accessTokenMutex;

    std::thread authServerThread;
    boost::asio::io_context authServerIoContext;

    Settings& settings;
    std::string clientId;
    std::set<std::string> scopes;
    std::uint16_t authServerPort;
};
