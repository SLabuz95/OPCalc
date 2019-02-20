#include"menu.hpp"
#include<QFileDialog>
#include<QFile>
#include"projectmenager.hpp"
#include<QTextStream>
#include<QMessageBox>
#include"readingmaterialsfilesenum.hpp"
#include"substancesparamsfileparamenum.hpp"
#include"appwindow.hpp"
#include<QMenu>
#include"windowmacros.hpp"
#include"innerwindowcep.hpp"
#include"appclass.hpp"
#include"innerwindowcp.hpp"
#include"mydialog.hpp"
#include"prompttypeenum.hpp"
#include"projectinfo.hpp"
#include"promptpanel.hpp"
#include"tabbar.hpp"
#include<QFileDialog>
#include<QByteArray>
#include<QTextStream>
#include<QDesktopServices>
#include<QDebug>

Menu::Menu(AppWindow *setParent){
    parent = setParent;
    init();
    createWidgets();
    createLayout();
    connectAllAtStart();
    addActionsToMenuAtStart();
}

Menu::~Menu(){
  disconnectAllToDelete();
  delete chooseProjectAction;
  chooseProjectAction = nullptr;
  delete newProjectAction;
  newProjectAction = nullptr;
  delete openProjectAction;
  openProjectAction = nullptr;
  delete deleteProjectAction;
  deleteProjectAction = nullptr;
  delete quitAction;
  quitAction = nullptr;
  delete optionsAction;
  optionsAction = nullptr;
  delete helpAction;
  helpAction = nullptr;
  delete programInformationAction;
  programInformationAction = nullptr;
  delete authorsAction;
  authorsAction = nullptr;
  switch((uint) *(parent->getWinStat())){
    case (uint) CALC_ENTER_PARAMS:
      delete saveAction;
      saveAction = nullptr;
      delete closeProjectAction;
      closeProjectAction = nullptr;
      break;
    }

  // Usuwanie QMenu
  delete fileMenu;
  fileMenu = nullptr;
  delete toolsMenu;
  toolsMenu = nullptr;
  delete helpMenu;
  helpMenu = nullptr;
  }

void Menu::init(){
    // Brak
}

void Menu::createWidgets(){
    if(fileMenu == nullptr){
        fileMenu = new QMenu();
        fileMenu->setTitle(MENU_BAR_FILE_MENU);
      }
    if(toolsMenu == nullptr){
        toolsMenu = new QMenu();
        toolsMenu->setTitle(MENU_BAR_OPTIONS_MENU);
      }
    if(helpMenu == nullptr){
        helpMenu = new QMenu();
        helpMenu->setTitle(MENU_BAR_HELP_MENU);
      }
    switch((uint) *(parent->getWinStat())){
    case (uint)CALC_ENTER_PARAMS:
        saveAction = new QAction(this);
        closeProjectAction = new QAction(this);
        break;
    default:
        if(chooseProjectAction == nullptr)
        chooseProjectAction = new QAction(this);
        if(newProjectAction == nullptr)
        newProjectAction = new QAction(this);
        if(openProjectAction == nullptr)
        openProjectAction = new QAction(this);
        if(deleteProjectAction == nullptr)
        deleteProjectAction = new QAction(this);
        if(quitAction == nullptr)
        quitAction = new QAction(this);
        if(optionsAction == nullptr)
        optionsAction = new QAction(this);
        if(helpAction == nullptr)
        helpAction = new QAction(this);
        if(!programInformationAction)
            programInformationAction = new QAction(this);
        if(!authorsAction)
            authorsAction = new QAction(this);
        break;
    }
}

void Menu::createLayout(){
    switch((uint) *(parent->getWinStat())){
    case (uint)CALC_ENTER_PARAMS:
        saveAction->setText(SAVE_ACTION_TEXT);
        closeProjectAction->setText(CLOSE_PROJECT_ACTION_TEXT);
        break;
    default:
        chooseProjectAction->setText(CHOOSE_PROJECT_ACTION_TEXT);
        newProjectAction->setText(NEW_PROJECT_ACTION_TEXT);
        openProjectAction->setText(OPEN_PROJECT_ACTION_TEXT);
        deleteProjectAction->setText(DELETE_PROJECT_ACTION_TEXT);
        quitAction->setText(QUIT_ACTION_TEXT);
        optionsAction->setText(OPTIONS_ACTION_TEXT);
        helpAction->setText(HELP_ACTION_TEXT);
        programInformationAction->setText("Informacje o programie");
        authorsAction->setText("Autorzy");
        break;
    }
}

