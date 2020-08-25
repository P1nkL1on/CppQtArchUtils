#include "lineeditwithhintlist.h"

#include <algorithm>

#include <QStringList>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QtDebug>
#include <QApplication>
#include <QFocusEvent>

LineEditWithHintList::LineEditWithHintList(QWidget *parent) : QWidget(parent)
{
    auto layout = new QVBoxLayout;
    setLayout(layout);

    m_textEdit = new QLineEdit;
    m_textEdit->setFocusPolicy(Qt::StrongFocus);
    m_textEdit->installEventFilter(this);
    layout->addWidget(m_textEdit);

    m_searchFrame = new QFrame;
    m_searchFrame->setContentsMargins(0, 0, 0, 0);
    auto searchLayout = new QVBoxLayout(this);
    searchLayout->setContentsMargins(0, 0, 0, 0);
    m_searchFrame->setLayout(searchLayout);
    m_searchFrame->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_searchFrame->hide();

    m_caseSensetiveEdit = new QCheckBox("Case Sensetive");
    m_listView = new QListWidget;
    m_listView->setAcceptDrops(false);
    m_listView->setFocusPolicy(Qt::NoFocus);
    m_listView->setSelectionMode(QAbstractItemView::NoSelection);
    m_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    searchLayout->addWidget(m_listView, 1);
    searchLayout->addWidget(m_caseSensetiveEdit);
    m_searchFrame->setFocusPolicy(Qt::NoFocus);
    m_searchFrame->installEventFilter(this);

    connect(m_textEdit, &QLineEdit::textChanged, this, &LineEditWithHintList::updateList);
    connect(m_caseSensetiveEdit, &QCheckBox::toggled, this, &LineEditWithHintList::updateList);
    connect(m_listView, &QListWidget::itemDoubleClicked, this, &LineEditWithHintList::confirmHint);
    connect(m_listView, &QListWidget::itemPressed, this, [this](QListWidgetItem * item){
        select(item->text());
    });

    installEventFilter(this);
}

LineEditWithHintList::~LineEditWithHintList()
{
    m_textEdit->removeEventFilter(this);
    m_searchFrame->removeEventFilter(this);
}

void LineEditWithHintList::addHints(const QStringList &items)
{
    m_items << items;
    updateList();
}

void LineEditWithHintList::updateList()
{
    updateListItems(m_textEdit->text());
    updateListSelectedItem();
    updateListPos();
}

void LineEditWithHintList::confirmHint()
{
    if (m_selected.isEmpty())
        return;

    m_textEdit->setText(m_selected);

    m_selected.clear();
    updateListSelectedItem();
}

void LineEditWithHintList::confirmText()
{
    qDebug() << "editing finished:" << m_textEdit->text();
    hideList();
    emit editingFinished();
}

void LineEditWithHintList::selectWithOffset(int offset)
{
    if (m_shownItemsCache.isEmpty())
        return;
    const int currentInd = m_shownItemsCache.indexOf(m_selected);
    const int newInd = qBound(0, currentInd + offset, m_shownItemsCache.size() - 1);
    select(m_shownItemsCache.at(newInd));
}

void LineEditWithHintList::select(const QString &item)
{
    if (m_items.isEmpty())
        return;
    if (not m_items.contains(item))
        return;
    m_selected = item;
    updateListSelectedItem();
}

void LineEditWithHintList::selectNext()
{
    selectWithOffset(1);
}

void LineEditWithHintList::selectPrev()
{
    selectWithOffset(-1);
}

void LineEditWithHintList::showList()
{
    if (m_searchFrame->isVisible())
        return;
    m_searchFrame->show();
    m_searchFrame->setFocus();
}

void LineEditWithHintList::hideList()
{
    if (m_searchFrame->isHidden())
        return;
    m_searchFrame->hide();
}

bool LineEditWithHintList::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::FocusIn and watched == m_textEdit){
        showList();
        updateList();
    }

    if (event->type() == QEvent::FocusOut and watched == m_textEdit){
        QFocusEvent *focusEvent = dynamic_cast<QFocusEvent *>(event);
        if (focusEvent->reason() != Qt::OtherFocusReason)
            confirmText();
    }

    if (event->type() == QEvent::KeyPress and watched == m_textEdit){
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return){
            if (not m_selected.isEmpty()){
                confirmHint();
                event->accept();
                return true;
            }
            if (not m_textEdit->text().isEmpty()){
                confirmText();
                event->accept();
                return true;
            }
        }
        if (keyEvent->key() == Qt::Key_Down){
            selectNext();
            event->accept();
            return true;
        }
        if (keyEvent->key() == Qt::Key_Up){
            selectPrev();
            event->accept();
            return true;
        }
    }
    if (event->type() == QEvent::Wheel){
        QWheelEvent *wheelEvent = dynamic_cast<QWheelEvent *>(event);
        if (wheelEvent->delta() < 0){
            selectNext();
            event->accept();
            return true;
        }
        if (wheelEvent->delta() > 0){
            selectPrev();
            event->accept();
            return true;
        }
    }

    if (event->type() == QEvent::KeyPress and watched == m_searchFrame){
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);
        QKeyEvent eventRedirected(keyEvent->type(), keyEvent->key(), keyEvent->modifiers(), keyEvent->text(), keyEvent->isAutoRepeat(), keyEvent->count());
        return qApp->sendEvent(m_textEdit, &eventRedirected);
    }
    return QWidget::eventFilter(watched, event);
}

void LineEditWithHintList::updateListItems(const QString &searchString)
{
    m_listView->clear();
    m_lastSelectedItem = nullptr;
    if (searchString.isEmpty()){
        m_listView->addItems(m_items);
        m_shownItemsCache = m_items;
        return;
    }

    QStringList sortedItems = m_items;
    QHash<QString, bool> hasAnyScore;
    sortOptionsByScore(
                searchString,
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
}

void LineEditWithHintList::updateListSelectedItem()
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

void LineEditWithHintList::updateListPos()
{
    const QPoint localPos = m_textEdit->rect().bottomLeft();
    const QPoint globalPos = m_textEdit->mapToGlobal(localPos);
    m_searchFrame->move(globalPos);
    m_searchFrame->adjustSize();
}

void LineEditWithHintList::sortOptionsByScore(
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

int LineEditWithHintList::optionScore( const QString &searchingFor, const QString &value)
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
