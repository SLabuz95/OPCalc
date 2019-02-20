#include"panelcep.hpp"
#include"panelcepcatchevents.hpp"
#include"windowmacros.hpp"
#include"panelcepelementflangetype.hpp"
#include"appwindow.hpp"
#include"innerwindowcep.hpp"

PanelCEP::PanelCEP(AppWindow *setParent){
  parent = setParent;
  init();
  setWindow();
  createWidgets();
  createLayout();
  connectAll();
}

PanelCEP::~PanelCEP(){
  disconnectAll();
  deleteWidgets();
  delete catchEvents;
  catchEvents = nullptr;
}

void PanelCEP::init(){
  catchEvents = new PanelCEPCatchEvents(this);
}

void PanelCEP::setWindow(){
    setBaseSize(PANEL_WIDTH, PANEL_HEIGHT * 6);
    setStyleSheet("QLabel{font-family: \"Comic Sans MS\"; font-size: 12px;}");
}

void PanelCEP::createWidgets(){
  panelCEPElement = new PanelCEPElement*[6];
  for(uint i = 0; i < 6; i++)
      (*(panelCEPElement + i)) = new PanelCEPElement(this, i);
}

void PanelCEP::createLayout(){
  for(uint i = 0; i < 6; i++){
      (*(panelCEPElement + i))->setStyleSheet(QString("PanelCEPElement{border: 1px solid rgba(255, 255, 255, 0.4);") + ((parent->getInnerWinCEP() != nullptr && i == *parent->getInnerWinCEP()->getInnerWinStatusCEP())? QString("background-color: qlineargradient(x1:0 , y1: 0.5, x2: 1, y2: 0.5, stop: 0 rgba(200,200,200,0.77), stop: 0.93 rgba(247,247,247,0.8), stop: 0.99 rgba(247,247,247,0.8));") : QString("background-color: rgba(255, 255, 255, 0.4);")) + ((i == 0)? QString("border-top-right-radius: 20px 15px;") : QString("")) +  ((i == 5)? QString("border-bottom-right-radius: 20px 15px;") : QString("")) + "}");
      (*(panelCEPElement + i))->setGeometry( 0, i * PANEL_HEIGHT, PANEL_WIDTH, PANEL_HEIGHT);
      (*(panelCEPElement + i))->show();
    }
}

void PanelCEP::deleteWidgets(){
  for(uint i = 0; i < 6 ; i++)
      delete (*(panelCEPElement + i));
  delete []panelCEPElement;
  panelCEPElement = nullptr;
}

void PanelCEP::connectAll(){
    for(uint i = 0; i < 6; i++){
        if(i != 1)
            (*(panelCEPElement + i))->installEventFilter(catchEvents);
        else
            (*(panelCEPElement + i))->installEventFilter(catchEvents);
    }
}

void PanelCEP::disconnectAll(){
  //Brak
}

AppWindow* PanelCEP::getParent(){
    return parent;
}

PanelCEPElement** PanelCEP::getPanelCEPElement(){
    return panelCEPElement;
}

PanelCEPCatchEvents* PanelCEP::getCatchEvents(){
    return catchEvents;
}

void PanelCEP::saveInnerWinCEPData(){
    switch(*parent->getInnerWinCEP()->getInnerWinStatusCEP()){
    case (uint)FLANGE:
        parent->getInnerWinCEP()->saveFromFlangeData();
        break;
    case (uint)PIPE:
        parent->getInnerWinCEP()->saveFromPipeData();
        break;
    case (uint)FLOW:
        parent->getInnerWinCEP()->saveFromFlowData();
        break;
    case (uint)FLUID:
        parent->getInnerWinCEP()->saveFromFluidData();
        break;
    default:
        break;
    }
}

void PanelCEP::reload(){
    disconnectAll();
    deleteWidgets();
    createWidgets();
    createLayout();
    connectAll();
}