void Menu::addActionsToMenu(){
    switch((uint) *(parent->getWinStat())){
      case (uint)CALC_ENTER_PARAMS:
        fileMenu->insertAction(quitAction, saveAction);
        fileMenu->insertAction(quitAction, closeProjectAction);
       }
}

void Menu::addActionsToMenuAtStart(){
  fileMenu->addAction(chooseProjectAction);
  fileMenu->addAction(newProjectAction);
  fileMenu->addAction(openProjectAction);
  fileMenu->addAction(deleteProjectAction);
  toolsMenu->addAction(optionsAction);
  helpMenu->addAction(helpAction);
  helpMenu->addAction(programInformationAction);
  helpMenu->addAction(authorsAction);
  switch((uint) *(parent->getWinStat())){
    case (uint)CALC_ENTER_PARAMS:
      fileMenu->addAction(saveAction);
     fileMenu->addAction(closeProjectAction);
     }
  fileMenu->addAction(quitAction);
  }

void Menu::setMenuBar(){
  parent->getMenuBar()->addMenu(fileMenu);
  parent->getMenuBar()->addMenu(toolsMenu);
  parent->getMenuBar()->addMenu(helpMenu);
  }

void Menu::deleteWidgets(){
 switch((uint) *(parent->getWinStat() + 1)){
   case CALC_ENTER_PARAMS:
     parent->getMenuBar()->removeAction(saveAction);
     delete saveAction;
     saveAction = nullptr;
     parent->getMenuBar()->removeAction(closeProjectAction);
     delete closeProjectAction;
     closeProjectAction = nullptr;
     break;
   }
}

void Menu::connectAllAtStart(){
  connect(chooseProjectAction, SIGNAL(triggered(bool)), this, SLOT(chooseProjectActionTriggered()));
  connect(newProjectAction, SIGNAL(triggered(bool)), this, SLOT(newProjectActionTrigerred()));
  connect(openProjectAction, SIGNAL(triggered(bool)), this, SLOT(openProjectActionTrigerred()));
  connect(deleteProjectAction, SIGNAL(triggered(bool)), this, SLOT(deleteProjectActionTrigerred()));
  connect(optionsAction, SIGNAL(triggered(bool)), this, SLOT(optionsActionTrigerred()));
  connect(helpAction, SIGNAL(triggered(bool)), this, SLOT(helpActionTrigerred()));
  connect(quitAction, SIGNAL(triggered(bool)), parent, SLOT(close()));
  connect(programInformationAction, SIGNAL(triggered(bool)), this, SLOT(programInformationActionTriggered()));
  connect(authorsAction, SIGNAL(triggered(bool)), this, SLOT(authorsActionTriggered()));
  switch((uint) *(parent->getWinStat())){
    case (uint)CALC_ENTER_PARAMS:
      connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(saveActionTrigerred()));
     connect(closeProjectAction, SIGNAL(triggered(bool)), this, SLOT(closeProjectActionTrigerred()));
      break;
    }
}

void Menu::connectAll(){
  switch((uint) *(parent->getWinStat())){
    case (uint)CALC_ENTER_PARAMS:
      connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(saveActionTrigerred()));
      connect(closeProjectAction, SIGNAL(triggered(bool)), this, SLOT(closeProjectActionTrigerred()));
      break;
    }
}

void Menu::disconnectAll(){
  switch((uint) *(parent->getWinStat() + 1)){
    case (uint)CALC_ENTER_PARAMS:
      disconnect(saveAction, SIGNAL(triggered(bool)), this, SLOT(saveActionTrigerred()));
      disconnect(closeProjectAction, SIGNAL(triggered(bool)), this, SLOT(closeProjectActionTrigerred()));
      break;
    }
}

