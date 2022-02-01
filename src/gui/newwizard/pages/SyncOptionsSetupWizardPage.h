#pragma once

#include <QSharedPointer>

#include "AbstractSetupWizardPage.h"

namespace Ui {
class SyncOptionsSetupWizardPage;
}

namespace OCC::Wizard {

class SyncOptionsSetupWizardPage : public AbstractSetupWizardPage
{
    Q_OBJECT

public:
    SyncOptionsSetupWizardPage();

private:
    QSharedPointer<::Ui::SyncOptionsSetupWizardPage> _ui;
};

}
