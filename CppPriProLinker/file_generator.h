#ifndef FILE_GENERATOR_H
#define FILE_GENERATOR_H

#include <QWidget>
#include "template_decoder.h"
#include "template_loader.h"

// base class for generating any file type
class FileGenerator : public QWidget
{
    Q_OBJECT
public:
    explicit FileGenerator(QWidget *parent = nullptr);
    void addTemplates(const QStringList &templates);
    virtual TemplateDecoder decoder() const = 0;
protected:
    QStringList m_templates;
};

#endif // FILE_GENERATOR_H