void Menu::reloadMenu(){
  disconnectAll();
  deleteWidgets();
  createWidgets();
  createLayout();
  connectAll();
  addActionsToMenu();
}

void Menu::chooseProjectActionTriggered(){
    if(*parent->getWinStat() == CALC_ENTER_PARAMS){
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
    if(parent->getApplication()->getProjectToShow() != nullptr)
        parent->getApplication()->setProjectToShow(nullptr);
    if(*(parent->getWinStat()) != CHOOSE_PROJECT){
        parent->getApplication()->setStatusToUse(CHOOSE_PROJECT_INNER);
        parent->setWinStat(CHOOSE_PROJECT);
    }else{
        if(*(parent->getInnerWindowCP()->getInnerWinStatCP()) != CHOOSE_PROJECT_INNER ){
            parent->getInnerWindowCP()->setInnerWinStatCP(CHOOSE_PROJECT_INNER);
        }
    }
}

void Menu::newProjectActionTrigerred(){
    if(*parent->getWinStat() == CALC_ENTER_PARAMS){
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
    if(parent->getApplication()->getProjectToShow() != nullptr)
        parent->getApplication()->setProjectToShow(nullptr);
    if(*(parent->getWinStat()) != CHOOSE_PROJECT){
         parent->getApplication()->setStatusToUse(NEW_PROJECT);
         parent->setWinStat(CHOOSE_PROJECT);
    }else{
        if(*(parent->getInnerWindowCP()->getInnerWinStatCP()) != NEW_PROJECT ){
             parent->getInnerWindowCP()->setInnerWinStatCP(NEW_PROJECT);
        }
    }
}

void Menu::openProjectActionTrigerred(){
    if(*parent->getWinStat() == CALC_ENTER_PARAMS){
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
    if(parent->getApplication()->getProjectToShow() != nullptr)
        parent->getApplication()->setProjectToShow(nullptr);
    if(*(parent->getWinStat()) != CHOOSE_PROJECT){
         parent->getApplication()->setStatusToUse(OPEN_PROJECT);
         parent->setWinStat(CHOOSE_PROJECT);
    }else{
        if(*(parent->getInnerWindowCP()->getInnerWinStatCP()) != OPEN_PROJECT){
            parent->getInnerWindowCP()->setInnerWinStatCP(OPEN_PROJECT);
        }
    }
}

void Menu::deleteProjectActionTrigerred(){
    if(*parent->getWinStat() == CALC_ENTER_PARAMS){
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
    if(parent->getApplication()->getProjectToShow() != nullptr)
        parent->getApplication()->setProjectToShow(nullptr);
    if(*(parent->getWinStat()) != CHOOSE_PROJECT){
         parent->getApplication()->setStatusToUse(DELETE_PROJECT);
         parent->setWinStat(CHOOSE_PROJECT);
    }else{
        if(*(parent->getInnerWindowCP()->getInnerWinStatCP()) != DELETE_PROJECT){
            parent->getInnerWindowCP()->setInnerWinStatCP(DELETE_PROJECT);
        }
    }
}

void Menu::optionsActionTrigerred(){
    if(*parent->getWinStat() == CALC_ENTER_PARAMS){
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
        parent->getInnerWinCEP()->setInnerWinStatusCEP(*parent->getInnerWinCEP()->getInnerWinStatusCEP());
    }
    MyDialog *dlg = new MyDialog(DEFAULT_PATH_DIALOG, parent);
    dlg->move(parent->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dlg->width()) * 0.5, (APP_WINDOW_HEIGHT - dlg->height()) * 0.5)));
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->show();
    dlg->exec();
    delete dlg;
}

void Menu::helpActionTrigerred(){
    qDebug() << ((QString("\"") == '"')? "true" : "false");
    //QDesktopServices::openUrl(QUrl::fromLocalFile("OPCalc Manual.pdf"));
}


void Menu::saveActionTrigerred(){
    if(*parent->getWinStat() == CALC_ENTER_PARAMS){
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
        ProjectMenager::writeProject(parent->getInnerWinCEP()->getProjectInfo());
        parent->getInnerWinCEP()->setInnerWinStatusCEP(*parent->getInnerWinCEP()->getInnerWinStatusCEP());
    }
}

void Menu::closeProjectActionTrigerred(){
    if(*parent->getWinStat() == CALC_ENTER_PARAMS){
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
    for(uint i = 0; i < (*parent->getApplication()->getProjectInfo())->getPosition(); i++){
        if((*(parent->getApplication()->getProjectInfo() + i)) == parent->getInnerWinCEP()->getProjectInfo()){
            if(ProjectMenager::checkProject((*(parent->getApplication()->getProjectInfo() + i))))
                    {
                        MyDialog *dlg = new MyDialog(QUESTION_DIALOG, QString(), QString("Czy chcesz zapisać projekt?"), true, QString(), QString(), QString(), parent);
                        dlg->move(parent->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dlg->width()) * 0.5, (APP_WINDOW_HEIGHT - dlg->height()) * 0.5)));
                        dlg->setWindowModality(Qt::ApplicationModal);
                        dlg->show();
                        switch(dlg->exec()){
                        case YES_RESULT:
                            ProjectMenager::writeProject((*(parent->getApplication()->getProjectInfo() + i)));
                            break;
                        case NO_RESULT:
                            break;
                        }
                        delete dlg;
                    }
            parent->getApplication()->removeProjectInfo((*(parent->getApplication()->getProjectInfo() + i))->getPosition());
            parent->getTabBar()->reload();
            parent->setWinStat(CHOOSE_PROJECT);
            return;
        }
    }
}

