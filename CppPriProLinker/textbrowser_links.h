#ifndef LINKTEXTBROWSER_H
#define LINKTEXTBROWSER_H

#include <QTextBrowser>

#include "ref_file.h"

class TextBrowserLinks : public QTextBrowser
{
    Q_OBJECT
public:
    TextBrowserLinks(QWidget *parent = nullptr);
    void setRefs(const QList<RefFile> &refs);
    void setText(const QString &text);
signals:
    void refHighLighted(const RefFile &ref);
    void refClicked(const RefFile &ref);
protected slots:
    void onUrlHighLighted(const QUrl &url);
    void onUrlClicked(const QUrl &url);
protected:
    void updateView();
    int indexOfUrl(const QUrl &url) const;
    QList<RefFile> m_refs;
    QString m_text;
};

#endif // LINKTEXTBROWSER_H
