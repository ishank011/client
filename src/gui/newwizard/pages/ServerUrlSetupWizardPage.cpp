#include "ServerUrlSetupWizardPage.h"

#include "theme.h"
#include "ui_ServerUrlSetupWizardPage.h"

namespace OCC::Wizard {

ServerUrlSetupWizardPage::ServerUrlSetupWizardPage(const QUrl &serverUrl)
    : _ui(new ::Ui::ServerUrlSetupWizardPage)
{
    _ui->setupUi(this);

    _ui->welcomeTextLabel->setText(tr("Welcome to %1").arg(Theme::instance()->appNameGUI()));

    _ui->urlLineEdit->setText(serverUrl.toString());
}

QUrl ServerUrlSetupWizardPage::serverUrl()
{
    return QUrl(_ui->urlLineEdit->text());
}

}
