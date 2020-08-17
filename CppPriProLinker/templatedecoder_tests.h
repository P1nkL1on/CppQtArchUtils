#ifndef TEMPLATEDECODERTESTS_H
#define TEMPLATEDECODERTESTS_H

#include <QObject>

class TemplateDecoderTests : public QObject
{
    Q_OBJECT
public:
    TemplateDecoderTests(QObject *parent = nullptr);
    ~TemplateDecoderTests() override;
private slots:
    void varTest();
    void multiVarTest();
    // conflicts UB /w hash
    // iterating
    void templateTest();
    void emptyTemplateTest();
    void multiTemplateTest();
    void severalTemplateTest();

    void templateToVarTest();
};

#endif // TEMPLATEDECODERTESTS_H
