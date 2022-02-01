#include "SetupWizardWindow.h"

#include <QDebug>
#include <QLabel>

#include "Pagination.h"
#include "ui_SetupWizardWindow.h"

namespace OCC::Wizard {

SetupWizardWindow::SetupWizardWindow(QWidget *parent)
    : QDialog(parent)
    , _ui(new ::Ui::SetupWizardWindow)
    //    , _transitionLayout(new QHBoxLayout)
    , _transitionProgressIndicator(new QProgressIndicator)
{
    _ui->setupUi(this);

    // make sure to initialize these after UI has been set up
    _pagination.reset(new Pagination(_ui->paginationLayout));

    slotHideErrorMessageWidget();

    _contentLayout.reset(new QStackedLayout(_ui->contentWidget));
    _contentLayout->setAlignment(Qt::AlignCenter);

    connect(_ui->cancelButton, &QPushButton::clicked, this, &SetupWizardWindow::reject);

    // set up transition
    //    _transitionLayout->addWidget(_transitionProgressIndicator.get());

    connect(_ui->nextButton, &QPushButton::clicked, this, [this]() {
        slotStartTransition();
        emit nextButtonClicked(_pagination->activePageIndex());
    });

    connect(_ui->backButton, &QPushButton::clicked, this, [this]() {
        slotStartTransition();
        emit backButtonClicked(_pagination->activePageIndex());
    });

    connect(_pagination.get(), &Pagination::paginationEntryClicked, this, [this](PageIndex clickedPageIndex) {
        slotStartTransition();
        emit paginationEntryClicked(_pagination->activePageIndex(), clickedPageIndex);
    });

    _transitionProgressIndicator->setFixedSize(32, 32);
}

void SetupWizardWindow::displayPage(AbstractSetupWizardPage *page, PageIndex index)
{
    // save some CPU cycles
    _transitionProgressIndicator->stopAnimation();

    _ui->backButton->setEnabled(true);
    _ui->nextButton->setEnabled(true);

    if (index == 0) {
        _ui->backButton->setEnabled(false);
    } else if (index == _pagination->size()) {
        _ui->nextButton->setEnabled(false);
    }

    slotReplaceContent(page);

    _pagination->setActivePageIndex(index);
    _pagination->setEnabled(true);

    connect(_ui->errorMessageDismissButton, &QPushButton::clicked, this, &SetupWizardWindow::slotHideErrorMessageWidget);

    // _ui->contentWidget->setLayout(page->layout());
}

void SetupWizardWindow::slotStartTransition()
{
    _transitionProgressIndicator->startAnimation();
    slotReplaceContent(_transitionProgressIndicator.get());

    // until a new page is displayed by the controller, we want to prevent the user from initiating another page change
    _ui->backButton->setEnabled(false);
    _ui->nextButton->setEnabled(false);
    _pagination->setEnabled(false);
    // also, we should assume the user has seen the error message in case one is shown
    slotHideErrorMessageWidget();
}

void SetupWizardWindow::slotReplaceContent(QWidget *newWidget)
{
    // there should not be more than one widget
    for (decltype(_contentLayout->count()) i = 0; i < _contentLayout->count(); ++i) {
        _contentLayout->removeItem(_contentLayout->itemAt(i));
    }

    _contentLayout->addWidget(newWidget);
}

void SetupWizardWindow::slotHideErrorMessageWidget()
{
    _ui->errorMessageWidget->setMaximumSize(16777215, 0);
}

void SetupWizardWindow::showErrorMessage(const QString &errorMessage)
{
    _ui->errorMessageLabel->setText(errorMessage);
    _ui->errorMessageWidget->setMaximumSize(16777215, 16777215);
}

void SetupWizardWindow::setPaginationEntries(const QStringList &paginationEntries)
{
    _pagination->setEntries(paginationEntries);
}

}
