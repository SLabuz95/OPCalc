#include"appwindow.hpp"
#include"appclass.hpp"
#include"tabbar.hpp"
#include"innerwindowcp.hpp"
#include"promptpanel.hpp"
#include"titlewidget.hpp"
#include"config.hpp"
#include"projectinfopanelcep.hpp"
#include"innerwindowcep.hpp"
#include"panelcep.hpp"
#include"panelcepelement.hpp"
#include"projectinfo.hpp"
#include"toolsfortoolbar.hpp"
#include"mydialog.hpp"
#include"projectmenager.hpp"
#include"menu.hpp"

AppWindow::AppWindow(App *parent){
    application = parent;
    progressBarSLValue = new uint;
    *progressBarSLValue = 0;
    this->setWindowFlags(Qt::FramelessWindowHint);
    init();
    setWindow();
    createWidgets();
    createLayout();
    connectWidgets();
    initAndStartTimers();
    application->setProjectToShow(nullptr);
}

AppWindow::~AppWindow(){
    WINDOW_STATUS_SAVE(winStat)
    disconnectWidgets();
    deleteWidgets();
    deleteMenu();
    deleteToolBar();
    disconnect(reloadTimer, SIGNAL(timeout()), this, SLOT(reloadTimerTask()));
    delete reloadTimer;
    reloadTimer = nullptr;
    delete winStat;
    winStat = nullptr;
    delete statusBar;
    statusBar = nullptr;
}

void AppWindow::createWidgetsForStartLoading(){
    windowIcon = new QLabel(this);
    windowIcon->show();
    progressBarSL = new QProgressBar(this);
    progressBarSL->show();
    progressBarSLMaxReady = new bool;
    *progressBarSLMaxReady = false;
 }

void AppWindow::createWidgetsForChooseProject(){
    if(tabBar == nullptr)
    {
        tabBar = new TabBar(this);
        static_cast<QFrame*>(tabBar)->setParent(this);
        tabBar->show();
    }else{
        tabBar->reload();
    }
    if(menu != nullptr)
        changeMenu();
    else
        createMenu();
    if(menuBar == nullptr)
       createMenuBar();
    if(toolBar != nullptr)
        changeToolBar();
    else
        createToolBar();
    if(statusBar == nullptr)
        createStatusBar();
    innerWinCP = new InnerWindowCP(this, false, false);
    static_cast<QFrame*>(innerWinCP)->setParent(this);
    innerWinCP->show();
    if(!promptPanel){
        promptPanel = new PromptPanel(this);
        static_cast<QFrame*>(promptPanel)->setParent(this);
        promptPanel->show();
    }
    titleWidget = new TitleWidget(this);
    static_cast<QFrame*>(titleWidget)->setParent(this);
    titleWidget->show();
    if(*innerWinCP->getEditProjectData())
        titleWidget->getTitleLabelCP()->setText("Dane projektu");
    if(application->getConfiguration()->getNumbOfRecentProjects() != 0 && innerWinCP->getRecentPanel() != nullptr)
        application->getConfiguration()->readForRecentList();
    show();
    promptPanel->show();
}

void AppWindow::createWidgetsForCalcEnterParams(){
    if(tabBar == nullptr)
    {
        tabBar = new TabBar(this);
        static_cast<QFrame*>(tabBar)->setParent(this);
        tabBar->show();
    }
    if(menu != nullptr)
        changeMenu();
    else
        createMenu();
    if(menuBar == nullptr)
        createMenuBar();
    if(toolBar != nullptr)
        changeToolBar();
    else
        createToolBar();
    if(statusBar == nullptr)
        createStatusBar();
    if(!promptPanel){
        promptPanel = new PromptPanel(this);
        static_cast<QFrame*>(promptPanel)->setParent(this);
        promptPanel->show();
    }
    projectInfoPanelCEP = new ProjectInfoPanelCEP(this);
    static_cast<QFrame*>(projectInfoPanelCEP)->setParent(this);
    projectInfoPanelCEP->show();
    innerWinCEP = new InnerWindowCEP(this, application->getProjectToShow());
    static_cast<QFrame*>(innerWinCEP)->setParent(this);
    innerWinCEP->show();
    panelCEP = new PanelCEP(this);
    static_cast<QFrame*>(panelCEP)->setParent(this);
    panelCEP->show();
    for(uint i = 0; i < 6; i++){
        (*(panelCEP->getPanelCEPElement() + i))->setStyleSheet(QString("PanelCEPElement{border: 1px solid rgba(255, 255, 255, 0.4);") + ((innerWinCEP != nullptr && i == *innerWinCEP->getInnerWinStatusCEP())? QString("background-color: qlineargradient(x1:0 , y1: 0.5, x2: 1, y2: 0.5, stop: 0 rgba(200,200,200,0.77), stop: 0.93 rgba(247,247,247,0.8), stop: 0.99 rgba(247,247,247,0.8));") : QString("background-color: rgba(255, 255, 255, 0.4);")) + ((i == 0)? QString("border-top-right-radius: 20px 15px;") : QString("")) +  ((i == 5)? QString("border-bottom-right-radius: 20px 15px;") : QString("")) + "}");
    }
    promptPanel->show();
}

