#pragma once

#include <QSharedPointer>

#include "AbstractSetupWizardPage.h"

namespace Ui {
class AccountConfiguredWizardPage;
}

namespace OCC::Wizard {

class AccountConfiguredWizardPage : public AbstractSetupWizardPage
{
    Q_OBJECT

public:
    AccountConfiguredWizardPage();

private:
    QSharedPointer<::Ui::AccountConfiguredWizardPage> _ui;
};

}
