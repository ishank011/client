
#include <QDebug>
#include <QRadioButton>

#include "Pagination.h"

namespace OCC::Wizard {

//PaginationEntry::PaginationEntry(QString title, bool enabled)
//    : title(std::move(title))
//    , enabled(enabled)
//{
//}
//
//PaginationEntry &PaginationEntry::operator=(PaginationEntry &&) noexcept = default;
//PaginationEntry &PaginationEntry::operator=(const PaginationEntry &) noexcept = default;
//PaginationEntry::PaginationEntry(const PaginationEntry &) = default;
//PaginationEntry::PaginationEntry(PaginationEntry &&) noexcept = default;

Pagination::Pagination(QHBoxLayout *layout)
    : _layout(layout)
    , _entries()
    , _activePageIndex(0)
    , _enabled(true)
{
}

void Pagination::setEntries(const QStringList &newEntries)
{
    // TODO: more advanced implementation (reuse existing buttons within layout)
    // current active page is also lost that way
    removeAllItems();

    _entries = newEntries;

    for (PageIndex i = 0; i < newEntries.count(); ++i) {
        auto entry = newEntries[i];

        auto newButton = new QRadioButton(entry, nullptr);

        _layout->addWidget(newButton);

        connect(newButton, &QRadioButton::clicked, this, [this, i](bool checked) {
            // we don't care about the argument
            (void)checked;

            emit paginationEntryClicked(i);
        });
    }

    enableOrDisableButtons();
}

// needed to clean up widgets we added to the layout
Pagination::~Pagination() noexcept
{
    removeAllItems();
}

void Pagination::removeAllItems()
{
    for (decltype(_layout->count()) i = 0; i < _layout->count(); ++i) {
        auto *item = _layout->itemAt(i);
        _layout->removeItem(item);

        // clean up now, the pagination object will live much longer than its entries
        item->widget()->deleteLater();
    }
}

PageIndex Pagination::size() const
{
    return _entries.size();
}

void Pagination::setEnabled(bool enabled)
{
    _enabled = enabled;
    enableOrDisableButtons();
}

void Pagination::enableOrDisableButtons()
{
    for (PageIndex i = 0; i < _entries.count(); ++i) {
        const auto enabled = [=]() {
            if (_enabled) {
                return i < _activePageIndex;
            }

            return false;
        }();

        // TODO: use custom QRadioButton which doesn't need to be disabled to not be clickable
        // can only jump to pages we have visited before
        // to avoid resetting the current page, we don't want to enable the active page either
        _layout->itemAt(i)->widget()->setEnabled(enabled);
    }
}

void Pagination::setActivePageIndex(PageIndex activePageIndex)
{
    _activePageIndex = activePageIndex;

    for (PageIndex i = 0; i < _entries.count(); ++i) {
        // we don't want to store those buttons in this object's state
        auto button = qobject_cast<QRadioButton *>(_layout->itemAt(i)->widget());
        button->setChecked(i == activePageIndex);
    }

    enableOrDisableButtons();
}

PageIndex Pagination::activePageIndex()
{
    return _activePageIndex;
}

}