void AppWindow::createWidgetsForCalcResult(){
    if(tabBar == nullptr)
    {
        tabBar = new TabBar(this);
        static_cast<QFrame*>(tabBar)->setParent(this);
        tabBar->show();
    }
    if(menu != nullptr)
        changeMenu();
    else
        createMenu();
    if(menuBar == nullptr)
        createMenuBar();
    if(toolBar != nullptr)
        changeToolBar();
    else
        createToolBar();
    if(statusBar == nullptr)
        createStatusBar();
    if(!promptPanel){
        promptPanel = new PromptPanel(this);
        static_cast<QFrame*>(promptPanel)->setParent(this);
        promptPanel->show();
    }
    projectInfoPanelCR = new ProjectInfoPanelCEP(this);
    static_cast<QFrame*>(projectInfoPanelCR)->setParent(this);
    projectInfoPanelCR->show();
    resultLabelCR = new QLabel(this);
    static_cast<QFrame*>(resultLabelCR)->setParent(this);
    resultLabelCR->show();
}

void AppWindow::createLayoutForStartLoading(){
    windowIcon->setGeometry(0,0, width(), height());
    windowIcon->setPixmap(START_LOADING_PIXMAP);
    windowIcon->setStyleSheet("background-color: transparent;");
    progressBarSL->setGeometry(50, 150, 100, 20);
    progressBarSL->setStyleSheet("QProgressBar{border-radius: 5px; border: 1px solid grey; background-color: white;} QProgressBar::chunk{background-color: grey; width: 1px;}");
    progressBarSL->setTextVisible(false);
    configProgressBarSL();
}

void AppWindow::createLayoutForChooseProject(){
    if(tabBar != nullptr)
        tabBar->setGeometry(0, MENU_BAR_HEIGHT + TOOL_BAR_HEIGHT, TABBAR_WIDTH, TABBAR_HEIGHT);
    menuBar->setGeometry(0, 0, MENU_BAR_WIDTH, MENU_BAR_HEIGHT);
    toolBar->setGeometry(0, MENU_BAR_HEIGHT, TOOL_BAR_WIDTH, TOOL_BAR_HEIGHT);
    titleWidget->setGeometry(0, MENU_BAR_HEIGHT + TOOL_BAR_HEIGHT + TABBAR_HEIGHT , CHOOSE_PROJECT_TITLE_WIDGET_WIDTH, CHOOSE_PROJECT_TITLE_WIDGET_HEIGHT);
    innerWinCP->setGeometry(0, MENU_BAR_HEIGHT + TOOL_BAR_HEIGHT + TABBAR_HEIGHT + CHOOSE_PROJECT_TITLE_WIDGET_HEIGHT, CHOOSE_PROJECT_INNER_WINDOW_WIDTH, CHOOSE_PROJECT_INNER_WINDOW_HEIGHT);
    statusBar->setGeometry(0, APP_WINDOW_HEIGHT - STATUS_BAR_HEIGHT, STATUS_BAR_WIDTH, STATUS_BAR_HEIGHT);
    if(*innerWinCP->getEditProjectData())
        application->getProjectToShow()->readForEditData();
    promptPanel->raise();
}

void AppWindow::createLayoutForCalcEnterParams(){
    if(tabBar != nullptr)
    tabBar->setGeometry(0, MENU_BAR_HEIGHT + TOOL_BAR_HEIGHT, TABBAR_WIDTH, TABBAR_HEIGHT);
    projectInfoPanelCEP->move(0, MENU_BAR_HEIGHT + TOOL_BAR_HEIGHT + TABBAR_HEIGHT + 3);
    panelCEP->setGeometry(0, MENU_BAR_HEIGHT + TOOL_BAR_HEIGHT + TABBAR_HEIGHT + PROJECT_PANEL_HEIGHT, PANEL_WIDTH, 6*PANEL_HEIGHT);
    innerWinCEP->setGeometry(PANEL_WIDTH, MENU_BAR_HEIGHT + TOOL_BAR_HEIGHT + TABBAR_HEIGHT+ PROJECT_PANEL_HEIGHT, ENTER_PARAMS_PANEL_WIDTH, ENTER_PARAMS_PANEL_HEIGHT);
    application->setProjectToShow(nullptr);
    promptPanel->raise();
}

