#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QBoxLayout>

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(
            QWidget *parent = nullptr,
            Qt::WindowFlags f = Qt::WindowFlags());
    ~Dialog();
    QDialogButtonBox *buttons() const;
    void setCentralWidget(QWidget *widget);
protected:
    QDialogButtonBox *m_buttons = nullptr;
    QVBoxLayout *m_contentLayout = nullptr;
    QWidget *m_centralWidget = nullptr;
};

#endif // DIALOG_H
