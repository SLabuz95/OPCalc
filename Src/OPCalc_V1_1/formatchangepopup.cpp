#include"formatchangepopup.hpp"
#include"windowmacros.hpp"
#include"formatabledata.hpp"
#include<QLabel>
#include"formatchangepopupcatchevents.hpp"
#include<QComboBox>
#include<QAbstractItemView>
#include<QLineEdit>
#include"formatabledatafield.hpp"
#include<QPainter>
#include<QBrush>
#include<QLinearGradient>
#include<QPointF>

template <typename ParamName>
FormatChangePopUp<ParamName>::FormatChangePopUp(FormatableDataField<ParamName> *element){
    this->modifiedElement = element;
    init();
    setWindow();
    createWidgets();
    createLayout();
    connectAll();
}

template <typename ParamName>
FormatChangePopUp<ParamName>::~FormatChangePopUp(){
    disconnectAll();
    deleteWidgets();
}

template <typename ParamName>
void FormatChangePopUp<ParamName>::init(){
    catchEvents = new FormatChangePopUpCatchEvents<ParamName>(this);
    setWindowModality(Qt::ApplicationModal);
}

template <typename ParamName>
void FormatChangePopUp<ParamName>::setWindow(){
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setMinimumSize(FORMAT_CHANGE_POPUP_WIDTH, FORMAT_CHANGE_POPUP_HEIGHT);
    setMaximumSize(FORMAT_CHANGE_POPUP_WIDTH, FORMAT_CHANGE_POPUP_HEIGHT);

}

template <typename ParamName>
void FormatChangePopUp<ParamName>::createWidgets(){
    closeButton = new QLabel(this);
    precisionLabel = new QLabel(this);
    formatLabel = new QLabel(this);
    decrementButton = new QLabel(this);
    incrementButton = new QLabel(this);
    precisionDisplayLabel = new QLabel(this);
    formatList = new QComboBox(this);
}

template <typename ParamName>
void FormatChangePopUp<ParamName>::createLayout(){
    QString* format = FormatableData<ParamName>::getFormatFromString((modifiedElement->isEditable())? modifiedElement->getDataUTextEdit()->text() : modifiedElement->getDataULabel()->text());
    closeButton->setPixmap(QPixmap(CLOSE_BUTTON_ICON));
    closeButton->setGeometry(FORMAT_CHANGE_POPUP_WIDTH - 20, 0, 20, 20);
    closeButton->setAlignment(Qt::AlignCenter);
    closeButton->show();
    precisionLabel->setGeometry(FORMAT_CHANGE_POPUP_HORIZONTAL_OFFSET, FORMAT_CHANGE_POPUP_VERTICAL_OFFSET, FORMAT_CHANGE_POPUP_INFO_LABEL_WIDTH, FORMAT_CHANGE_POPUP_LABEL_HEIGHT);
    precisionLabel->setText("Miejsca po przecinku:");
    precisionLabel->show();
    formatLabel->setGeometry(FORMAT_CHANGE_POPUP_HORIZONTAL_OFFSET, FORMAT_CHANGE_POPUP_LABEL_HEIGHT + 2 * FORMAT_CHANGE_POPUP_VERTICAL_OFFSET, FORMAT_CHANGE_POPUP_INFO_LABEL_WIDTH, FORMAT_CHANGE_POPUP_LABEL_HEIGHT);
    formatLabel->setText("Format wyświetlania:");
    formatLabel->show();
    decrementButton->setGeometry(2 * FORMAT_CHANGE_POPUP_HORIZONTAL_OFFSET + FORMAT_CHANGE_POPUP_INFO_LABEL_WIDTH, FORMAT_CHANGE_POPUP_VERTICAL_OFFSET, FORMAT_CHANGE_POPUP_BUTTON_WIDTH, FORMAT_CHANGE_POPUP_LABEL_HEIGHT);
    decrementButton->setAlignment(Qt::AlignCenter);
    decrementButton->setPixmap(QPixmap(REMOVE_BUTTON_ICON));
    decrementButton->show();
    incrementButton->setGeometry(2 * FORMAT_CHANGE_POPUP_HORIZONTAL_OFFSET + FORMAT_CHANGE_POPUP_INFO_LABEL_WIDTH + FORMAT_CHANGE_POPUP_BUTTON_WIDTH + FORMAT_CHANGE_POPUP_PRECISION_DISPLAY_LABEL_WIDTH, FORMAT_CHANGE_POPUP_VERTICAL_OFFSET, FORMAT_CHANGE_POPUP_BUTTON_WIDTH, FORMAT_CHANGE_POPUP_LABEL_HEIGHT);
    incrementButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
    incrementButton->setAlignment(Qt::AlignCenter);
    incrementButton->show();
    precisionDisplayLabel->setGeometry(2 * FORMAT_CHANGE_POPUP_HORIZONTAL_OFFSET + FORMAT_CHANGE_POPUP_INFO_LABEL_WIDTH + FORMAT_CHANGE_POPUP_BUTTON_WIDTH, FORMAT_CHANGE_POPUP_VERTICAL_OFFSET, FORMAT_CHANGE_POPUP_PRECISION_DISPLAY_LABEL_WIDTH, FORMAT_CHANGE_POPUP_LABEL_HEIGHT);
    precisionDisplayLabel->setText(QString::number(format->right(format->length() - 1).toInt()));
    precisionDisplayLabel->setAlignment(Qt::AlignCenter);
    precisionDisplayLabel->show();
    formatList->setGeometry(2 * FORMAT_CHANGE_POPUP_HORIZONTAL_OFFSET + FORMAT_CHANGE_POPUP_INFO_LABEL_WIDTH, 2 * FORMAT_CHANGE_POPUP_VERTICAL_OFFSET + FORMAT_CHANGE_POPUP_LABEL_HEIGHT, FORMAT_CHANGE_POPUP_FORMAT_LIST_WIDTH, FORMAT_CHANGE_POPUP_LABEL_HEIGHT);
    formatList->addItem("0.1e-2");
    formatList->addItem("0.001");
    formatList->setCurrentIndex((format->at(0)=='e')? 0 : 1);
    formatList->show();
    delete format;
}

