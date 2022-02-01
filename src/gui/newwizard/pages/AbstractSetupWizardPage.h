#pragma once

#include <QWidget>

namespace OCC::Wizard {

class AbstractSetupWizardPage : public QWidget
{
    Q_OBJECT

public:
    virtual ~AbstractSetupWizardPage();
};

}
