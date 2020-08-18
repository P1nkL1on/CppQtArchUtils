#include "stringlistedit.h"


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

    m_modeEdit = new QComboBox;
    m_modeEdit->addItems({"Search", "Edit"});
    toolsLayout->addWidget(m_modeEdit);

    m_listView = new QListWidget;
    m_listView->setAcceptDrops(false);
    m_listView->setFocusPolicy(Qt::NoFocus);
    m_listView->setSelectionMode(QAbstractItemView::NoSelection);
    layout->addWidget(m_listView, 1);

    setMode(Mode::Search);
}

void StringListEdit::addItems(const QStringList &items)
{
    m_items << items;
    updateListAccordingToLineEdit();
}

void StringListEdit::updateListAccordingToLineEdit()
{
    m_listView->clear();
    m_listView->addItems(m_items);
}

void StringListEdit::setMode(StringListEdit::Mode mode)
{
    Q_ASSERT(mode != Mode::Invalid);
    if (mode == m_mode)
        return;
    m_searchFrame->setVisible(mode == Mode::Search);
    m_editFrame->setVisible(mode == Mode::Edit);
}
