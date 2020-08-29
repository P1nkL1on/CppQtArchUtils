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
    void setContent(QWidget *content);
    void setOkCancel();
protected:
    QDialogButtonBox *m_buttons = nullptr;
    QVBoxLayout *m_contentLayout = nullptr;
    QWidget *m_content = nullptr;
};

#endif // DIALOG_H
