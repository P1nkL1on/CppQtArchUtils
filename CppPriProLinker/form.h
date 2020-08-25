#ifndef FORM_H
#define FORM_H

#include <QMainWindow>
#include <QPlainTextEdit>

#include "templateloader.h"
#include "templatedecoderfactory.h"

class Form : public QMainWindow
{
    Q_OBJECT

public:
    Form(QWidget *parent = nullptr);
    ~Form();
protected slots:
    void updatePreview();
protected:
    bool m_skipEmptyBlocks = false;
    int m_newLinesBetweenBlocks = 0;
    int m_tabToSpaceCount = 4;
    TemplateLoader m_loader;
    TemplateDecoder m_decoder;
    QPlainTextEdit *m_textPreviewEdit = nullptr;
};

#endif // FORM_H