void AppWindow::createLayoutForCalcResult(){
    projectInfoPanelCEP->setGeometry(0, MENU_BAR_HEIGHT + TOOL_BAR_HEIGHT, PROJECT_PANEL_WIDTH, PROJECT_PANEL_HEIGHT);
    resultLabelCR->setGeometry(RESULT_PANEL_RAPORT_HORIZONTAL_OFFSET, RESULT_PANEL_RAPORT_VERTICAL_OFFSET, RESULT_PANEL_RAPORT_RESULT_LABEL_WIDTH, RESULT_PANEL_RAPORT_RESULT_LABEL_HEIGHT);
    resultLabelCR->setText(RESULT_PANEL_TEXT);
}

void AppWindow::deleteWidgetsForStartLoading(){
    delete progressBarSL;
    progressBarSL = nullptr;
    delete windowIcon;
    windowIcon = nullptr;
    delete progressBarSLMaxReady;
    progressBarSLMaxReady = nullptr;
    delete progressBarSLValue;
    progressBarSLValue = nullptr;
}

void AppWindow::deleteWidgetsForChooseProject(){
    if(application->getProjectInfo() == nullptr){
        delete tabBar;
        tabBar = nullptr;
    }
    delete titleWidget;
    titleWidget = nullptr;
    delete innerWinCP;
    innerWinCP = nullptr;
}

void AppWindow::deleteWidgetsForCalcEnterParams(){
    if(application->getProjectInfo() == nullptr){
        delete tabBar;
        tabBar = nullptr;
    }
    delete projectInfoPanelCEP;
    projectInfoPanelCEP = nullptr;
    delete panelCEP;
    panelCEP = nullptr;
    delete innerWinCEP;
    innerWinCEP = nullptr;
}

void AppWindow::deleteWidgetsForCalcResult(){
    if(application->getProjectInfo() == nullptr){
        delete tabBar;
        tabBar = nullptr;
    }
    delete projectInfoPanelCR;
    projectInfoPanelCR = nullptr;
    delete resultLabelCR;
    resultLabelCR = nullptr;
}

void AppWindow::createWidgets(){
    switch((uint) *winStat){
    case (uint)START_LOADING:
        createWidgetsForStartLoading();
        break;
    case (uint)CHOOSE_PROJECT:
        createWidgetsForChooseProject();
        break;
    case (uint)CALC_ENTER_PARAMS:
        createWidgetsForCalcEnterParams();
        break;
    case (uint)CALC_RESULT:
        createWidgetsForCalcResult();
        break;
    }
}

void AppWindow::createLayout(){
    switch((uint) *winStat){
    case (uint)START_LOADING:
        createLayoutForStartLoading();
        break;
    case (uint)CHOOSE_PROJECT:
        createLayoutForChooseProject();
        break;
    case (uint)CALC_ENTER_PARAMS:
        createLayoutForCalcEnterParams();
        break;
    case (uint)CALC_RESULT:
        createLayoutForCalcResult();
        break;
    }

}

void AppWindow::setWindow(){
    if(*winStat == START_LOADING){
        setMinimumSize(START_LOADING_WINDOW_ICON_WIDTH, START_LOADING_WINDOW_ICON_HEIGHT);
        setMaximumSize(START_LOADING_WINDOW_ICON_WIDTH, START_LOADING_WINDOW_ICON_HEIGHT);
        setStyleSheet("AppWindow{background: qlineargradient(x1:0, y1:1, x2:1, y2:0, stop:0 rgba(235, 235,235, 255),stop: 0.63 rgba(200, 200, 200, 255), stop: 1 rgba(210,210,210,255)); border-radius: 10px;}");
    }
    if(*(winStat + 1) == START_LOADING){
        setWindowFlags(Qt::Window);
        hide();
        move(20,20);
        setMinimumSize(APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT);
        setMaximumSize(APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT);
        setStyleSheet("AppWindow{background: qlineargradient(x1:0, y1:1, x2:1, y2:0, stop:0 rgba(235, 235,235, 255),stop: 0.63 rgba(200, 200, 200, 255), stop: 1 rgba(210,210,210,255));}");
    }
      show();
}

