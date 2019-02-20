#include"optionalprojectdatapanel.hpp"
#include<QKeyEvent>
#include"windowmacros.hpp"
#include"innerwindowcp.hpp"
#include<QLineEdit>
#include<QLabel>
#include"innerwindowcpcatchevents.hpp"
#include"formatabledatafield.hpp"

OptionalProjectDataPanel::OptionalProjectDataPanel(InnerWindowCP *setParent){
    parent = setParent;
    init();
    setWindow();
    createWidgets();
    createLayout();
    connectAll();
}

OptionalProjectDataPanel::~OptionalProjectDataPanel(){
  disconnectAll();
  deleteWidgets();
  }

void OptionalProjectDataPanel::init(){
    setContextMenuPolicy(Qt::NoContextMenu);
}

void OptionalProjectDataPanel::setWindow(){
    setBaseSize(NEW_PROJECT_OPTIONAL_WIDTH, NEW_PROJECT_OPTIONAL_HEIGHT);
    setStyleSheet("QLabel{font-family: \"Arial\"; font-weight: normal; font-size: 12px;}");
}

void OptionalProjectDataPanel::createWidgets(){
    if(*(parent->getEditProjectData())){
        if(*(parent->getEditableData())){
            companyTextEdit = new FormatableDataField<ProjectMenagerParam>(true, nullptr, false, this, QString(), false);
            static_cast<QWidget*>(companyTextEdit)->setParent(this);
            companyTextEdit->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
            companyTextEdit->getDataUTextEdit()->setFrame(false);
            companyTextEdit->show();
            objectTextEdit = new FormatableDataField<ProjectMenagerParam>(true, nullptr, false, this, QString(), false);
            static_cast<QWidget*>(objectTextEdit)->setParent(this);
            objectTextEdit->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
            objectTextEdit->show();
            objectTextEdit->getDataUTextEdit()->setFrame(false);
            flangeTextEdit = new FormatableDataField<ProjectMenagerParam>(true, nullptr, false, this, QString(), false);
            static_cast<QWidget*>(flangeTextEdit)->setParent(this);
            flangeTextEdit->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
            flangeTextEdit->show();
            flangeTextEdit->getDataUTextEdit()->setFrame(false);
        }else{
            companyEditDataLabel = new QLabel(this);
            companyEditDataLabel->show();
            objectEditDataLabel = new QLabel(this);
            objectEditDataLabel->show();
            flangeEditDataLabel = new QLabel(this);
            flangeEditDataLabel->show();
        }
    infoLabel = new QLabel(this);
    static_cast<QWidget*>(infoLabel)->setParent(this);
    infoLabel->show();
    companyLabel = new QLabel(this);
    static_cast<QWidget*>(companyLabel)->setParent(this);
    companyLabel->show();
    objectLabel = new QLabel(this);
    static_cast<QWidget*>(objectLabel)->setParent(this);
    objectLabel->show();
    flangeLabel = new QLabel(this);
    static_cast<QWidget*>(flangeLabel)->setParent(this);
    flangeLabel->show();
    }else{
        infoLabel = new QLabel(this);
        static_cast<QWidget*>(infoLabel)->setParent(this);
        infoLabel->show();
        companyLabel = new QLabel(this);
        static_cast<QWidget*>(companyLabel)->setParent(this);
        companyLabel->show();
        objectLabel = new QLabel(this);
        static_cast<QWidget*>(objectLabel)->setParent(this);
        objectLabel->show();
        flangeLabel = new QLabel(this);
        static_cast<QWidget*>(flangeLabel)->setParent(this);
        flangeLabel->show();
        companyTextEdit = new FormatableDataField<ProjectMenagerParam>(true, nullptr, false, this, QString(), false);
        static_cast<QWidget*>(companyTextEdit)->setParent(this);
        companyTextEdit->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
        companyTextEdit->show();
        companyTextEdit->getDataUTextEdit()->setFrame(false);
        objectTextEdit = new FormatableDataField<ProjectMenagerParam>(true, nullptr, false, this, QString(), false);
        static_cast<QWidget*>(objectTextEdit)->setParent(this);
        objectTextEdit->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
        objectTextEdit->show();
        objectTextEdit->getDataUTextEdit()->setFrame(false);
        flangeTextEdit = new FormatableDataField<ProjectMenagerParam>(true, nullptr, false, this, QString(), false);
        static_cast<QWidget*>(flangeTextEdit)->setParent(this);
        flangeTextEdit->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
        flangeTextEdit->show();
        flangeTextEdit->getDataUTextEdit()->setFrame(false);
    }
}

