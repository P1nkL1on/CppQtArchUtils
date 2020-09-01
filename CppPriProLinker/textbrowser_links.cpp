#include "textbrowser_links.h"

TextBrowserLinks::TextBrowserLinks(QWidget *parent) :
    QTextBrowser(parent)
{
    setReadOnly(true);
    setOpenLinks(false);
    setOpenExternalLinks(false);
    connect(this, QOverload<const QUrl &>::of(&QTextBrowser::highlighted),
            this, &TextBrowserLinks::onUrlHighLighted);
    connect(this, &QTextBrowser::anchorClicked,
            this, &TextBrowserLinks::onUrlClicked);
}

void TextBrowserLinks::setRefs(const QList<RefFile> &refs)
{
    m_refs = refs;
    updateView();
}

void TextBrowserLinks::setText(const QString &text)
{
    m_text = text;
    updateView();
}

void TextBrowserLinks::onUrlHighLighted(const QUrl &url)
{
    const int ind = indexOfUrl(url);
    if (ind < 0)
        return;
    const RefFile res = m_refs[ind];
    emit refHighLighted(res);
}

void TextBrowserLinks::onUrlClicked(const QUrl &url)
{
    const int ind = indexOfUrl(url);
    if (ind < 0)
        return;
    const RefFile res = m_refs[ind];
    emit refClicked(res);
}

void TextBrowserLinks::updateView()
{
    document()->clear();
    QTextCursor cursor(document());
    QMap<int, QString> formats;
    int refInd = 0;
    for (const RefFile &ref : m_refs){
        QTextCharFormat linkFormat = cursor.charFormat();
        formats.insert(ref.pos, "-1");
        formats.insert(ref.pos + ref.text.size(), QString::number(refInd));
        ++refInd;
    }
    formats.insert(m_text.size() - 1, {});
    int prev = 0;
    for (int pos : formats.keys()){
        QTextCharFormat format;
        const QString currentLink = formats.value(pos);
        const QString textSegment = m_text.mid(prev, pos - prev);
        format.setAnchorHref(currentLink);
        format.setFont(QFont("Courier New", 14));
        setCurrentCharFormat(format);
        cursor.insertText(textSegment, format);
        prev = pos;
    }
}

int TextBrowserLinks::indexOfUrl(const QUrl &url) const
{
    const QString text = url.url();
    bool isOk = false;
    const int index = text.toInt(&isOk);
    return isOk ? index : -1;
}
