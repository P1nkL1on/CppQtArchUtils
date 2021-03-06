#include "form.h"

#include <QDebug>
#include <QtGlobal>
#include <QCheckBox>
#include <QSpinBox>
#include <QBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

#include "lineedit_withhintlist.h"

Form::Form(QWidget *parent)
    : QMainWindow(parent)
{
    m_loader = TemplateLoader({{"pri", ":/pri_0.txt"}});

    // layout
    auto centralWidget = new QWidget;
    auto vLayout = new QVBoxLayout;
    auto fLayout = new QFormLayout;
    auto skipEmptyBlocksEdit = new QCheckBox;
    auto tabToSpaceCountEdit  = new QSpinBox;
    auto newLinesBetweenBlocksEdit = new QSpinBox;
    m_textPreviewEdit = new QPlainTextEdit;


    centralWidget->setLayout(vLayout);
    vLayout->addLayout(fLayout, 1);
    fLayout->addRow("Tab to Space Count", tabToSpaceCountEdit);
    tabToSpaceCountEdit->setValue(4);
    fLayout->addRow("Skip Empty Blocks", skipEmptyBlocksEdit);
    fLayout->addRow("New Lines Between Blocks", newLinesBetweenBlocksEdit);

    vLayout->addWidget(m_textPreviewEdit, 1);
    m_textPreviewEdit->setReadOnly(true);
    m_textPreviewEdit->setFont(QFont("Courier New", 10));

    for (int i = 0; i < 5; ++i){
        auto editor = new LineEditWithHintList;
        editor->addHints({"abcdefgh", "ghjlqwepoqwi123456", "13579", "00000000", "asdasdsad1220", "aasss0oooo0", "0", "FUU00UCK"});
        vLayout->addWidget(editor);
    }

    setCentralWidget(centralWidget);


    connect(tabToSpaceCountEdit, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=]{
        m_tabToSpaceCount = tabToSpaceCountEdit->value();
        updatePreview();
    });
    connect(skipEmptyBlocksEdit, &QCheckBox::toggled, this, [=]{
        m_skipEmptyBlocks = skipEmptyBlocksEdit->isChecked();
        updatePreview();
    });
    connect(newLinesBetweenBlocksEdit, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=]{
        m_newLinesBetweenBlocks = newLinesBetweenBlocksEdit->value();
        updatePreview();
    });

    updatePreview();
}

Form::~Form()
{

}

void Form::updatePreview()
{
    PlainFileData plainData;
    QString err;
    m_loader.readTemplate("pri", plainData, err);
    if (not err.isEmpty()){
        QMessageBox::warning(this, "File Loading Failed", err);
        return;
    }
    const FileData data = plainData.split("\n");

    const QStringList pris {
        "PriAlpha.pri", "PriBeta.pri", "PriAnother.pri"
    };
    const QStringList headers {
        "form.h", "computer.h"
    };
    const QStringList sources {
        "form.cpp", "computer.cpp", "main.cpp"
    };
    const QStringList resources {
    };
    m_decoder = TemplateDecoder::proDecoder(
                pris,
                headers,
                sources,
                resources,
                m_tabToSpaceCount,
                m_skipEmptyBlocks,
                m_newLinesBetweenBlocks);

    const FileData decodedData = m_decoder.decode(data);
    m_textPreviewEdit->setPlainText(decodedData.join("\n"));
}
