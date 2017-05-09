#include "subtoolbar.h"
#include "toolbutton.h"
#include "colorbutton.h"
#include "fontsizewidget.h"
#include "utils/baseutils.h"
#include "utils/configsettings.h"

#include <QLineEdit>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QSlider>
#include <QDebug>

namespace {
    const int TOOLBAR_HEIGHT = 28;
    const int TOOLBAR_WIDTH = 284;
    const int BUTTON_SPACING = 3;
    const int COLOR_NUM = 16;
}

SubToolBar::SubToolBar(QWidget *parent)
    : QStackedWidget(parent),
      m_lineWidth(1)
{
    initWidget();
}

void SubToolBar::initWidget() {
    setObjectName("SubToolBar");
    setStyleSheet(getFileContent(":/resources/qss/subtoolbar.qss"));
    setFixedSize(TOOLBAR_WIDTH, TOOLBAR_HEIGHT);

    initRectLabel();
    initArrowLabel();
    initLineLabel();
    initTextLabel();
    initColorLabel();
    initSaveLabel();

    setCurrentWidget(0);
}

void SubToolBar::initRectLabel() {
    m_rectLabel = new QLabel(this);
    QButtonGroup* rectBtnGroup = new QButtonGroup();
    rectBtnGroup->setExclusive(true);
    QList<ToolButton*> btnList;
    //rectangle, oval...
    ToolButton* fineLine = new ToolButton();
    fineLine->setObjectName("FineLine");
    rectBtnGroup->addButton(fineLine);
    btnList.append(fineLine);
    ToolButton*  mediumLine = new ToolButton();
    mediumLine->setObjectName("MediumLine");
    rectBtnGroup->addButton(mediumLine);
    btnList.append(mediumLine);
    ToolButton* thickLine = new ToolButton();
    thickLine->setObjectName("ThickLine");
    rectBtnGroup->addButton(thickLine);
    btnList.append(thickLine);
    //seperator line...
    QLabel* vSeperatorLine = new QLabel();
    vSeperatorLine->setFixedSize(1, 16);
    vSeperatorLine->setObjectName("VerticalSeperatorLine");
    //blur, mosaic...
    ToolButton* blurBtn = new ToolButton();
    blurBtn->setObjectName("BlurBtn");
    ToolButton* mosaicBtn = new ToolButton();
    mosaicBtn->setObjectName("MosaicBtn");
    connect(blurBtn, &ToolButton::clicked, this, [=]{
        ConfigSettings::instance()->setValue("effect", "is_blur", blurBtn->isChecked());
        if (blurBtn->isChecked()) {
            mosaicBtn->setChecked(false);
            ConfigSettings::instance()->setValue("effect", "is_mosaic", false);
        }
    });
    connect(mosaicBtn, &ToolButton::clicked, this, [=]{
        ConfigSettings::instance()->setValue("effect", "is_mosaic", mosaicBtn->isChecked());
        if (mosaicBtn->isChecked()) {
            blurBtn->setChecked(false);
            ConfigSettings::instance()->setValue("effect", "is_blur", false);
        }
    });
    int lineWidthIndex = ConfigSettings::instance()->value("rectangle",
                                                      "linewidth_index").toInt();
    btnList[lineWidthIndex]->setChecked(true);

    QHBoxLayout* rectLayout = new QHBoxLayout();
    rectLayout->setMargin(0);
    rectLayout->setSpacing(4);
    rectLayout->addSpacing(4);
    for (int i = 0; i < btnList.length(); i++) {
        rectLayout->addWidget(btnList[i]);
        connect(btnList[i], &ToolButton::clicked, this, [=]{
                ConfigSettings::instance()->setValue(m_currentType, "linewidth_index", i);
        });
    }
    rectLayout->addSpacing(2);
    rectLayout->addWidget(vSeperatorLine);
    rectLayout->addSpacing(2);
    rectLayout->addWidget(blurBtn);
    rectLayout->addWidget(mosaicBtn);
    rectLayout->addStretch();
    m_rectLabel->setLayout(rectLayout);
    addWidget(m_rectLabel);

    connect(this, &SubToolBar::shapeChanged, this, [=]{
        int lineIndex = ConfigSettings::instance()->value(m_currentType,
                                                          "linewidth_index").toInt();
        btnList[lineIndex]->setChecked(true);
    });
}