void AppWindow::deleteWidgets(){
  switch((uint) *(winStat + 1)){
    case (uint)START_LOADING:
      deleteWidgetsForStartLoading();
      break;
    case (uint)CHOOSE_PROJECT:
      deleteWidgetsForChooseProject();
      break;
    case (uint)CALC_ENTER_PARAMS:
      deleteWidgetsForCalcEnterParams();
      break;
    case (uint)CALC_RESULT:
      deleteWidgetsForCalcResult();
      break;
  }
}

void AppWindow::createMenu(){
  menu = new Menu(this);
}

void AppWindow::changeMenu(){
  menu->reloadMenu();
}

void AppWindow::changeMenuBar(){
  menu->reloadMenu();
}

void AppWindow::createMenuBar(){
  menuBar = new QMenuBar(this);
  menuBar->setFixedHeight(MENU_BAR_HEIGHT);
  menu->setMenuBar();
  menuBar->show();
}

void AppWindow::createToolBar(){
  toolBar = new QToolBar(this);
  tools = new ToolsForToolBar(this);
  tools->setToolBar();
  toolBar->show();
}

void AppWindow::deleteMenu(){
    delete menu;
    menu = nullptr;
    delete menuBar;
    menuBar = nullptr;
}

void AppWindow::deleteToolBar(){
  delete tools;
  tools = nullptr;
  delete toolBar;
  toolBar = nullptr;
  }

void AppWindow::changeToolBar(){
  tools->reloadTools();
  tools->setToolBar();

}

void AppWindow::createStatusBar(){
  statusBar = new QStatusBar(this);
  statusBar->setGeometry(0, APP_WINDOW_HEIGHT - STATUS_BAR_HEIGHT, STATUS_BAR_WIDTH, STATUS_BAR_HEIGHT);
  statusBar->setStyleSheet("QStatusBar{background: rgba(255,255,255, 0.6);} QStatusBar::item{border: 2px solid red;}");
  statusBar->show();
  statusBar->showMessage("Gotowe");
}

void AppWindow::configProgressBarSL(){
  progressBarSL->setMinimum(0);
  progressBarSL->setMaximum(1);
}

void AppWindow::reload(){
    reloadTimer->stop();
    winStatChanged = false;
    disconnectWidgets();
    deleteWidgets();
    setWindow();
    createWidgets();
    createLayout();
    connectWidgets();
    switch((uint) *winStat){
    case (uint)CHOOSE_PROJECT:
        switch((uint) *innerWinCP->getInnerWinStatCP()){
        case (uint) OPEN_PROJECT:
            application->getConfiguration()->readForProjectList(true);
            break;
        case (uint) DELETE_PROJECT:
            application->getConfiguration()->readForProjectList(false);
            break;
        }
    }
    statusBar->showMessage("Gotowy");
    reloadTimer->start();
}

void AppWindow::init(){
 winStat = new WinStatus[2];
 *winStat = START_LOADING;
 *(winStat + 1) = CALC_RESULT;
 setWindowTitle(QString(APP_NAME) + QString(" ") + APP_VERSION);
 setWindowIcon(QIcon("Icons\\winIcon.png"));
}

void AppWindow::initAndStartTimers(){
  reloadTimer = new QTimer(this);
  loadingTimer = new QTimer(this);
  connect(loadingTimer, SIGNAL(timeout()), this, SLOT(loadingTimerTask()));
  connect(reloadTimer, SIGNAL(timeout()), this, SLOT(reloadTimerTask()));
  reloadTimer->setInterval(1);
  loadingTimer->setInterval(1);
  reloadTimer->start();
  loadingTimer->start();
}

void AppWindow::connectWidgets(){
  switch((uint) *winStat){
    case (uint)START_LOADING:
      break;
    case (uint)CHOOSE_PROJECT:
      break;
    case (uint)CALC_ENTER_PARAMS:
      break;
    case (uint)CALC_RESULT:
      break;
   }

}

void AppWindow::disconnectWidgets(){
  switch((uint) *(winStat + 1)){
    case (uint)START_LOADING:
      break;
    case (uint)CHOOSE_PROJECT:
      break;
    case (uint)CALC_ENTER_PARAMS:
      break;
    case (uint)CALC_RESULT:
      break;
  }

}

void AppWindow::disconnectLoadingTimer(){
    disconnect(loadingTimer, SIGNAL(timeout()), this, SLOT(loadingTimerTask()));
}

