#include "stringlistedit.h"
#include <algorithm>
#include <QStringList>
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
    connect(m_listView, &QListWidget::itemClicked, this, [](QListWidgetItem *){
        // clicked
    });
}

void StringListEdit::addItems(const QStringList &items)
{
    m_items << items;
    updateListAccordingToLineEdit();
}

void StringListEdit::updateListAccordingToLineEdit()
{
    m_listView->clear();
    if (m_textEdit->text().isEmpty()){
        m_listView->addItems(m_items);
        return;
    }

    QStringList sortedItems = m_items;
    QHash<QString, bool> hasAnyScore;
    sortOptionsByScore(
                m_textEdit->text(),
                m_caseSensetiveEdit->isChecked(),
                sortedItems,
                hasAnyScore);
    QStringList shownItems;
    for (const QString &item : sortedItems)
        if (hasAnyScore.value(item, false))
            shownItems << item;
    m_listView->addItems(shownItems);
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
