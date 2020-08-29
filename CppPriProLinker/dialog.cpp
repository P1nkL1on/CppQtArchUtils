#include "dialog.h"

Dialog::Dialog(
        QWidget *parent,
        Qt::WindowFlags f) :
    QDialog(parent, f & ~Qt::WindowContextHelpButtonHint),
    m_buttons(new QDialogButtonBox),
    m_contentLayout(new QVBoxLayout)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    layout->addLayout(m_contentLayout, 1);
    layout->addWidget(m_buttons);
    connect(m_buttons, &QDialogButtonBox::accepted, this, &Dialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &Dialog::reject);
}

Dialog::~Dialog()
{

}

QDialogButtonBox *Dialog::buttons() const
{
    return m_buttons;
}

void Dialog::setCentralWidget(QWidget *widget)
{
    if (m_centralWidget)
        m_contentLayout->removeWidget(m_centralWidget);
    m_centralWidget = widget;
    m_contentLayout->addWidget(m_centralWidget, 1);
}
