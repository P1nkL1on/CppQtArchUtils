#include "dialog_progress.h"

DialogProgress::DialogProgress(
        QWidget *parent,
        ShowElements s,
        Qt::WindowFlags f) :
    Dialog(parent, f)
{
}

DialogProgress::~DialogProgress()
{

}

void DialogProgress::setTotal(int total)
{
    Q_ASSERT(total > 0);
}

void DialogProgress::setProgress(int progress)
{

}

void DialogProgress::reject()
{
    Dialog::reject();
}

void DialogProgress::closeEvent(QCloseEvent *e)
{

}