void SubToolBar::initArrowLabel() {
    m_arrowLabel = new QLabel(this);
    //arrow
    QButtonGroup*  arrowBtnGroup = new QButtonGroup();
    arrowBtnGroup->setExclusive(true);
    QList<ToolButton*> arrowBtnList;
    ToolButton* arrowFineLine = new ToolButton();
    arrowFineLine->setObjectName("ArrowFine");
    arrowBtnGroup->addButton(arrowFineLine);
    arrowBtnList.append(arrowFineLine);
    ToolButton*  arrowMediumLine = new ToolButton();
    arrowMediumLine->setObjectName("ArrowMedium");
    arrowBtnGroup->addButton(arrowMediumLine);
    arrowBtnList.append(arrowMediumLine);
    ToolButton* arrowThickLine = new ToolButton();
    arrowThickLine->setObjectName("ArrowThick");
    arrowBtnGroup->addButton(arrowThickLine);
    arrowBtnList.append(arrowThickLine);
    //line
    QButtonGroup*  lineBtnGroup = new QButtonGroup();
    lineBtnGroup->setExclusive(true);
    //rectangle, oval...
    ToolButton* fineLine = new ToolButton();
    fineLine->setObjectName("ArrowFineLine");
    lineBtnGroup->addButton(fineLine);
    arrowBtnList.append(fineLine);
    ToolButton*  mediumLine = new ToolButton();
    mediumLine->setObjectName("ArrowMediumLine");
    lineBtnGroup->addButton(mediumLine);
    arrowBtnList.append(mediumLine);
    ToolButton* thickLine = new ToolButton();
    thickLine->setObjectName("ArrowThickLine");
    lineBtnGroup->addButton(thickLine);
    arrowBtnList.append(thickLine);
    //seperator line...
    QLabel* vSeperatorLine = new QLabel();
    vSeperatorLine->setFixedSize(1, 16);
    vSeperatorLine->setObjectName("VerticalSeperatorLine");
    QButtonGroup* styleBtnGroup = new QButtonGroup;
    styleBtnGroup->setExclusive(true);
    ToolButton*  lineBtn = new ToolButton();
    lineBtn->setObjectName("LineBtn"); 
    styleBtnGroup->addButton(lineBtn, 0);
    ToolButton* arrowBtn = new ToolButton();
    arrowBtn->setObjectName("ArrowBtn");
    arrowBtn->setChecked(true);
    styleBtnGroup->addButton(arrowBtn, 1);
    if (ConfigSettings::instance()->value("arrow", "is_straight").toBool()) {
        lineBtn->setChecked(true);
    } else {
        arrowBtn->setChecked(true);
    }

    int arrowWidthIndex = ConfigSettings::instance()->value("arrow",
                                                      "linewidth_index").toInt();
    arrowBtnList[arrowWidthIndex]->setChecked(true);
    arrowBtnList[arrowWidthIndex+3]->setChecked(true);
    QHBoxLayout* arrowLayout = new QHBoxLayout();
    arrowLayout->setMargin(0);
    arrowLayout->setSpacing(2);
    for (int j = 0; j < arrowBtnList.length(); j++) {
        arrowLayout->addWidget(arrowBtnList[j]);
        connect(arrowBtnList[j], &ToolButton::clicked, this, [=]{
            if (j < 3) {
                 ConfigSettings::instance()->setValue("arrow", "linewidth_index", j);
            } else {
                ConfigSettings::instance()->setValue("arrow", "linewidth_index", j - 3);
            }
        });
    }
    arrowLayout->addSpacing(2);
    arrowLayout->addWidget(vSeperatorLine);
    arrowLayout->addSpacing(2);
    arrowLayout->addWidget(lineBtn);
    arrowLayout->addSpacing(2);
    arrowLayout->addWidget(arrowBtn);
    arrowLayout->addStretch();
    m_arrowLabel->setLayout(arrowLayout);
    addWidget(m_arrowLabel);

    fineLine->hide();
    mediumLine->hide();
    thickLine->hide();

    connect(arrowBtn, &ToolButton::toggled, this, [=](bool checked){
        if (checked) {
            arrowFineLine->show();
            arrowMediumLine->show();
            arrowThickLine->show();
            fineLine->hide();
            mediumLine->hide();
            thickLine->hide();
        } else {
            arrowFineLine->hide();
            arrowMediumLine->hide();
            arrowThickLine->hide();
            fineLine->show();
            mediumLine->show();
            thickLine->show();
        }
        ConfigSettings::instance()->setValue("arrow", "is_straight", !checked);
    });
    connect(this, &SubToolBar::shapeChanged, this, [=]{
        int lineIndex = ConfigSettings::instance()->value(m_currentType,
                                                          "linewidth_index").toInt();
        arrowBtnList[lineIndex]->setChecked(true);
        arrowBtnList[lineIndex+3]->setChecked(true);
    });
}

