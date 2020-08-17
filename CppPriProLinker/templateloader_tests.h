#ifndef TEMPLATELOADER_TESTS_H
#define TEMPLATELOADER_TESTS_H

#include <QObject>

class TemplateLoaderTests : public QObject
{
    Q_OBJECT
public:
    TemplateLoaderTests(QObject *parent = nullptr);
    ~TemplateLoaderTests() override;
private slots:
    void readEmptyFile();
    void readMultiLine();
    void readOneLine();
    void readUnexist();
};

#endif // TEMPLATELOADER_TESTS_H
