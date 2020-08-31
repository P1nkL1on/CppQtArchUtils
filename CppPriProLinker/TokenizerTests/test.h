#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "test_header.h"

struct StructDefine;

struct Struct {
	
};

class Class : public QWidget
{
    Q_OBJECT
public:
    explicit Class(QWidget *parent = nullptr);
    ~Class();
protected:
    QWidget *m_centralWidget = nullptr;
};

namespace A {
	
class AClass;	
	
namespace B {

class InnerClass : public AClass;

}
	
}

#endif // DIALOG_H
