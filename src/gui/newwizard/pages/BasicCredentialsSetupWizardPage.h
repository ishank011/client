#pragma once

#include <QSharedPointer>

#include "AbstractSetupWizardPage.h"

namespace Ui {
class BasicCredentialsSetupWizardPage;
}

namespace OCC::Wizard {

class BasicCredentialsSetupWizardPage : public AbstractSetupWizardPage
{
    Q_OBJECT

public:
    BasicCredentialsSetupWizardPage(const QUrl &serverUrl, const QString &username, const QString &password);

    QString username();
    QString password();

private:
    QSharedPointer<::Ui::BasicCredentialsSetupWizardPage> _ui;
};

}
