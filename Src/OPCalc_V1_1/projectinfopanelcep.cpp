#include"projectinfopanelcep.hpp"
#include"projectinfopanelcepcatchevents.hpp"
#include"windowmacros.hpp"
#include"appwindow.hpp"
#include"appclass.hpp"
#include"innerwindowcep.hpp"
#include"projectinfo.hpp"

ProjectInfoPanelCEP::ProjectInfoPanelCEP(AppWindow *setParent){
    parent = setParent;
    init();
    createWidgets();
    createLayout();
    setWindow();
    connectAll();
}

ProjectInfoPanelCEP::~ProjectInfoPanelCEP(){
  disconnectAll();
  deleteWidgets();
  delete projectInfoPanelCEPCatchEvents;
  projectInfoPanelCEPCatchEvents = nullptr;
}

void ProjectInfoPanelCEP::init(){
    projectInfoPanelCEPCatchEvents = new ProjectInfoPanelCEPCatchEvents(this);
}

void ProjectInfoPanelCEP::setWindow(){
    setBaseSize(APP_WINDOW_WIDTH, PROJECT_PANEL_HEIGHT);
    setStyleSheet("ProjectInfoPanelCEP{background-color: rgba(255, 255, 255, 0.4);}");
}

void ProjectInfoPanelCEP::createWidgets(){
    projectNameLabel = new QLabel(this);
    projectNameLabel->show();
    infoButton = new QLabel(this);
    infoButton->show();
}

void ProjectInfoPanelCEP::createLayout(){
    projectNameLabel->setText(PROJECT_NAME_TEXT);
    projectNameLabel->setGeometry(0, 0, APP_WINDOW_WIDTH - PROJECT_INFO_BUTTON_WIDTH, PROJECT_NAME_BUTTON_HEIGHT);
    projectNameLabel->setAlignment(Qt::AlignCenter);
    infoButton->setGeometry(APP_WINDOW_WIDTH - PROJECT_INFO_BUTTON_WIDTH , 0, PROJECT_INFO_BUTTON_WIDTH, PROJECT_NAME_BUTTON_HEIGHT);
    infoButton->setPixmap(PROJECT_INFO_ICON);
}

void ProjectInfoPanelCEP::deleteWidgets(){
    delete projectNameLabel;
    projectNameLabel = nullptr;
    delete infoButton;
    infoButton = nullptr;
}

void ProjectInfoPanelCEP::connectAll(){
infoButton->installEventFilter(projectInfoPanelCEPCatchEvents);
}

void ProjectInfoPanelCEP::disconnectAll(){

}

QLabel* ProjectInfoPanelCEP::getInfoButton(){
    return infoButton;
}

void ProjectInfoPanelCEP::infoButtonReleased(){
    if(*parent->getInnerWinCEP()->getParent()->getWinStat() == CALC_ENTER_PARAMS){
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
 parent->getApplication()->setProjectToShow(parent->getInnerWinCEP()->getProjectInfo());
 InnerWinStatusCEP *tempInnerWinStatusCEP = new InnerWinStatusCEP;
 *tempInnerWinStatusCEP = *parent->getInnerWinCEP()->getInnerWinStatusCEP();
 parent->getInnerWinCEP()->getProjectInfo()->setInnerWinStatusCEP(tempInnerWinStatusCEP);
 parent->getApplication()->setStatusToUse(NEW_PROJECT);
 parent->setWinStat(CHOOSE_PROJECT);
}

AppWindow* ProjectInfoPanelCEP::getParent(){
    return parent;
}
