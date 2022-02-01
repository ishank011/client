#pragma once

#include <QWidget>

namespace OCC::Wizard {

class AbstractSetupWizardPage : public QWidget
{
    Q_OBJECT

public:
    //        AbstractSetupWizardPage();

    virtual ~AbstractSetupWizardPage();

Q_SIGNALS:
    void commit();
};

}
