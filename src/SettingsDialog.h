// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2023, Lev Leontev

#pragma once

#include <QDialog>
#include <memory>

#include "AuthenticateWithTwitchDialog.h"
#include "RewardsTheaterPlugin.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    SettingsDialog(RewardsTheaterPlugin& plugin, QWidget* parent);
    ~SettingsDialog();

private slots:
    void logInOrLogOut();
    void openRewardsQueue();
    void updateAuthButtonText(const std::optional<std::string>& username);

private:
    RewardsTheaterPlugin& plugin;
    std::unique_ptr<Ui::SettingsDialog> ui;
    AuthenticateWithTwitchDialog* authenticateWithTwitchDialog;
};
