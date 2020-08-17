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
    void test01();
};

#endif // TEMPLATEDECODERTESTS_H