void AppWindow::deleteLoadingTimer(){
    delete loadingTimer;
    loadingTimer = nullptr;
}

void AppWindow::reloadTimerTask(){
    if(winStatChanged == true)
        reload();
 }

App* AppWindow::getApplication(){
    return application;
}

WinStatus* AppWindow::getWinStat(){
    return winStat;
}

QMenuBar* AppWindow::getMenuBar(){
  return menuBar;
}

QToolBar* AppWindow::getToolBar(){
    return toolBar;
}

InnerWindowCP* AppWindow::getInnerWindowCP(){
    return innerWinCP;
}

Menu* AppWindow::getMenu(){
    return menu;
}

ToolsForToolBar* AppWindow::getTools(){
    return tools;
}

InnerWindowCEP* AppWindow::getInnerWinCEP(){
    return innerWinCEP;
}

TitleWidget* AppWindow::getTitleWidget(){
  return titleWidget;
}

QProgressBar* AppWindow::getProgressBarSL(){
  return progressBarSL;
}

TabBar* AppWindow::getTabBar(){
    return tabBar;
}

PromptPanel* AppWindow::getPromptPanel(){
    return promptPanel;
}

QStatusBar* AppWindow::getStatusBar(){
    return statusBar;
}

void AppWindow::setWinStat(WinStatus winStatus){
  WINDOW_STATUS_SAVE(winStat)
  *winStat = winStatus;
   winStatChanged = true;
}

void AppWindow::setWinStatChanged(bool statChanged){
  winStatChanged = statChanged;
}


void AppWindow::loadingTimerTask(){
    if(application->getConfiguration() && application->getConfiguration()->isConstructingFail()){
        QFile(CONFIG_FILE).remove();
        QFile(CONFIG_BACK_UP_FILE).remove();
        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny"), QString("Błąd odczytu pliku konfiguracyjnego. Aplikacja zostanie zamknięta."), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.exec();
        QFile(CONFIG_FILE).remove();
        QFile(CONFIG_BACK_UP_FILE).remove();
        close();
    }
  if(*progressBarSLMaxReady){
    loadingTimer->stop();
    WINDOW_STATUS_SAVE(winStat);
        disconnectLoadingTimer();
        delete loadingTimer;
        loadingTimer = nullptr;
        *winStat = CHOOSE_PROJECT;
        winStatChanged = true;
    }else{
      progressBarSL->setMaximum(application->getConfiguration()->getProgressBarMax());
      progressBarSL->setMinimum(application->getConfiguration()->getProgressBarMin());
      progressBarSL->setValue(application->getConfiguration()->getProgressBarValue());
  }
}

void AppWindow::setProgressBarSLMaxReady(bool set){
  *progressBarSLMaxReady = set;
}

void AppWindow::saveAllProjects(){
    if(application->getProjectInfo()){
        for(uint i = (*application->getProjectInfo())->getPosition();i != 0; i--){
            if(ProjectMenager::checkProject(*(application->getProjectInfo() + i - 1))){
                MyDialog *dlg = new MyDialog(QUESTION_DIALOG, QString(), QString("Czy chcesz zapisać projekty?"),true, QString(), QString(), QString(),this);
                dlg->setWindowModality(Qt::ApplicationModal);
                dlg->move(mapToGlobal(QPoint((width() - dlg->width()) * 0.5, (height() - dlg->height()) * 0.5)));
                dlg->show();
                switch(dlg->exec()){
                case YES_RESULT:
                    for(uint i = (*application->getProjectInfo())->getPosition();i != 0; i--) {
                        ProjectMenager::writeProject(*(application->getProjectInfo() + i - 1));
                    }
               }
                delete dlg;
                return;
            }
        }
    }
}

void AppWindow::closeEvent(QCloseEvent *event)
  {
    if(*winStat == CALC_ENTER_PARAMS){
        switch(*innerWinCEP->getInnerWinStatusCEP()){
        case (uint)FLANGE:
            innerWinCEP->saveFromFlangeData();
            break;
        case (uint)PIPE:
            innerWinCEP->saveFromPipeData();
            break;
        case (uint)FLOW:
            innerWinCEP->saveFromFlowData();
            break;
        case (uint)FLUID:
            innerWinCEP->saveFromFluidData();
            break;
        default:
            break;
        }
    }
    application->getConfiguration()->rewriteGeneral();
    saveAllProjects();
    event->accept();
  }

PanelCEP* AppWindow::getPanelCEP(){
    return panelCEP;
}