template <typename ParamName>
void FormatChangePopUp<ParamName>::deleteWidgets(){
    delete closeButton;
    closeButton = nullptr;
    delete precisionLabel;
    precisionLabel = nullptr;
    delete formatLabel;
    formatLabel = nullptr;
    delete decrementButton;
    decrementButton = nullptr;
    delete incrementButton;
    incrementButton = nullptr;
    delete precisionDisplayLabel;
    precisionDisplayLabel = nullptr;
    delete formatList;
    formatList = nullptr;
    delete catchEvents;
    catchEvents = nullptr;
}

template <typename ParamName>
void FormatChangePopUp<ParamName>::connectAll(){
    closeButton->installEventFilter(catchEvents);
    decrementButton->installEventFilter(catchEvents);
    incrementButton->installEventFilter(catchEvents);
    formatList->view()->installEventFilter(catchEvents);
}

template <typename ParamName>
void FormatChangePopUp<ParamName>::disconnectAll(){

}

template <typename ParamName>
QLabel* FormatChangePopUp<ParamName>::getCloseButton(){
    return closeButton;
}

template <typename ParamName>
QLabel* FormatChangePopUp<ParamName>::getDecrementButton(){
    return decrementButton;
}

template <typename ParamName>
QLabel* FormatChangePopUp<ParamName>::getIncrementButton(){
    return incrementButton;
}

template <typename ParamName>
QLabel* FormatChangePopUp<ParamName>::getPrecisionDisplayLabel(){
    return precisionDisplayLabel;
}

template <typename ParamName>
QComboBox* FormatChangePopUp<ParamName>::getFormatList(){
    return formatList;
}

template <typename ParamName>
FormatChangePopUpCatchEvents<ParamName>* FormatChangePopUp<ParamName>::getFormatChangePopUpCatchEvents(){
    return catchEvents;
}

template <typename ParamName>
void FormatChangePopUp<ParamName>::decrement(){
    if(precisionDisplayLabel->text().toUShort() > 0)
    {
        modifiedElement->getFormatableData()->decrementPrecision();
        precisionDisplayLabel->setText(QString::number(precisionDisplayLabel->text().toUShort() - 1));
        formatChanged();
    }
}

template <typename ParamName>
void FormatChangePopUp<ParamName>::increment(){
    if(precisionDisplayLabel->text().toUShort() < UINT8_MAX)
    {
        modifiedElement->getFormatableData()->incrementPrecision();
        precisionDisplayLabel->setText(QString::number(precisionDisplayLabel->text().toUShort() + 1));
        formatChanged();
    }
}

template <typename ParamName>
void FormatChangePopUp<ParamName>::formatChanged(){
    modifiedElement->rewriteData(true);
}

template <typename ParamName>
void FormatChangePopUp<ParamName>::changeFormatType(){
    switch(formatList->currentIndex()){
    case 0:
        modifiedElement->getFormatableData()->changeFormatType('e');
        break;
    case 1:
        modifiedElement->getFormatableData()->changeFormatType('f');
        break;
    default:
        break;
    }
    formatChanged();
}


template <typename ParamName>
void FormatChangePopUp<ParamName>::paintEvent(QPaintEvent* ev){
    QPainter pa(this);
    // Background
    QPen p;
    QLinearGradient lg(QPointF(0, height()), QPointF(width(),0));
    lg.setColorAt(0, QColor(235, 235, 235, 255));
    lg.setColorAt(0.63, QColor(200,200,200,255));
    lg.setColorAt(1 , QColor(210,210,210,255));
    QBrush b(lg);
    p.setWidth(0);
    p.setColor(Qt::transparent);
    pa.setPen(p);
    pa.setBrush(b);
    pa.drawRect(0,0, width(), height());

    // Border
    lg.setColorAt(0, QColor(245, 245, 245, 255));
    lg.setColorAt(0.63, QColor(210,210, 210, 255));
    lg.setColorAt(1, QColor(220, 220, 220, 255));
    //QBrush b2(lg);
    p.setWidth(2);
    p.setBrush(b);
    p.setColor(Qt::gray);
    p.setStyle(Qt::SolidLine);
    pa.setPen(p);
    pa.setBrush(Qt::NoBrush);
    pa.drawRoundRect(0,0, width(), height(), 3, 3);
    QWidget::paintEvent(ev);

    // Background

}

template class FormatChangePopUp<ProjectMenagerParam>;
template class FormatChangePopUp<FlangeMaterialsFileParams>;
template class FormatChangePopUp<PipeMaterialsFileParams>;
template class FormatChangePopUp<SubstancesParamsFileParam>;
