#include "AccountConfiguredWizardPage.h"

#include "theme.h"
#include "ui_AccountConfiguredWizardPage.h"

namespace OCC::Wizard {

AccountConfiguredWizardPage::AccountConfiguredWizardPage()
    : _ui(new ::Ui::AccountConfiguredWizardPage)
{
    _ui->setupUi(this);

    _ui->configureSyncPushButton->hide();

    connect(_ui->groupBox, &QGroupBox::toggled, this, [this](bool enabled) {
        _ui->groupBoxContentWidget->setVisible(enabled);
        _ui->groupBox->setFlat(!enabled);
    });

    // toggle once
    _ui->groupBox->setChecked(true);
    _ui->groupBox->setChecked(false);
}

}
