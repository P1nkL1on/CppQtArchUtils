#include "file_generator.h"

FileGenerator::FileGenerator(QWidget *parent) :
    QWidget(parent)
{

}

void FileGenerator::addTemplates(const QStringList &templates)
{
    m_templates << templates;
}
