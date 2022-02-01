#include "BasicCredentialsSetupWizardPage.h"

#include "theme.h"
#include "ui_BasicCredentialsSetupWizardPage.h"

namespace OCC::Wizard {

BasicCredentialsSetupWizardPage::BasicCredentialsSetupWizardPage(const QUrl &serverUrl, const QString &username)
    : _ui(new ::Ui::BasicCredentialsSetupWizardPage)
{
    _ui->setupUi(this);

    _ui->urlLabel->setText(serverUrl.toString());
    _ui->usernameLineEdit->setText(username);
}

QString BasicCredentialsSetupWizardPage::username()
{
    return _ui->usernameLineEdit->text();
}

QString BasicCredentialsSetupWizardPage::password()
{
    return _ui->passwordLineEdit->text();
}

}