void Menu::programInformationActionTriggered(){
    MyDialog dlg(QUESTION_DIALOG, QString("Informacje o programie"), QString("Program umożliwia obliczanie parametrów kryz pomiarowych na podstawie normy PN-EN ISO 5167. Program napisany w ramach pracy inżynierskiej dla potrzeb firmy Grupa Azoty Automatyka SA w Tarnowie.Program napisany z wykorzystaniem bibliotek Qt i jest oparty na licencji LGPL."), false, QString("Zamknij"), QString(), QString(), nullptr);
    dlg.setWindowModality(Qt::ApplicationModal);
    dlg.exec();
}

void Menu::authorsActionTriggered(){
    MyDialog dlg(QUESTION_DIALOG, QString("Autorzy"), QString("Autor programu: Sebastian Łabuz\n Kontakt emailowy: sebastian.labuz@interia.pl"), false, QString("Zamknij"), QString(), QString(), nullptr);
    dlg.setWindowModality(Qt::ApplicationModal);
    dlg.exec();
}

void Menu::disconnectAllToDelete(){
    disconnect(chooseProjectAction, SIGNAL(triggered(bool)), this, SLOT(chooseProjectActionTriggered()));
    disconnect(newProjectAction, SIGNAL(triggered(bool)), this, SLOT(newProjectActionTrigerred()));
    disconnect(openProjectAction, SIGNAL(triggered(bool)), this, SLOT(openProjectActionTrigerred()));
    disconnect(deleteProjectAction, SIGNAL(triggered(bool)), this, SLOT(deleteProjectActionTrigerred()));
    disconnect(optionsAction, SIGNAL(triggered(bool)), this, SLOT(optionsActionTrigerred()));
    disconnect(helpAction, SIGNAL(triggered(bool)), this, SLOT(helpActionTrigerred()));
    disconnect(quitAction, SIGNAL(triggered(bool)), parent, SLOT(close()));
    disconnect(programInformationAction, SIGNAL(triggered(bool)), this, SLOT(programInformationActionTrigerred()));
    disconnect(authorsAction, SIGNAL(triggered(bool)), this, SLOT(authorsActionTrigerred()));
    switch((uint) *(parent->getWinStat())){
      case (uint)CALC_ENTER_PARAMS:
        disconnect(saveAction, SIGNAL(triggered(bool)), this, SLOT(saveActionTrigerred()));
        disconnect(closeProjectAction, SIGNAL(triggered(bool)), this, SLOT(closeProjectActionTrigerred()));
        break;
      }
}
