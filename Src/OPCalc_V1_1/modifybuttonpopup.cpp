#include"modifybuttonpopup.hpp"
#include<QLabel>
#include"windowmacros.hpp"
#include"mainmacros.hpp"
#include"innerwindowcep.hpp"
#include"innerwindowcepcatchevents.hpp"

ModifyButtonPopUp::ModifyButtonPopUp(InnerWindowCEP *setParent, MaterialsChangesWidgetType setType){
    parent = setParent;
    type = setType;
    init();
    setWindow();
    createWidgets();
    createLayout();
    connectAll();
}

ModifyButtonPopUp::~ModifyButtonPopUp(){
    deleteWidgets();
}

void ModifyButtonPopUp::init(){
    // Brak
}

void ModifyButtonPopUp::setWindow(){
    setObjectName("popup");
    setStyleSheet("QFrame#popup{background-color: rgba(240, 240, 240, 0.6);}");
}

void ModifyButtonPopUp::createWidgets(){
    addButton = new QLabel(this);
    addButton->show();
    removeButton = new QLabel(this);
    removeButton->show();
    editButton = new QLabel(this);
    editButton->show();
}

void ModifyButtonPopUp::createLayout(){
    addButton->setGeometry(0, MODIFY_BUTTON_POPUP_OFFSET, MODIFY_BUTTON_POPUP_BUTTON_WIDTH, MODIFY_BUTTON_POPUP_BUTTON_HEIGHT);
    addButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
    removeButton->setGeometry(0 , 2 * MODIFY_BUTTON_POPUP_OFFSET + MODIFY_BUTTON_POPUP_BUTTON_HEIGHT, MODIFY_BUTTON_POPUP_BUTTON_WIDTH, MODIFY_BUTTON_POPUP_BUTTON_HEIGHT);
    removeButton->setPixmap(QPixmap(REMOVE_BUTTON_ICON));
    editButton->setGeometry(0 , 3 * MODIFY_BUTTON_POPUP_OFFSET + 2 * MODIFY_BUTTON_POPUP_BUTTON_HEIGHT, MODIFY_BUTTON_POPUP_BUTTON_WIDTH, MODIFY_BUTTON_POPUP_BUTTON_HEIGHT);
    editButton->setPixmap(QPixmap(EDIT_BUTTON_ICON));
}

void ModifyButtonPopUp::deleteWidgets(){
    delete addButton;
    addButton = nullptr;
    delete removeButton;
    removeButton = nullptr;
    delete editButton;
    editButton = nullptr;
}

void ModifyButtonPopUp::connectAll(){
    addButton->installEventFilter(parent->getInnerWindowCEPCatchEvents());
    removeButton->installEventFilter(parent->getInnerWindowCEPCatchEvents());
    editButton->installEventFilter(parent->getInnerWindowCEPCatchEvents());
}

void ModifyButtonPopUp::disconnectAll(){
    // Brak
}

QLabel* ModifyButtonPopUp::getAddButton(){
    return addButton;
}

QLabel* ModifyButtonPopUp::getRemoveButton(){
    return removeButton;
}

QLabel* ModifyButtonPopUp::getEditButton(){
    return editButton;
}

void ModifyButtonPopUp::addButtonReleased(){
    parent->createMaterialChangesWidget(ADD, type);

}

void ModifyButtonPopUp::removeButtonReleased(){
    parent->createMaterialChangesWidget(REMOVE, type);
}

void ModifyButtonPopUp::editButtonReleased(){
    parent->createMaterialChangesWidget(EDIT, type);
}
