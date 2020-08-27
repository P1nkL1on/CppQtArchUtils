#ifndef STRINGLISTEDIT_H
#define STRINGLISTEDIT_H

#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QBoxLayout>
#include <QFrame>

class LineEditWithHintList : public QWidget
{
    Q_OBJECT
public:
    explicit LineEditWithHintList(QWidget *parent = nullptr);
    ~LineEditWithHintList() override;
    void addHints(const QStringList &items);
signals:
    void editingFinished();
protected slots:
    void updateList();
    void confirmHint();
    void confirmText();
protected:
    void selectWithOffset(int offset);
    void updateListItems(const QString &searchString);
    void updateListSelectedItem();
    void updateListPos();
    void select(const QString &item);
    void selectNext();
    void selectPrev();
    void showList();
    void hideList();

    bool eventFilter(QObject *watched, QEvent *event) override;

    QLineEdit *m_textEdit = nullptr;
    QCheckBox *m_caseSensetiveEdit = nullptr;
    QListWidget *m_listView = nullptr;
    QFrame *m_searchFrame = nullptr;

    QStringList m_items;
    QString m_selected;
    QStringList m_shownItemsCache;
    QListWidgetItem *m_lastSelectedItem = nullptr;

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
