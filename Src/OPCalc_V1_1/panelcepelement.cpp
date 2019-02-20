#include "panelcepelementflangetype.hpp"
#include"panelcep.hpp"
#include"windowmacros.hpp"
#include<QLabel>
#include"appwindow.hpp"
#include"innerwindowcep.hpp"
#include"projectinfo.hpp"

PanelCEPElement::PanelCEPElement(PanelCEP *setParent, uint numb){
  parent = setParent;
  numberOfPanel = (InnerWinStatusCEP)numb;
  init();
  setWindow();
  createWidgets();
  createLayout();
  connectAll();
}

void PanelCEPElement::init(){
   setParent(parent);
}

void PanelCEPElement::setWindow(){

}

void PanelCEPElement::createWidgets(){
  infoLabel = new QLabel(this);
  infoLabel->show();
}

void PanelCEPElement::createLayout(){
  infoLabel->setGeometry(PANEL_LABEL_X, PANEL_LABEL_Y, PANEL_LABEL_WIDTH, PANEL_LABEL_HEIGHT);
  switch((uint)numberOfPanel){
    case 0:
      infoLabel->setText(CHOOSE_PROJECT_PANEL_TEXT);
      break;
    case 1:
      infoLabel->setText(FLANGE_PANEL_TEXT);
      break;
    case 2:
      infoLabel->setText(PIPE_PANEL_TEXT);
      break;
    case 3:
      infoLabel->setText(FLOW_PANEL_TEXT);
      break;
    case 4:
      infoLabel->setText(FLUID_PANEL_TEXT);
      break;
    case 5 :
      infoLabel->setText(RESULT_PANEL_TEXT);
      break;
    }
}

void PanelCEPElement::deleteWidgets(){
  delete infoLabel;
  infoLabel = nullptr;
}

void PanelCEPElement::connectAll(){

}

void PanelCEPElement::disconnectAll(){
  // Brak meteprogramowego rozłączenia
}

QLabel* PanelCEPElement::getInfoLabel(){
  return infoLabel;
}

uint PanelCEPElement::getNumberOfPanel(){
    return (uint)numberOfPanel;
}

void PanelCEPElement::setNumber(InnerWinStatusCEP setNumb){
  numberOfPanel = setNumb;
}


PanelCEPElement::~PanelCEPElement(){
    disconnectAll();
    deleteWidgets();
}

void PanelCEPElement::chooseFlangeTypeButtonReleased(){}

