#include "stringlistedit.h"

#include <algorithm>

#include <QStringList>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QtDebug>

StringListEdit::StringListEdit(QWidget *parent) : QWidget(parent)
{
    auto layout = new QVBoxLayout;
    setLayout(layout);

    m_textEdit = new QLineEdit;
    m_textEdit->setFocusPolicy(Qt::StrongFocus);
    layout->addWidget(m_textEdit);

    auto toolsLayout = new QHBoxLayout;
    layout->addLayout(toolsLayout);


    m_editFrame = new QFrame;
    auto editLayout = new QHBoxLayout;
    m_editFrame->setLayout(editLayout);
    toolsLayout->addWidget(m_editFrame, 1);

    m_addButton = new QPushButton("Add");
    editLayout->addWidget(m_addButton);
    m_removeButton = new QPushButton("Remove");
    editLayout->addWidget(m_removeButton);


    m_searchFrame = new QFrame;
    auto searchLayout = new QHBoxLayout;
    m_searchFrame->setLayout(searchLayout);
    toolsLayout->addWidget(m_searchFrame, 1);

    m_caseSensetiveEdit = new QCheckBox("Case Sensetive");
    searchLayout->addWidget(m_caseSensetiveEdit);


    m_modeEdit = new QComboBox;
    m_modeEdit->addItems({"Search", "Edit"});
    toolsLayout->addWidget(m_modeEdit);

    m_listView = new QListWidget;
    m_listView->setAcceptDrops(false);
    m_listView->setFocusPolicy(Qt::NoFocus);
    m_listView->setSelectionMode(QAbstractItemView::NoSelection);
    layout->addWidget(m_listView, 1);

    setMode(Mode::Search);

    connect(m_textEdit, &QLineEdit::textChanged, this, &StringListEdit::updateListAccordingToLineEdit);
    connect(m_caseSensetiveEdit, &QCheckBox::toggled, this, &StringListEdit::updateListAccordingToLineEdit);
    connect(m_listView, &QListWidget::itemPressed, this, [this](QListWidgetItem * item){
        select(item->text());
    });
}

void StringListEdit::addItems(const QStringList &items)
{
    m_items << items;
    updateListAccordingToLineEdit();
}

void StringListEdit::selectWithOffset(int offset)
{
    if (m_shownItemsCache.isEmpty())
        return;
    const int currentInd = m_shownItemsCache.indexOf(m_selected);
    const int newInd = qBound(0, currentInd + offset, m_shownItemsCache.size() - 1);
    qDebug() << currentInd << "->" << newInd;
    select(m_shownItemsCache.at(newInd));
}

void StringListEdit::select(const QString &item)
{
    if (m_items.isEmpty())
        return;
    if (not m_items.contains(item))
        return;
    m_selected = item;
    updateSelectedView();
}

void StringListEdit::selectNext()
{
    selectWithOffset(1);
}

void StringListEdit::selectPrev()
{
    selectWithOffset(-1);
}

void StringListEdit::updateListAccordingToLineEdit()
{
    m_listView->clear();
    m_lastSelectedItem = nullptr;
    if (m_textEdit->text().isEmpty()){
        m_listView->addItems(m_items);
        m_shownItemsCache = m_items;
        updateSelectedView();
        return;
    }

    QStringList sortedItems = m_items;
    QHash<QString, bool> hasAnyScore;
    sortOptionsByScore(
                m_textEdit->text(),
                m_caseSensetiveEdit->isChecked(),
                sortedItems,
                hasAnyScore);
    m_shownItemsCache.clear();
    for (const QString &item : sortedItems)
        if (hasAnyScore.value(item, false))
            m_shownItemsCache << item;
    m_listView->addItems(m_shownItemsCache);

    const int shownCount = m_shownItemsCache.size();
    const int totalCount = m_items.size();
    if (shownCount < totalCount){
        QListWidgetItem *lastItem = new QListWidgetItem;
        lastItem->setFlags(Qt::NoItemFlags);
        lastItem->setText(QString("and %1 other item(s)").arg(totalCount - shownCount));
        lastItem->setForeground(QBrush(Qt::gray));
        m_listView->addItem(lastItem);
    }
    updateSelectedView();
}

void StringListEdit::updateSelectedView()
{
    if (m_lastSelectedItem)
        m_lastSelectedItem->setBackgroundColor(Qt::white);

    const int selectedInd = m_shownItemsCache.indexOf(m_selected);
    if (selectedInd < 0){
        m_lastSelectedItem = nullptr;
        return;
    }
    m_lastSelectedItem = m_listView->item(selectedInd);
    if (not m_lastSelectedItem)
        return;
    m_lastSelectedItem->setBackgroundColor(Qt::yellow);
    m_listView->scrollToItem(m_lastSelectedItem);
}

void StringListEdit::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Down){
        selectNext();
        e->accept();
        return;
    }
    if (e->key() == Qt::Key_Up){
        selectPrev();
        e->accept();
        return;
    }
    QWidget::keyPressEvent(e);
}

void StringListEdit::wheelEvent(QWheelEvent *e)
{
    if (e->delta() < 0){
        selectNext();
        e->accept();
        return;
    }
    if (e->delta() > 0){
        selectPrev();
        e->accept();
        return;
    }
    QWidget::wheelEvent(e);
}

void StringListEdit::setMode(StringListEdit::Mode mode)
{
    Q_ASSERT(mode != Mode::Invalid);
    if (mode == m_mode)
        return;
    m_searchFrame->setVisible(mode == Mode::Search);
    m_editFrame->setVisible(mode == Mode::Edit);
}

void StringListEdit::sortOptionsByScore(
        const QString &searchingFor,
        bool caseSensetive,
        QStringList &optionList,
        QHash<QString, bool> &hasAnyScore)
{
    const auto score = [&](const QString &s){
        const int score = optionScore(caseSensetive ? searchingFor : searchingFor.toLower(), caseSensetive ? s : s.toLower());
        hasAnyScore.insert(s, score > std::numeric_limits<int>::min());
        return score;
    };
    std::sort(optionList.begin(), optionList.end(), [score](const QString &a, const QString &b) {
        return score(a) > score(b);
    });
}

int StringListEdit::optionScore( const QString &searchingFor, const QString &value)
{
    int score = 0;
    int valueInd = 0;
    int searchingForInd = 0;

    while (searchingForInd < searchingFor.size()){
        const auto searchForCharacter = searchingFor[searchingForInd];
        int distance = 0;
        while (valueInd < value.size() and value[valueInd] != searchForCharacter){
            ++valueInd;
            ++distance;
        }
        if (valueInd >= value.size())
            return std::numeric_limits<int>::min();
        ++searchingForInd;
        ++valueInd;
        score -= distance;
    }
    return score;
}
