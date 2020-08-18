#ifndef STRINGLISTEDIT_H
#define STRINGLISTEDIT_H

#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QBoxLayout>
#include <QFrame>

class StringListEdit : public QWidget
{
    Q_OBJECT
public:
    explicit StringListEdit(QWidget *parent = nullptr);
    void addItems(const QStringList &items);
protected slots:
    void updateListAccordingToLineEdit();
protected:
    enum class Mode {Invalid, Search, Edit};
    QComboBox *m_modeEdit = nullptr;
    QLineEdit *m_textEdit = nullptr;
    QPushButton *m_addButton = nullptr;
    QPushButton *m_removeButton = nullptr;
    QCheckBox *m_caseSensetiveEdit = nullptr;
    QListWidget *m_listView = nullptr;
    QFrame *m_editFrame = nullptr;
    QFrame *m_searchFrame = nullptr;
    Mode m_mode = Mode::Invalid;
    QStringList m_items;

    void setMode(Mode mode);
    static void sortOptionsByScore(
            const QString &searchingFor,
            bool caseSensetive,
            QStringList &optionList,
            QHash<QString, bool> &hasAnyScore);
    static int optionScore(
            const QString &searchingFor,
            const QString &option);
};

#endif // STRINGLISTEDIT_H
