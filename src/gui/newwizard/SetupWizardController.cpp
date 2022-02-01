#include "SetupWizardController.h"
#include "pages/AccountConfiguredWizardPage.h"
#include "pages/BasicCredentialsSetupWizardPage.h"
#include "pages/ServerUrlSetupWizardPage.h"

#include <QTimer>

#include <chrono>

using namespace std::chrono_literals;

namespace OCC::Wizard {

SetupWizardController::SetupWizardController(QWidget *parent)
    : QObject(parent)
    , _wizardWindow(new SetupWizardWindow(parent))
{
    // initialize pagination
    const auto paginationEntries = QStringList() << "Server URL"
                                                 << "Credentials"
                                                 << "Sync Options";
    _wizardWindow->setPaginationEntries(paginationEntries);

    nextStep(std::nullopt, std::nullopt);

    // allow settings dialog to clean up the wizard controller and all the objects it created
    connect(_wizardWindow.get(), &SetupWizardWindow::rejected, this, [this]() {
        Q_EMIT finished(nullptr);
    });

    connect(_wizardWindow.get(), &SetupWizardWindow::paginationEntryClicked, this, [this, paginationEntries](PageIndex currentPage, PageIndex clickedPageIndex) {
        Q_ASSERT(currentPage < paginationEntries.size());

        // FIXME: just here for debugging purposes
        QTimer::singleShot(800ms, [=]() {
            nextStep(currentPage, clickedPageIndex);
        });
    });
    connect(_wizardWindow.get(), &SetupWizardWindow::nextButtonClicked, this, [this, paginationEntries](PageIndex currentPage) {
        Q_ASSERT(currentPage < paginationEntries.size());

        if (currentPage == (paginationEntries.size() - 1)) {
            emit finished(_accountBuilder.build());
        }

        // FIXME: just here for debugging purposes
        QTimer::singleShot(800ms, [=]() {
            nextStep(currentPage, std::nullopt);
        });
    });

    // in case the back button is clicked, the current page's data is dismissed, and the previous page should be shown
    connect(_wizardWindow.get(), &SetupWizardWindow::backButtonClicked, this, [this](PageIndex currentPage) {
        // back button should be disabled on the first page
        Q_ASSERT(currentPage > 0);

        // FIXME: just here for debugging purposes
        QTimer::singleShot(800ms, [=]() {
            nextStep(currentPage, currentPage - 1);
        });
    });
}

void SetupWizardController::showUi()
{
    _wizardWindow->show();
}

void SetupWizardController::nextStep(std::optional<PageIndex> currentPage, std::optional<PageIndex> desiredPage)
{
    // should take care of cleaning up the page once the function has finished
    QScopedPointer<AbstractSetupWizardPage> page(_currentPage);

    // initial state
    if (!currentPage.has_value()) {
        desiredPage = 0;
    }

    // "next button" workflow
    if (!desiredPage.has_value()) {
        // try to fill in data appropriately
        // if it works, go to next page
        // otherwise, show current page again
        if (currentPage == 0) {
            auto *pagePtr = qobject_cast<ServerUrlSetupWizardPage *>(_currentPage);

            if (_accountBuilder.setServerUrl(pagePtr->serverUrl())) {
                desiredPage = currentPage.value() + 1;
            } else {
                _wizardWindow->showErrorMessage("Invalid server URL");
                desiredPage = currentPage.value();
            }
        }

        if (currentPage == 1) {
            if (_accountBuilder.workflowType() == SetupWizardAccountBuilder::HTTP_BASIC_WORKFLOW) {
                auto *pagePtr = qobject_cast<BasicCredentialsSetupWizardPage *>(_currentPage);
                if (_accountBuilder.setBasicCredentials(pagePtr->username(), pagePtr->password())) {
                    desiredPage = currentPage.value() + 1;
                } else {
                    _wizardWindow->showErrorMessage("Invalid credentials");
                    desiredPage = currentPage.value();
                }
            }
        }

        // final step
        if (currentPage == 2) {
            auto account = _accountBuilder.build();
            Q_ASSERT(account != nullptr);
            emit finished(account);
        }
    }

    if (desiredPage == 0) {
        _currentPage = new ServerUrlSetupWizardPage(_accountBuilder.serverUrl());
        _wizardWindow->displayPage(_currentPage, 0);
        return;
    }

    if (desiredPage == 1) {
        if (_accountBuilder.workflowType() == SetupWizardAccountBuilder::HTTP_BASIC_WORKFLOW) {
            _currentPage = new BasicCredentialsSetupWizardPage(_accountBuilder.serverUrl(), _accountBuilder.username(), _accountBuilder.password());
            _wizardWindow->displayPage(_currentPage, 1);
            return;
        }
    }

    if (desiredPage == 2) {
        _currentPage = new AccountConfiguredWizardPage;
        _wizardWindow->displayPage(_currentPage, 2);
        return;
    }

    Q_UNREACHABLE();
}

}