void SubToolBar::initLineLabel() {
     m_lineLabel = new QLabel(this);
     //rectangle, oval...
    QButtonGroup* lineBtnGroup = new QButtonGroup();
    lineBtnGroup->setExclusive(true);
    QList<ToolButton*> btnList;

    ToolButton* fineLine = new ToolButton();
    fineLine->setObjectName("FineLine");
    lineBtnGroup->addButton(fineLine);
    btnList.append(fineLine);
    ToolButton*  mediumLine = new ToolButton();
    mediumLine->setObjectName("MediumLine");
    lineBtnGroup->addButton(mediumLine);
    btnList.append(mediumLine);
    ToolButton* thickLine = new ToolButton();
    thickLine->setObjectName("ThickLine");
    lineBtnGroup->addButton(thickLine);
    btnList.append(thickLine);

    int lineWidthIndex = ConfigSettings::instance()->value("line",
                                                      "linewidth_index").toInt();
    btnList[lineWidthIndex]->setChecked(true);

    QHBoxLayout* lineLayout = new QHBoxLayout();
    lineLayout->setMargin(0);
    lineLayout->setSpacing(2);
    for(int k = 0; k < btnList.length(); k++) {
        lineLayout->addWidget(btnList[k]);
        connect(btnList[k], &ToolButton::clicked, this, [=]{
            ConfigSettings::instance()->setValue("line", "linewidth_index", k);
        });
    }

    lineLayout->addStretch();
    m_lineLabel->setLayout(lineLayout);
    addWidget(m_lineLabel);

    connect(this, &SubToolBar::shapeChanged, this, [=]{
        int lineIndex = ConfigSettings::instance()->value(m_currentType,
                                                          "linewidth_index").toInt();
        btnList[lineIndex]->setChecked(true);
    });
}

void SubToolBar::initTextLabel() {
    //text...
    FontSizeWidget* fontsizeWidget = new FontSizeWidget;
    m_textLabel = new QLabel(this);
    QHBoxLayout* textLayout = new QHBoxLayout();
    textLayout->setMargin(0);
    textLayout->setSpacing(0);
    textLayout->addSpacing(4);
    textLayout->addWidget(fontsizeWidget);
    textLayout->addStretch();
    m_textLabel->setLayout(textLayout);
    addWidget(m_textLabel);
}

void SubToolBar::initColorLabel() {
    m_colorLabel = new QLabel(this);
    QList<ColorButton*> colorBtnList;
    QButtonGroup* colorBtnGroup = new QButtonGroup(m_colorLabel);
    colorBtnGroup->setExclusive(true);

    for(int i = 0; i < COLOR_NUM; i ++) {
        ColorButton* colorBtn = new ColorButton(colorIndexOf(i));
        colorBtnList.append(colorBtn);
        colorBtnGroup->addButton(colorBtn);
    }

    QHBoxLayout* colorLayout = new QHBoxLayout();
    colorLayout->setMargin(0);
    colorLayout->setSpacing(2);
    colorLayout->addStretch();
    for(int i = 0; i < colorBtnList.length(); i++) {
        colorLayout->addWidget(colorBtnList[i]);
        connect(colorBtnList[i], &ColorButton::updatePaintColor,
                this, &SubToolBar::currentColorChanged);
    }
    colorLayout->addStretch();
    m_colorLabel->setLayout(colorLayout);

    addWidget(m_colorLabel);
}

