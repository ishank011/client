#include "SetupWizardController.h"
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

    chooseAndShowPage(std::nullopt, std::nullopt);

    // allow settings dialog to clean up the wizard controller and all the objects it created
    connect(_wizardWindow.get(), &SetupWizardWindow::rejected, this, [this]() {
        Q_EMIT finished(nullptr);
    });
    //
    //    connect(_wizardWindow.get(), &SetupWizardWindow::nextButtonClicked, this, [this]() {
    //        // FIXME: remove timer (only there for debugging)
    //        QTimer::singleShot(800ms, [this]() {
    //            auto* page = new ServerUrlSetupWizardPage;
    //            _wizardWindow->displayPage(page, 1);
    //        });
    //    });

    connect(_wizardWindow.get(), &SetupWizardWindow::paginationEntryClicked, this, [this, paginationEntries](PageIndex currentPage, PageIndex clickedPageIndex) {
        Q_ASSERT(currentPage < paginationEntries.size());

        // FIXME: just here for debugging purposes
        QTimer::singleShot(800ms, [=]() {
            chooseAndShowPage(currentPage, clickedPageIndex);
        });
    });
    connect(_wizardWindow.get(), &SetupWizardWindow::nextButtonClicked, this, [this, paginationEntries](PageIndex currentPage) {
        Q_ASSERT(currentPage < paginationEntries.size());

        if (currentPage == (paginationEntries.size() - 1)) {
            emit finished(_accountBuilder.build());
        }

        // FIXME: just here for debugging purposes
        QTimer::singleShot(800ms, [=]() {
            chooseAndShowPage(currentPage, std::nullopt);
        });
    });

    // in case the back button is clicked, the current page's data is dismissed, and the previous page should be shown
    connect(_wizardWindow.get(), &SetupWizardWindow::backButtonClicked, this, [this](PageIndex currentPage) {
        // back button should be disabled on the first page
        Q_ASSERT(currentPage > 0);

        // FIXME: just here for debugging purposes
        QTimer::singleShot(800ms, [=]() {
            chooseAndShowPage(currentPage, currentPage - 1);
        });
    });
}

void SetupWizardController::showUi()
{
    _wizardWindow->show();
}

void SetupWizardController::chooseAndShowPage(std::optional<PageIndex> currentPage, std::optional<PageIndex> desiredPage)
{
    //    if (currentPage.has_value() && desiredPage.has_value()) {
    //        Q_ASSERT(currentPage != desiredPage);
    //    }

    //// clean up is done here to save us from connecting a lambda over and over again
    //auto cleanUpPage = [this]() {
    //    _currentPage->deleteLater();
    //};

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
    }

    if (desiredPage == 0) {
        _currentPage = new ServerUrlSetupWizardPage(_accountBuilder.serverUrl());
        _wizardWindow->displayPage(_currentPage, 0);
        return;
    }

    Q_UNREACHABLE();
}

}