void OptionalProjectDataPanel::createLayout(){
    if(*(parent->getEditProjectData())){
        if(*(parent->getEditableData())){
            companyTextEdit->setGeometry( NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH + NEW_PROJECT_OPTIONAL_MID_OFFSET, NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
            companyTextEdit->getDataUTextEdit()->setGeometry(0,0, NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
            objectTextEdit->setGeometry( NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH + NEW_PROJECT_OPTIONAL_MID_OFFSET, 2 * NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + 2 * NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
            objectTextEdit->getDataUTextEdit()->setGeometry(0,0,NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
            flangeTextEdit->setGeometry(NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH + NEW_PROJECT_OPTIONAL_MID_OFFSET, 3 * NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + 3 * NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
            flangeTextEdit->getDataUTextEdit()->setGeometry(0,0,NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
        }else{
            companyEditDataLabel->setGeometry( NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH + NEW_PROJECT_OPTIONAL_MID_OFFSET, NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
            objectEditDataLabel->setGeometry( NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH + NEW_PROJECT_OPTIONAL_MID_OFFSET, 2 * NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + 2 * NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
            flangeEditDataLabel->setGeometry(NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH + NEW_PROJECT_OPTIONAL_MID_OFFSET, 3 * NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + 3 * NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
        }
        infoLabel->setText(NEW_PROJECT_OPTIONAL_TEXT);
        infoLabel->setGeometry(0,0, NEW_PROJECT_OPTIONAL_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
        infoLabel->setAlignment(Qt::AlignCenter);
        infoLabel->setStyleSheet("font-family: \"Comic Sans MS\"; font-weight: normal; font-size: 16px;");
        companyLabel->setText(NEW_PROJECT_OPTIONAL_COMPANY_TEXT);
        companyLabel->setGeometry(0,NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT + NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET, NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
        objectLabel->setText(NEW_PROJECT_OPTIONAL_OBJECT_TEXT);
        objectLabel->setGeometry(0, 2 * NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + 2*NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_LEFT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
        flangeLabel->setText(NEW_PROJECT_OPTIONAL_FLANGE_TEXT);
        flangeLabel->setGeometry(0, 3 * NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + 3 * NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
    }else{
    infoLabel->setText(NEW_PROJECT_OPTIONAL_TEXT);
    infoLabel->setGeometry(0,0, NEW_PROJECT_OPTIONAL_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-family: \"Comic Sans MS\"; font-weight: normal; font-size: 16px;");
    companyLabel->setText(NEW_PROJECT_OPTIONAL_COMPANY_TEXT);
    companyLabel->setGeometry(0,NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT + NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET, NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
    objectLabel->setText(NEW_PROJECT_OPTIONAL_OBJECT_TEXT);
    objectLabel->setGeometry(0, 2 * NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + 2*NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_LEFT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
    flangeLabel->setText(NEW_PROJECT_OPTIONAL_FLANGE_TEXT);
    flangeLabel->setGeometry(0, 3 * NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + 3 * NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
    companyTextEdit->setGeometry( NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH + NEW_PROJECT_OPTIONAL_MID_OFFSET, NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
    companyTextEdit->getDataUTextEdit()->setGeometry(0,0, NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
    objectTextEdit->setGeometry( NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH + NEW_PROJECT_OPTIONAL_MID_OFFSET, 2 * NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + 2 * NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
    objectTextEdit->getDataUTextEdit()->setGeometry(0,0,NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
    flangeTextEdit->setGeometry(NEW_PROJECT_OPTIONAL_LEFT_ELEMENT_WIDTH + NEW_PROJECT_OPTIONAL_MID_OFFSET, 3 * NEW_PROJECT_OPTIONAL_VERTICAL_OFFSET + 3 * NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT, NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
    flangeTextEdit->getDataUTextEdit()->setGeometry(0,0,NEW_PROJECT_OPTIONAL_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_OPTIONAL_ELEMENTS_HEIGHT);
    }
}

void OptionalProjectDataPanel::deleteWidgets(){
    if(*(parent->getEditProjectData())){
        if(!*(parent->getEditableData())){
            delete companyTextEdit;
            companyTextEdit = nullptr;
            delete objectTextEdit;
            objectTextEdit = nullptr;
            delete flangeTextEdit;
            flangeTextEdit = nullptr;
        }else{
            delete companyEditDataLabel;
            companyEditDataLabel = nullptr;
            delete objectEditDataLabel;
            objectEditDataLabel = nullptr;
            delete flangeEditDataLabel;
            flangeEditDataLabel = nullptr;
        }
        delete infoLabel;
        infoLabel = nullptr;
        delete companyLabel;
        companyLabel = nullptr;
        delete objectLabel;
        objectLabel = nullptr;
        delete flangeLabel;
        flangeLabel = nullptr;
    }else{
    delete infoLabel;
    infoLabel = nullptr;
    delete companyLabel;
    companyLabel = nullptr;
    delete objectLabel;
    objectLabel = nullptr;
    delete flangeLabel;
    flangeLabel = nullptr;
    delete companyTextEdit;
    companyTextEdit = nullptr;
    delete objectTextEdit;
    objectTextEdit = nullptr;
    delete flangeTextEdit;
    flangeTextEdit = nullptr;
    }
}

void OptionalProjectDataPanel::connectAll(){
    if(*(parent->getEditProjectData())){
        if(*(parent->getEditableData())){
            companyTextEdit->installEventFilter(parent->getInnerWindowCPCatchEvents());
            objectTextEdit->installEventFilter(parent->getInnerWindowCPCatchEvents());
            flangeTextEdit->installEventFilter(parent->getInnerWindowCPCatchEvents());
        }
    }else{
        companyTextEdit->installEventFilter(parent->getInnerWindowCPCatchEvents());
        objectTextEdit->installEventFilter(parent->getInnerWindowCPCatchEvents());
        flangeTextEdit->installEventFilter(parent->getInnerWindowCPCatchEvents());
    }
}

void OptionalProjectDataPanel::disconnectAll(){

}

QLabel* OptionalProjectDataPanel::getCompanyLabel(){
    return companyLabel;
}

QLabel* OptionalProjectDataPanel::getFlangeLabel(){
    return flangeLabel;
}

QLabel* OptionalProjectDataPanel::getObjectLabel(){
    return objectLabel;
}

FormatableDataField<ProjectMenagerParam>* OptionalProjectDataPanel::getCompanyTextEdit(){
  return companyTextEdit;
}

FormatableDataField<ProjectMenagerParam>* OptionalProjectDataPanel::getObjectTextEdit(){
  return objectTextEdit;
}

FormatableDataField<ProjectMenagerParam>* OptionalProjectDataPanel::getFlangeTextEdit(){
  return flangeTextEdit;
}

bool OptionalProjectDataPanel::checkCompanyTextEdit(QKeyEvent* ev){
    uint len = companyTextEdit->getDataUTextEdit()->text().length();
    if(len > 30){
        switch(ev->key()){
        case Qt::Key_Left:
            return false;
        case Qt::Key_Right:
            return false;
        case Qt::Key_Backspace:
            companyTextEdit->getDataUTextEdit()->backspace();
            break;
        case Qt::Key_Delete:
            companyTextEdit->getDataUTextEdit()->del();
            break;
        default:
            return true;
        }
    }else{
    switch(ev->key()){
    case Qt::Key_Enter:
        return true;
    case Qt::Key_Tab:
        return true;
    case Qt::Key_Slash:
        return true;
    case Qt::Key_Backslash:
        return true;
    case Qt::Key_Return:
        return true;
    case Qt::Key_Colon:
        return true;
    case Qt::Key_Asterisk:
        return true;
    case Qt::Key_Less:
        return true;
    case Qt::Key_Greater:
        return true;
    case Qt::Key_QuoteDbl:
        return true;
    case Qt::Key_Question:
        return true;
    case Qt::Key_Bar:
        return true;
    case Qt::Key_Space:
        companyTextEdit->getDataUTextEdit()->insert(ev->text());
        break;
    case Qt::Key_Backspace:
        companyTextEdit->getDataUTextEdit()->backspace();
        break;
    case Qt::Key_Delete:
        companyTextEdit->getDataUTextEdit()->del();
        break;
    default:
        if(ev->count())
            companyTextEdit->getDataUTextEdit()->insert(ev->text());
        else
            return false;
     }
  }
    companyTextEdit->multiSpaceKeyDetection();
return true;
}

bool OptionalProjectDataPanel::checkFlangeTextEdit(QKeyEvent* ev){
    uint len = flangeTextEdit->getDataUTextEdit()->text().length();
    if(len > 30){
        switch(ev->key()){
        case Qt::Key_Left:
            return false;
        case Qt::Key_Right:
            return false;
        case Qt::Key_Backspace:
            flangeTextEdit->getDataUTextEdit()->backspace();
            break;
        case Qt::Key_Delete:
            flangeTextEdit->getDataUTextEdit()->del();
            break;
        default:
            return true;
        }
    }else{
        switch(ev->key()){
    case Qt::Key_Enter:
        return true;
    case Qt::Key_Tab:
        return true;
    case Qt::Key_Slash:
        return true;
    case Qt::Key_Backslash:
        return true;
    case Qt::Key_Return:
        return true;
    case Qt::Key_Colon:
        return true;
    case Qt::Key_Asterisk:
        return true;
    case Qt::Key_Less:
        return true;
    case Qt::Key_Greater:
        return true;
    case Qt::Key_QuoteDbl:
        return true;
    case Qt::Key_Question:
        return true;
    case Qt::Key_Bar:
        return true;
        case Qt::Key_Space:
            flangeTextEdit->getDataUTextEdit()->insert(ev->text());
            break;
        case Qt::Key_Backspace:
            flangeTextEdit->getDataUTextEdit()->backspace();
            break;
        case Qt::Key_Delete:
            flangeTextEdit->getDataUTextEdit()->del();
            break;
        default:
            if(ev->count())
                flangeTextEdit->getDataUTextEdit()->insert(ev->text());
            else
                return false;
         }
    }
    flangeTextEdit->multiSpaceKeyDetection();
return true;
}

bool OptionalProjectDataPanel::checkObjectTextEdit(QKeyEvent* ev){
    uint len = objectTextEdit->getDataUTextEdit()->text().length();
    if(len > 30){
        switch(ev->key()){
        case Qt::Key_Left:
            return false;
        case Qt::Key_Right:
            return false;
        case Qt::Key_Backspace:
            objectTextEdit->getDataUTextEdit()->backspace();
            break;
        case Qt::Key_Delete:
            objectTextEdit->getDataUTextEdit()->del();
            break;
        default:
            return true;
        }
    }    else{
    switch(ev->key()){
    case Qt::Key_Enter:
        return true;
    case Qt::Key_Tab:
        return true;
    case Qt::Key_Slash:
        return true;
    case Qt::Key_Backslash:
        return true;
    case Qt::Key_Return:
        return true;
    case Qt::Key_Colon:
        return true;
    case Qt::Key_Asterisk:
        return true;
    case Qt::Key_Less:
        return true;
    case Qt::Key_Greater:
        return true;
    case Qt::Key_QuoteDbl:
        return true;
    case Qt::Key_Question:
        return true;
    case Qt::Key_Bar:
        return true;
    case Qt::Key_Space:
        objectTextEdit->getDataUTextEdit()->insert(ev->text());
        break;
    case Qt::Key_Backspace:
        objectTextEdit->getDataUTextEdit()->backspace();
        break;
    case Qt::Key_Delete:
        objectTextEdit->getDataUTextEdit()->del();
        break;
    default:
        if(ev->count())
            objectTextEdit->getDataUTextEdit()->insert(ev->text());
        else
            return false;
     }
}
objectTextEdit->multiSpaceKeyDetection();
return false;
}

void OptionalProjectDataPanel::setCompanyEditDataLabel(QString set){
    companyEditDataLabel->setText(set);
}

void OptionalProjectDataPanel::setFlangeEditDataLabel(QString set){
    flangeEditDataLabel->setText(set);
}

void OptionalProjectDataPanel::setObjectEditDataLabel(QString set){
    objectEditDataLabel->setText(set);
}

void OptionalProjectDataPanel::setCompanyTextEdit(QString set){
    companyTextEdit->getDataUTextEdit()->setText(set);
}

void OptionalProjectDataPanel::setObjectTextEdit(QString set){
    objectTextEdit->getDataUTextEdit()->setText(set);
}

void OptionalProjectDataPanel::setFlangeTextEdit(QString set){
    flangeTextEdit->getDataUTextEdit()->setText(set);
}