void SubToolBar::initSaveLabel() {
    //save to...
    QList<ToolButton*> toolBtnList;
    ToolButton* saveDesktopBtn = new ToolButton();
    saveDesktopBtn->setObjectName("SaveToDesktop");
    toolBtnList.append(saveDesktopBtn);

    ToolButton* savePicBtn = new ToolButton();
    savePicBtn->setObjectName("SaveToPictureDir");
    toolBtnList.append(savePicBtn);

    ToolButton* saveSpecificDirBtn = new ToolButton();
    saveSpecificDirBtn->setObjectName("SaveToSpecificDir");
    toolBtnList.append(saveSpecificDirBtn);

    ToolButton* saveClipboardBtn = new ToolButton();
    saveClipboardBtn->setObjectName("SaveToClipboard");
    toolBtnList.append(saveClipboardBtn);

    ToolButton* saveAutoClipboardBtn = new ToolButton();
    saveAutoClipboardBtn->setObjectName("SaveToAutoClipboard");
    toolBtnList.append(saveAutoClipboardBtn);

    QLabel* lowQualityText = new QLabel();
    lowQualityText->setObjectName("LowQualityLabel");
    lowQualityText->setText("Low");
    QSlider* saveQualitySlider = new QSlider(Qt::Horizontal);
    saveQualitySlider->setObjectName("SaveQualitySlider");
    saveQualitySlider->setMinimum(1);
    saveQualitySlider->setMaximum(100);
    saveQualitySlider->setPageStep(1);
    saveQualitySlider->setSliderPosition(100);
    QLabel* highQualityText = new QLabel();
    highQualityText->setObjectName("HighQualityLabel");
    highQualityText->setText("High");

     m_saveLabel = new QLabel(this);
    QHBoxLayout* saveLayout = new QHBoxLayout();
    saveLayout->setMargin(0);
    saveLayout->setSpacing(3);
    saveLayout->addSpacing(5);
    foreach (ToolButton* btn, toolBtnList) {
        saveLayout->addWidget(btn);
        connect(btn, &ToolButton::clicked, this,  [=]{
            setSaveOption(toolBtnList.indexOf(btn));
        });
    }
    saveLayout->addWidget(lowQualityText);
    saveLayout->addSpacing(2);
    saveLayout->addWidget(saveQualitySlider);
    saveLayout->addSpacing(2);
    saveLayout->addWidget(highQualityText);
    saveLayout->addSpacing(2);
    saveLayout->addStretch();
    m_saveLabel->setLayout(saveLayout);
    addWidget(m_saveLabel);
}

void SubToolBar::switchContent(QString shapeType) {
    if (shapeType == "rectangle" || shapeType == "oval") {
        setCurrentWidget(m_rectLabel);
        m_currentType = shapeType;
        emit shapeChanged();
    }   else if (shapeType == "arrow") {
        setCurrentWidget(m_arrowLabel);
        m_currentType = shapeType;
         emit shapeChanged();
    } else if (shapeType == "line") {
        setCurrentWidget(m_lineLabel);
        m_currentType = shapeType;
         emit shapeChanged();
    } else if (shapeType == "text") {
        setCurrentWidget(m_textLabel);
        m_currentType = shapeType;
         emit shapeChanged();
    } else if (shapeType == "color") {
        setCurrentWidget(m_colorLabel);
    } else if (shapeType == "saveList") {
        setCurrentWidget(m_saveLabel);
    }
    qDebug() << "subToolBar shape:" << shapeType;
}

void SubToolBar::setSaveOption(int saveOption) {
    ConfigSettings::instance()->setValue("save", "save_op", saveOption);
    emit saveAction();
}

SubToolBar::~SubToolBar() {}
