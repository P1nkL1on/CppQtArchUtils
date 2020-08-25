#ifndef GUARDCONVERTERTESTS_H
#define GUARDCONVERTERTESTS_H

#include <QObject>
#include <QString>

class GuardConverterTests : public QObject
{
    Q_OBJECT
public:
    GuardConverterTests(QObject *parent = nullptr);
    ~GuardConverterTests() override;
private slots:
    void expectedConvertions();
    void fileExtConvertions();
};

#endif // GUARDCONVERTERTESTS_H
