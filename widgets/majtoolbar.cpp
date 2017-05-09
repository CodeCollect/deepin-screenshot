#include "majtoolbar.h"
#include "utils/baseutils.h"
#include "utils/configsettings.h"
#include "bigcolorbutton.h"
#include "toolbutton.h"
#include "savebutton.h"

#include <QApplication>
#include <QButtonGroup>
#include <QDebug>
#include <QCursor>

namespace {
    const int TOOLBAR_HEIGHT = 28;
    const int TOOLBAR_WIDTH = 284;
    const int BTN_SPACING = 6;
}
MajToolBar::MajToolBar(QWidget *parent)
    : QLabel(parent),
      m_isChecked(false),
      m_currentShape("")
{
    initWidgets();
}

MajToolBar::~MajToolBar() {}

void MajToolBar::initWidgets() {
    setStyleSheet(getFileContent(":/resources/qss/majtoolbar.qss"));
    setFixedSize(TOOLBAR_WIDTH, TOOLBAR_HEIGHT);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setAcceptDrops(true);
    installEventFilter(this);

    QList<ToolButton*> toolBtnList;
    QButtonGroup* buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);
    ToolButton* rectBtn = new ToolButton();
    rectBtn->setObjectName("RectBtn");
    toolBtnList.append(rectBtn);
    ToolButton* ovalBtn = new ToolButton();
    ovalBtn->setObjectName("OvalBtn");
    toolBtnList.append(ovalBtn);
    ToolButton* arrowBtn = new ToolButton();
    arrowBtn->setObjectName("ArrowBtn");
    toolBtnList.append(arrowBtn);
    ToolButton* lineBtn = new ToolButton();
    lineBtn->setObjectName("PenBtn");
    toolBtnList.append(lineBtn);
    ToolButton* textBtn = new ToolButton();
    textBtn->setObjectName("TextBtn");
    toolBtnList.append(textBtn);


    BigColorButton* colorBtn = new BigColorButton();
    colorBtn->setObjectName("ColorBtn");
    SaveButton* saveBtn = new SaveButton();
//    ToolButton* shareBtn = new ToolButton();
//    shareBtn->setObjectName("ShareBtn");
    ToolButton* closeBtn = new ToolButton();
    closeBtn->setObjectName("CloseBtn");

    m_baseLayout = new QHBoxLayout();
    m_baseLayout->setMargin(0);
    m_baseLayout->addSpacing(BTN_SPACING);
    for (int k = 0; k < toolBtnList.length(); k++) {
        m_baseLayout->addWidget(toolBtnList[k]);
        m_baseLayout->addSpacing(BTN_SPACING);
        buttonGroup->addButton(toolBtnList[k]);
    }
    m_baseLayout->addWidget(colorBtn);
    m_baseLayout->addSpacing(BTN_SPACING);
    m_baseLayout->addWidget(saveBtn);
//    m_baseLayout->addWidget(shareBtn);
    m_baseLayout->addSpacing(BTN_SPACING);
    m_baseLayout->addWidget(closeBtn);
    m_baseLayout->addSpacing(4);
    m_baseLayout->addStretch();

    setLayout(m_baseLayout);

    connect(rectBtn, &ToolButton::clicked, this, [=](){
        if (m_currentShape != "rectangle") {
            m_currentShape = "rectangle";
            m_isChecked = true;
            int rectColorIndex = ConfigSettings::instance()->value("rectangle", "color_index").toInt();
            ConfigSettings::instance()->setValue("common", "color_index", rectColorIndex);

        } else {
            m_currentShape = "";
            m_isChecked = false;
        }
        rectBtn->setChecked(m_isChecked);
        rectBtn->update();
        emit buttonChecked(m_isChecked, "rectangle");
    });
    connect(ovalBtn, &ToolButton::clicked, this, [=](){
        if (m_currentShape != "oval") {
            m_currentShape = "oval";
            m_isChecked = true;
            int ovalColorIndex = ConfigSettings::instance()->value("oval", "color_index").toInt();
            ConfigSettings::instance()->setValue("common", "color_index", ovalColorIndex);
        } else {
            m_currentShape = "";
            m_isChecked = false;
        }
        ovalBtn->setChecked(m_isChecked);
        emit buttonChecked(m_isChecked, "oval");
    });
    connect(arrowBtn, &ToolButton::clicked, this, [=](){
        if (m_currentShape != "arrow") {
            m_currentShape = "arrow";
            m_isChecked = true;
            int rectColorIndex = ConfigSettings::instance()->value("arrow", "color_index").toInt();
            ConfigSettings::instance()->setValue("common", "color_index", rectColorIndex);
        } else {
            m_currentShape = "";
            m_isChecked = false;
        }
        arrowBtn->setChecked(m_isChecked);
        emit buttonChecked(m_isChecked, "arrow");
    });
    connect(lineBtn, &ToolButton::clicked, this, [=](){
        if (m_currentShape != "line") {
            m_currentShape = "line";
            m_isChecked = true;
            int rectColorIndex = ConfigSettings::instance()->value("line", "color_index").toInt();
            ConfigSettings::instance()->setValue("common", "color_index", rectColorIndex);
        } else {
            m_currentShape = "";
            m_isChecked = false;
        }
        lineBtn->setChecked(m_isChecked);
        emit buttonChecked(m_isChecked, "line");
    });
    connect(textBtn, &ToolButton::clicked, this, [=](){
        if (m_currentShape != "text") {
            m_currentShape = "text";
            m_isChecked = true;
            int rectColorIndex = ConfigSettings::instance()->value("text", "color_index").toInt();
            ConfigSettings::instance()->setValue("common", "color_index", rectColorIndex);
        } else {
            m_currentShape = "";
            m_isChecked = false;
        }
        textBtn->setChecked(m_isChecked);
        emit buttonChecked(m_isChecked, "text");
    });
    connect(colorBtn, &BigColorButton::clicked, this, [=]{
        colorBtn->setChecked(true);
        emit buttonChecked(true, "color");
    });
    connect(ConfigSettings::instance(), &ConfigSettings::colorChanged,
            colorBtn, &BigColorButton::setColorIndex);

    connect(this, &MajToolBar::mainColorChanged, colorBtn, &BigColorButton::setColor);
    connect(saveBtn, &SaveButton::saveAction, this, [=](){
        emit saveImage();
    });
    connect(saveBtn, &SaveButton::expandSaveOption, this, [=](bool expand){
        emit buttonChecked(expand, "saveList");
    });

    connect(closeBtn, &ToolButton::clicked, this, [=](bool checked){
        Q_UNUSED(checked);
        qDebug() << "screenshot will exit!";
        qApp->quit();
    });
}

bool MajToolBar::isButtonChecked() {
    return m_isChecked;
}

bool MajToolBar::eventFilter(QObject *watched, QEvent *event) {
    Q_UNUSED(watched);

    if (event->type() == QEvent::Enter) {
        setCursor(Qt::ArrowCursor);
        qApp->setOverrideCursor(Qt::ArrowCursor);
    }
    return false;
}

void MajToolBar::mouseMoveEvent(QMouseEvent *ev) {
    Q_UNUSED(ev);
    qApp->setOverrideCursor(Qt::ArrowCursor);
}