#include <QMessageBox>

#include "SyncOptionsSetupWizardPage.h"

#include "theme.h"
#include "ui_SyncOptionsSetupWizardPage.h"

namespace OCC::Wizard {

SyncOptionsSetupWizardPage::SyncOptionsSetupWizardPage()
    : _ui(new ::Ui::SyncOptionsSetupWizardPage)
{
    _ui->setupUi(this);

    _ui->syncEverythingRadioButton->setChecked(true);
    _ui->chooseWhatToSyncPushButton->setEnabled(false);

    connect(_ui->chooseWhatToSyncRadioButton, &QRadioButton::toggled, _ui->chooseWhatToSyncPushButton, &QPushButton::setEnabled);

    connect(_ui->chooseWhatToSyncPushButton, &QPushButton::pressed, this, [this]() {
        QMessageBox::information(this, "Choose what to sync", "Choose what to sync");
    });

    connect(_ui->localDirectoryPushButton, &QPushButton::pressed, this, [this]() {
        QMessageBox::information(this, "Choose local directory", "Choose local directory");
    });
}

}
