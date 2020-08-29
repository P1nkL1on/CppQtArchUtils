#include "dialog_progress.h"
#include <QPushButton>
#include <QFormLayout>
#include <QTime>
#include <QCloseEvent>

DialogProgress::DialogProgress(
        QWidget *parent,
        ShowElements s,
        Qt::WindowFlags f) :
    Dialog(parent, f),
    m_shownElements(s)
{
    buttons()->clear();
    if (s & TerminataBtn){
        buttons()->addButton(QDialogButtonBox::Cancel);
        buttons()->button(QDialogButtonBox::Cancel)->setText("Terminate");
    }

    QWidget *centralWidget = new QWidget;
    QFormLayout *layout = new QFormLayout;
    layout->setLabelAlignment(Qt::AlignLeft);
    layout->setAlignment(Qt::AlignRight);
    centralWidget->setLayout(layout);

    m_labelElapsed = new QLabel(formatTime(0), this);
    m_labelRemaining = new QLabel(formatTime(0), this);
    m_progressBar = new QProgressBar(this);
    m_progressBar->setTextVisible(false);

    if (s & ElapsedTime)
        layout->addRow("Elapsed time", m_labelElapsed);
    if (s & RemainingTime)
        layout->addRow("Remaining time", m_labelRemaining);
    if (s & ProgressBar)
        layout->addRow(m_progressBar);

    setCentralWidget(centralWidget);
}

DialogProgress::~DialogProgress()
{

}

void DialogProgress::setTotal(int total)
{
    Q_ASSERT(total > 0);
    m_total = total;
    m_progressBar->setMaximum(m_total);
}

void DialogProgress::setProgress(int progress)
{
    Q_ASSERT(progress >= 0);
    m_progress = progress;
    m_progressBar->setValue(m_progress);
    const int64_t timeElapsed = m_elaspedTimer.elapsed();
    const int64_t timeRemainig = int64_t(timeElapsed / double(progress) * (m_total - progress));
    const QString remainigStr = formatTime(timeRemainig);
    m_labelRemaining->setText(remainigStr);
}
int DialogProgress::exec()
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [=]{
        const QString elapsedStr = formatTime(m_elaspedTimer.elapsed());
        m_labelElapsed->setText(elapsedStr);
    });
    m_timer->start(1000);
    m_elaspedTimer.restart();
    return Dialog::exec();
}


void DialogProgress::reject()
{
    if (m_isTerminated)
        return;
    m_isTerminated = true;
    emit terminated();
    delete m_timer;
    if (m_shownElements & TerminataBtn){
        QPushButton *cancelButton = buttons()->button(QDialogButtonBox::Cancel);
        cancelButton->setText("Terminating...");
        cancelButton->setEnabled(false);
    }
}

void DialogProgress::closeEvent(QCloseEvent *e)
{
    e->ignore();
    reject();
}

QString DialogProgress::formatTime(int64_t ms) const
{
    return QTime(
                int(ms / 3600000),
                int(ms / 60000) % 60,
                (ms / 1000) % 60, 0)
            .toString("hh:mm:ss")
            .rightJustified(20);
}
