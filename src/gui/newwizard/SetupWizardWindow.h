#pragma once

#include <QDialog>
#include <QList>
#include <QPair>
#include <QStackedLayout>
#include <QStackedWidget>

#include "3rdparty/QProgressIndicator/QProgressIndicator.h"
#include "Pagination.h"
#include "SetupWizardAccountBuilder.h"
#include "pages/AbstractSetupWizardPage.h"

namespace Ui {
class SetupWizardWindow;
}

namespace OCC::Wizard {
/**
     * This class contains the UI-specific code. It hides the complexity from the controller, and provides a high-level API.
     */
class SetupWizardWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SetupWizardWindow(QWidget *parent);

    /**
         * Set entries in the pagination at the bottom of the wizard UI.
         * The entries are identified by their position in the list (read: index).
         */
    void setPaginationEntries(const QStringList &paginationEntries);

    /**
         * Render this page within the wizard window.
         * @param page page to render
         * @param index index to highlight in pagination (also used to decide which buttons to enable)
         */
    void displayPage(AbstractSetupWizardPage *page, PageIndex index);

    void showErrorMessage(const QString &errorMessage);

private Q_SLOTS:
    void slotReplaceContent(QWidget *newWidget);
    void slotHideErrorMessageWidget();

    /**
         * Show "transition to next page" animation. Use displayPage(...) to end it.
         */
    void slotStartTransition();

Q_SIGNALS:
    void paginationEntryClicked(PageIndex currentPage, PageIndex clickedPageIndex);
    void nextButtonClicked(PageIndex currentPage);
    void backButtonClicked(PageIndex currentPage);

private:
    QSharedPointer<::Ui::SetupWizardWindow> _ui;
    QSharedPointer<QStackedLayout> _contentLayout;
    QSharedPointer<Pagination> _pagination;
    QSharedPointer<QProgressIndicator> _transitionProgressIndicator;
};
}
