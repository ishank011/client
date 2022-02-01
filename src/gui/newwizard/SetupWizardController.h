#pragma once

#include "pages/AbstractSetupWizardPage.h"
#include <QDialog>
#include <QSharedPointer>
#include <SetupWizardAccountBuilder.h>
#include <SetupWizardWindow.h>
#include <account.h>

namespace OCC::Wizard {
/**
     * This class is the backbone of the new setup wizard. It instantiates the required UI elements and fills them with the correct data. It also provides the public API for the settings UI.
     *
     * The new setup wizard uses dependency injection where applicable. The account object is created using the builder pattern.
     */
class SetupWizardController : public QObject
{
    Q_OBJECT

public:
    explicit SetupWizardController(QWidget *parent);

    /**
         * Show wizard UI. Can be used to show the wizard after initializing the controller.
         */
    void showUi();

Q_SIGNALS:
    /**
         * Emitted when the wizard has finished. It passes the built account object.
         */
    void finished(AccountPtr newAccount);

private:
    void chooseAndShowPage(std::optional<PageIndex> currentPage, std::optional<PageIndex> desiredPage);

    // using a shared pointer saves us from deleting the object in the destructor
    QSharedPointer<SetupWizardWindow> _wizardWindow;

    // keeping a pointer on the current page allows us to check whether the controller has been initialized yet
    // the pointer is also used to clean up the page
    QPointer<AbstractSetupWizardPage> _currentPage;

    SetupWizardAccountBuilder _accountBuilder;
};
}
