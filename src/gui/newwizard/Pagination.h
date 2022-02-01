#pragma once

#include <QHBoxLayout>
#include <QString>
#include <QWidget>

namespace OCC::Wizard {

using PageIndex = QStringList::size_type;

/**
 * Renders pagination entries as radio buttons in a horizontal layout.
 *
 * The class assumes ownership of the layout, and will add/edit/remove buttons when needed.
 * Unfortunately, it cannot set itself as a parent, so make sure the layout's lifetime will exceed the pagination's.
 */
class Pagination : public QObject
{
    Q_OBJECT

public:
    explicit Pagination(QHBoxLayout *layout);

    ~Pagination() noexcept override;

    void setEntries(const QStringList &newEntries);

    [[nodiscard]] PageIndex size() const;

    PageIndex activePageIndex();

Q_SIGNALS:
    void paginationEntryClicked(PageIndex clickedPageIndex);

public Q_SLOTS:
    void setEnabled(bool enabled);
    void setActivePageIndex(PageIndex activePageIndex);

private:
    void removeAllItems();
    void enableOrDisableButtons();

    QHBoxLayout *_layout;
    QStringList _entries;
    PageIndex _activePageIndex;
    bool _enabled;
};

}
