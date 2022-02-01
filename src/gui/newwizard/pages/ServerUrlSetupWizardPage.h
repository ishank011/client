#pragma once

#include <QSharedPointer>

#include "AbstractSetupWizardPage.h"

namespace Ui {
class ServerUrlSetupWizardPage;
}

namespace OCC::Wizard {
class ServerUrlSetupWizardPage : public AbstractSetupWizardPage
{
    Q_OBJECT

public:
    ServerUrlSetupWizardPage(const QUrl &serverUrl);

    QUrl serverUrl();

private:
    QSharedPointer<::Ui::ServerUrlSetupWizardPage> _ui;
};
}
