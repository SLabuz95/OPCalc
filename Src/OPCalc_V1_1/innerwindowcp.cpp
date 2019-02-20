#include"innerwindowcp.hpp"
#include"recentpanel.hpp"
#include"errormacroes.hpp"
#include"projectinfopanelcp.hpp"
#include"projectmenager.hpp"
#include"mainmacros.hpp"
#include<QFontDatabase>
#include<QKeyEvent>
#include<QScrollBar>
#include<QPalette>
#include<QFileDialog>
#include<QFontMetrics>
#include<QCursor>
#include<QTimer>
#include"innerwindowcpcatchevents.hpp"
#include"appwindow.hpp"
#include<QLineEdit>
#include"optionalprojectdatapanel.hpp"
#include<QCheckBox>
#include"appclass.hpp"
#include"config.hpp"
#include"projectlist.hpp"
#include"projectinfo.hpp"
#include"tabbar.hpp"
#include"titlewidget.hpp"
#include"projectlistelementdp.hpp"
#include"formatabledatafield.hpp"
#include"mydialog.hpp"
#include"projectlistelementop.hpp"

InnerWindowCP::InnerWindowCP(AppWindow *setParent, bool setEditData, bool setEditable){
    editProjectData = new bool;
    *editProjectData = setEditData;
    editableData = new bool;
    *editableData = setEditable;
    innerChange = new bool;
    *innerChange = false;
    parent = setParent;
    init();
    setWindow();
    checkStatusToUse();
    if(parent->getApplication()->getProjectToShow() != nullptr){
        *editProjectData = true;
    }
    createWidgets();
    createLayout();
    connectAll();
    initAndStartTimers();
 }

InnerWindowCP::~InnerWindowCP(){
    reloadTimer->stop();
    *innerChange = true;
    disconnectAll();
    deleteWidgets();
    delete reloadTimer;
    reloadTimer = nullptr;
    delete editProjectData;
    editProjectData = nullptr;
    delete editableData;
    editableData = nullptr;
    delete innerChange;
    innerChange = nullptr;
    delete innerWinStatCP;
    innerWinStatCP = nullptr;
    delete innerWindowCPCatchEvents;
    innerWindowCPCatchEvents = nullptr;
    if(noProjectsLabel != nullptr)
        delete noProjectsLabel;
    noProjectsLabel = nullptr;
}

void InnerWindowCP::init(){
    innerWinStatCP = new InnerWinStatusCP[2];
    *innerWinStatCP = CHOOSE_PROJECT_INNER;
    *(innerWinStatCP + 1) = CHOOSE_PROJECT_INNER;
    innerWindowCPCatchEvents = new InnerWindowCPCatchEvents(this);
}

void InnerWindowCP::setWindow(){
    // Ustawione w klasie nadrzędnej
    setStyleSheet("QLabel{font-family: \"Arial\"; font-weight: normal; font-size: 12px;}");
 }

void InnerWindowCP::initAndStartTimers(){
   reloadTimer = new QTimer();
   reloadTimer->setInterval(1);
   connect(reloadTimer, SIGNAL(timeout()), this, SLOT(reloadTimerTask()));
   reloadTimer->start();
}

void InnerWindowCP::createWidgets(){
    switch((uint) *innerWinStatCP){
    case (uint)CHOOSE_PROJECT_INNER:
        createWidgetsForChooseProject();
        break;
    case (uint)NEW_PROJECT:
        createWidgetsForNewProject();
        break;
    case (uint)OPEN_PROJECT:
        createWidgetsForOpenProject();
        break;
    case (uint)DELETE_PROJECT:
        createWidgetsForDeleteProject();
        break;
    }
   }

void InnerWindowCP::createLayout(){
    switch((uint) *innerWinStatCP){
    case (uint)CHOOSE_PROJECT_INNER:
        createLayoutForChooseProject();
        break;
    case (uint)NEW_PROJECT:
        createLayoutForNewProject();
        break;
    case (uint)OPEN_PROJECT:
        createLayoutForOpenProject();
        break;
    case (uint)DELETE_PROJECT:
        createLayoutForDeleteProject();
        break;
    }
    }

void InnerWindowCP::deleteWidgets(){
    switch((uint) *((*innerChange == true)? innerWinStatCP : (innerWinStatCP + 1))){
    case (uint)CHOOSE_PROJECT_INNER:
        deleteWidgetsForChooseProject();
        break;
    case (uint)NEW_PROJECT:
        deleteWidgetsForNewProject();
        break;
    case (uint)OPEN_PROJECT:
        deleteWidgetsForOpenProject();
        break;
    case (uint)DELETE_PROJECT:
        deleteWidgetsForDeleteProject();
        break;
    }
    if(*innerChange == false)
        *editProjectData = false;
 }

void InnerWindowCP::createWidgetsForChooseProject(){
    newProjectButtonCP = new QLabel(this);
    newProjectButtonCP->show();
    openProjectButtonCP = new QLabel(this);
    openProjectButtonCP->show();
    deleteProjectButtonCP = new QLabel(this);
    deleteProjectButtonCP->show();
    recentPanel = new RecentPanel(this);
    static_cast<QFrame*>(recentPanel)->setParent(this);
    recentPanel->show();

}

void InnerWindowCP::createWidgetsForNewProject(){
    if(*editProjectData == true){
        if(*editableData){
            projectNameTextEdit = new FormatableDataField<ProjectMenagerParam>(true, nullptr, false, this, QString(), false);
            projectNameTextEdit->show();
            projectNameTextEdit->getDataUTextEdit()->setFrame(false);
            fileDialogButton = new QLabel(this);
            fileDialogButton->show();
            resetToDefaultPathButton = new QLabel(this);
            resetToDefaultPathButton->show();
            defaultProjectCB = new QCheckBox(this);
            defaultProjectCB->show();
            defaultProjectHelpButton = new QLabel(this);
            defaultProjectHelpButton->show();
        }else{
            projectNameEditDataLabel = new QLabel(this);
            projectNameEditDataLabel->show();
        }
        projectNameLabel = new QLabel(this);
        projectNameLabel->show();
        projectPathLabel = new QLabel(this);
        projectPathLabel->show();
        projectCreatedLabel = new QLabel(this);
        projectCreatedLabel->show();
        projectModifiedLabel = new QLabel(this);
        projectModifiedLabel->show();
        projectCreatedDateLabel = new QLabel(this);
        projectCreatedDateLabel->show();
        defaultProjectLabel = new QLabel(this);
        defaultProjectLabel->show();
        projectModifiedLabel = new QLabel(this);
        projectModifiedLabel->show();
        projectModifiedDateLabel = new QLabel(this);
        projectModifiedDateLabel->show();
        optionalProjectDataPanel = new OptionalProjectDataPanel(this);
        static_cast<QFrame*>(optionalProjectDataPanel)->setParent(this);
        optionalProjectDataPanel->show();
        editButton = new QLabel(this);
        editButton->show();
        finalizeButton = new QLabel(this);
        finalizeButton->show();
    }else{
    projectNameLabel = new QLabel(this);
    projectNameLabel->show();
    projectNameTextEdit = new FormatableDataField<ProjectMenagerParam>(true, nullptr, false, this, QString(), false);
    projectNameTextEdit->setObjectName("projectName");
    projectNameTextEdit->show();
    projectNameTextEdit->getDataUTextEdit()->setFrame(false);
    projectPathLabel = new QLabel(this);
    projectPathLabel->show();
    projectNameTextEdit->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid rgb(255,0,0);}");
    resetToDefaultPathButton = new QLabel(this);
    resetToDefaultPathButton->show();
    fileDialogButton = new QLabel(this);
    fileDialogButton->show();
    defaultProjectLabel = new QLabel(this);
    defaultProjectLabel->show();
    defaultProjectCB = new QCheckBox(this);
    defaultProjectCB->show();
    defaultProjectHelpButton = new QLabel(this);
    defaultProjectHelpButton->show();
    optionalProjectDataPanel = new OptionalProjectDataPanel(this);
    static_cast<QFrame*>(optionalProjectDataPanel)->setParent(this);
    optionalProjectDataPanel->show();
    createProjectButtonNP = new QLabel(this);
    createProjectButtonNP->show();
    cancelButtonNP = new QLabel(this);
    cancelButtonNP->show();
    }
    projectPathEditDataLabel = new QLabel(this);
    projectPathEditDataLabel->show();

}

void InnerWindowCP::createWidgetsForOpenProject(){
    if(!scrollArea)
      scrollArea = new QScrollArea(this);
    if(!projectInfoScrollArea)
        projectInfoScrollArea = new QScrollArea(this);
    if(parent->getApplication()->getConfiguration()->getNumbOfProjects() == 0)
        noProjectsLabel = new QLabel(this);
    projectLabelOP = new QLabel(this);
    projectLabelOP->show();
    projectListOP = new ProjectList(LIST_OPEN_PROJECT, this);
    static_cast<QFrame*>(projectListOP)->setParent(this);
    projectListOP->show();
    otherProjectButtonOP = new QLabel(this);
    otherProjectButtonOP->show();
    projectInfoPanelOP = new ProjectInfoPanelCP(this);
    static_cast<QFrame*>(projectInfoPanelOP)->setParent(this);
    projectInfoPanelOP->show();
    openProjectButtonOP = new QLabel(this);
    openProjectButtonOP->show();
    cancelButtonOP = new QLabel(this);
    cancelButtonOP->show();
    }

void InnerWindowCP::createWidgetsForDeleteProject(){
    if(!scrollArea)
      scrollArea = new QScrollArea(this);
    if(!projectInfoScrollArea)
        projectInfoScrollArea = new QScrollArea(this);
    if(parent->getApplication()->getConfiguration()->getNumbOfProjects() == 0)
        noProjectsLabel = new QLabel(this);
    projectLabelDP = new QLabel(this);
    projectLabelDP->show();
    projectListDP = new ProjectList(LIST_DELETE_PROJECT, this);
    static_cast<QFrame*>(projectListDP)->setParent(this);
    projectListDP->show();
    projectInfoPanelDP = new ProjectInfoPanelCP(this);
    static_cast<QFrame*>(projectInfoPanelDP)->setParent(this);
    projectInfoPanelDP->show();
    deleteProjectButtonDP = new QLabel(this);
    deleteProjectButtonDP->show();
    cancelButtonDP = new QLabel(this);
    cancelButtonDP->show();
    }

void InnerWindowCP::createLayoutForChooseProject(){
    newProjectButtonCP->setText(NEW_PROJECT_BUTTON_TEXT);
    newProjectButtonCP->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
    newProjectButtonCP->setAlignment(Qt::AlignCenter);
    newProjectButtonCP->setGeometry(CHOOSE_PROJECT_LEFT_BUTTON_OFFSET, CHOOSE_PROJECT_TOOLBAR_BUTTON_OFFSET, CHOOSE_PROJECT_BUTTON_WIDTH, CHOOSE_PROJECT_BUTTON_HEIGHT );
    openProjectButtonCP->setText(OPEN_PROJECT_BUTTON_TEXT);
    openProjectButtonCP->setGeometry(CHOOSE_PROJECT_LEFT_BUTTON_OFFSET, CHOOSE_PROJECT_TOOLBAR_BUTTON_OFFSET + CHOOSE_PROJECT_BUTTON_HEIGHT + CHOOSE_PROJECT_BUTTONS_OFFSET, CHOOSE_PROJECT_BUTTON_WIDTH, CHOOSE_PROJECT_BUTTON_HEIGHT);
    openProjectButtonCP->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
    openProjectButtonCP->setAlignment(Qt::AlignCenter);
    deleteProjectButtonCP->setText(DELETE_PROJECT_BUTTON_TEXT);
    deleteProjectButtonCP->setGeometry(CHOOSE_PROJECT_LEFT_BUTTON_OFFSET, CHOOSE_PROJECT_TOOLBAR_BUTTON_OFFSET + 2*CHOOSE_PROJECT_BUTTON_HEIGHT + 2*CHOOSE_PROJECT_BUTTONS_OFFSET, CHOOSE_PROJECT_BUTTON_WIDTH, CHOOSE_PROJECT_BUTTON_HEIGHT);
    deleteProjectButtonCP->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
    deleteProjectButtonCP->setAlignment(Qt::AlignCenter);
    recentPanel->setGeometry(CHOOSE_PROJECT_LEFT_BUTTON_OFFSET + CHOOSE_PROJECT_BUTTON_WIDTH + CHOOSE_PROJECT_BUTTON_RECENT_OFFSET, 0, RECENT_PANEL_WIDTH, RECENT_PANEL_HEIGHT(5));
 }

void InnerWindowCP::createLayoutForNewProject(){
    if(*editProjectData){
        if(*editableData){
            projectNameTextEdit->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET,NEW_PROJECT_WINDOW_VERTICAL_OFFSET , NEW_PROJECT_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
            projectNameTextEdit->getDataUTextEdit()->setGeometry(0,0,NEW_PROJECT_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
            projectNameTextEdit->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
            fileDialogButton->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET + NEW_PROJECT_RIGHT_ELEMENT_WIDTH + NEW_PROJECT_SMALL_BUTTONS_HORIZONTAL_OFFSET, 2 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_SMALL_BUTTONS_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
            fileDialogButton->setPixmap(QPixmap(NEW_PROJECT_FILE_DIALOG_BUTTON_ICON));
            resetToDefaultPathButton->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET + NEW_PROJECT_RIGHT_ELEMENT_WIDTH + 2 * NEW_PROJECT_SMALL_BUTTONS_HORIZONTAL_OFFSET + NEW_PROJECT_SMALL_BUTTONS_WIDTH, 2 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_SMALL_BUTTONS_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
            resetToDefaultPathButton->setPixmap(QPixmap(NEW_PROJECT_RESET_TO_DEFAULT_PATH_BUTTON_ICON));
            editButton->setText(tr("Zakończ edycję"));
            defaultProjectCB->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET, 5 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + 4 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_SMALL_BUTTONS_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
            defaultProjectCB->setChecked(parent->getApplication()->getProjectToShow()->isDefaultProj());
            defaultProjectHelpButton->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET + NEW_PROJECT_SMALL_BUTTONS_WIDTH + NEW_PROJECT_SMALL_BUTTONS_HORIZONTAL_OFFSET, 5 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + 4 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_SMALL_BUTTONS_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
            defaultProjectHelpButton->setPixmap(QPixmap(NEW_PROJECT_DEFAULT_PROJECT_HELP_BUTTON_ICON));
            defaultProjectLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET,  5 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + 4 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_LEFT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
            defaultProjectLabel->setText(NEW_PROJECT_DEFAULT_PROJECT_LABEL_TEXT);
            }else{
            defaultProjectLabel->setText(NEW_PROJECT_DEFAULT_PROJECT_LABEL_TEXT + ((parent->getApplication()->getProjectToShow()->isDefaultProj())? "Tak" : "Nie"));
            defaultProjectLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET, 5 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + 4 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_LEFT_ELEMENT_WIDTH + 40, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
           projectNameEditDataLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET, NEW_PROJECT_WINDOW_VERTICAL_OFFSET, NEW_PROJECT_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
           projectPathEditDataLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET, 2 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
           editButton->setText(tr("Edytuj"));
     }
     projectNameLabel->setText(NEW_PROJECT_PROJECT_NAME_TEXT);
    projectNameLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET, NEW_PROJECT_WINDOW_VERTICAL_OFFSET, NEW_PROJECT_LEFT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    projectPathLabel->setText(NEW_PROJECT_PROJECT_PATH_TEXT);
    projectPathLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT + 2 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET, NEW_PROJECT_LEFT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
     projectCreatedLabel->setText(NEW_PROJECT_CREATED_LABEL_TEXT);
     projectCreatedLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET, 2 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT + 3 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET, NEW_PROJECT_LEFT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
     projectCreatedDateLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET, 2 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT + 3 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET, NEW_PROJECT_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
     projectModifiedLabel->setText(NEW_PROJECT_MODIFIED_LABEL_TEXT);
     projectModifiedLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET, 3 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT + 4 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET, NEW_PROJECT_LEFT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
     projectModifiedDateLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET, 3 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT + 4 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET, NEW_PROJECT_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
     optionalProjectDataPanel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET, 5*NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT + 6*NEW_PROJECT_WINDOW_VERTICAL_OFFSET, NEW_PROJECT_OPTIONAL_WIDTH, NEW_PROJECT_OPTIONAL_HEIGHT);
    //createProjectButtonNP->setIcon(NEW_PROJECT_CREATE_BUTTON_ICON);
     editButton->setGeometry(NEW_PROJECT_BUTTONS_HORIZONTAL_OFFSET, 5 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT + 6 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + NEW_PROJECT_OPTIONAL_HEIGHT, NEW_PROJECT_BUTTONS_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    editButton->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
    editButton->setAlignment(Qt::AlignCenter);
    finalizeButton->setText(NEW_PROJECT_FINALIZE_BUTTON_TEXT);
    //cancelButtonNP->setIcon(NEW_PROJECT_CANCEL_BUTTON_ICON);
    finalizeButton->setAlignment(Qt::AlignCenter);
    finalizeButton->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
    finalizeButton->setGeometry(2 * NEW_PROJECT_BUTTONS_HORIZONTAL_OFFSET+ NEW_PROJECT_BUTTONS_WIDTH, 5 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT + 6 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + NEW_PROJECT_OPTIONAL_HEIGHT , NEW_PROJECT_BUTTONS_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    }else{
    projectNameLabel->setText(NEW_PROJECT_PROJECT_NAME_TEXT);
    projectNameLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET, NEW_PROJECT_WINDOW_VERTICAL_OFFSET, NEW_PROJECT_LEFT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    projectNameTextEdit->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET,NEW_PROJECT_WINDOW_VERTICAL_OFFSET , NEW_PROJECT_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    projectNameTextEdit->getDataUTextEdit()->setGeometry(0,0,NEW_PROJECT_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    projectNameTextEdit->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
    projectPathLabel->setText(NEW_PROJECT_PROJECT_PATH_TEXT);
    projectPathLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT + 2 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET, NEW_PROJECT_LEFT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    projectPathEditDataLabel->setText(parent->getApplication()->getConfiguration()->getDefaultPath() + "OPCalc\\Projects\\");
    fileDialogButton->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET + NEW_PROJECT_RIGHT_ELEMENT_WIDTH + NEW_PROJECT_SMALL_BUTTONS_HORIZONTAL_OFFSET, 2 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_SMALL_BUTTONS_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    fileDialogButton->setPixmap(QPixmap(NEW_PROJECT_FILE_DIALOG_BUTTON_ICON));
    resetToDefaultPathButton->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET + NEW_PROJECT_RIGHT_ELEMENT_WIDTH + 2 * NEW_PROJECT_SMALL_BUTTONS_HORIZONTAL_OFFSET + NEW_PROJECT_SMALL_BUTTONS_WIDTH, 2 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_SMALL_BUTTONS_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    resetToDefaultPathButton->setPixmap(QPixmap(NEW_PROJECT_RESET_TO_DEFAULT_PATH_BUTTON_ICON));
    defaultProjectLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET, 3 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + 2 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_LEFT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    defaultProjectLabel->setText(NEW_PROJECT_DEFAULT_PROJECT_LABEL_TEXT);
    defaultProjectCB->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET, 3 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + 2 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_SMALL_BUTTONS_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    defaultProjectCB->setChecked(true);
    defaultProjectHelpButton->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET + NEW_PROJECT_SMALL_BUTTONS_WIDTH + NEW_PROJECT_SMALL_BUTTONS_HORIZONTAL_OFFSET, 3 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + 2 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_SMALL_BUTTONS_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    defaultProjectHelpButton->setPixmap(QPixmap(NEW_PROJECT_DEFAULT_PROJECT_HELP_BUTTON_ICON));
    optionalProjectDataPanel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET, 3 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT + 4 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET, NEW_PROJECT_OPTIONAL_WIDTH, NEW_PROJECT_OPTIONAL_HEIGHT);
    createProjectButtonNP->setText(NEW_PROJECT_CREATE_BUTTON_TEXT);
    //createProjectButtonNP->setIcon(NEW_PROJECT_CREATE_BUTTON_ICON);
    createProjectButtonNP->setGeometry(NEW_PROJECT_BUTTONS_HORIZONTAL_OFFSET, 3 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT + 5 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + NEW_PROJECT_OPTIONAL_HEIGHT, NEW_PROJECT_BUTTONS_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    createProjectButtonNP->setAlignment(Qt::AlignCenter);
    createProjectButtonNP->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
    cancelButtonNP->setText(NEW_PROJECT_CANCEL_BUTTON_TEXT);
    //cancelButtonNP->setIcon(NEW_PROJECT_CANCEL_BUTTON_ICON);
    cancelButtonNP->setAlignment(Qt::AlignCenter);
    cancelButtonNP->setGeometry(2 * NEW_PROJECT_BUTTONS_HORIZONTAL_OFFSET+ NEW_PROJECT_BUTTONS_WIDTH, 3 * NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT + 5 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + NEW_PROJECT_OPTIONAL_HEIGHT , NEW_PROJECT_BUTTONS_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
    cancelButtonNP->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
    }
    projectPathEditDataLabel->installEventFilter(innerWindowCPCatchEvents);
    projectPathEditDataLabel->setGeometry(NEW_PROJECT_WINDOW_LEFT_OFFSET + NEW_PROJECT_LEFT_ELEMENT_WIDTH + NEW_PROJECT_WINDOW_MIDDLE_OFFSET, 2 * NEW_PROJECT_WINDOW_VERTICAL_OFFSET + NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT, NEW_PROJECT_RIGHT_ELEMENT_WIDTH, NEW_PROJECT_WINDOW_ELEMENTS_HEIGHT);
 }

void InnerWindowCP::createLayoutForOpenProject(){
  projectLabelOP->setText(OPEN_PROJECT_LIST_INFO_TEXT);
  projectLabelOP->setAlignment(Qt::AlignCenter);
  projectLabelOP->setStyleSheet("font-family: \"Comic Sans MS\"; font-weight: normal; font-size: 14px;");
  projectLabelOP->setGeometry(OPEN_PROJECT_WINDOW_LEFT_OFFSET, 0, OPEN_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH, OPEN_PROJECT_LABEL_BUTTON_HEIGHT);
  projectListOP->setGeometry(OPEN_PROJECT_WINDOW_LEFT_OFFSET, OPEN_PROJECT_LABEL_BUTTON_HEIGHT + OPEN_PROJECT_LABEL_LIST_OFFSET, OPEN_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH, PROJECT_LIST_HEIGHT(*(projectListOP->getNumbOfElements())));
  otherProjectButtonOP->setText(OPEN_PROJECT_OTHER_BUTTON_TEXT);
  otherProjectButtonOP->setAlignment(Qt::AlignCenter);
  otherProjectButtonOP->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
  otherProjectButtonOP->setGeometry(OPEN_PROJECT_WINDOW_LEFT_OFFSET + (OPEN_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH - OPEN_PROJECT_OTHER_PROJECT_BUTTON_WIDTH) * 0.5, OPEN_PROJECT_VERTICAL_OFFSET + OPEN_PROJECT_LABEL_LIST_OFFSET + OPEN_PROJECT_LABEL_BUTTON_HEIGHT + 350, OPEN_PROJECT_OTHER_PROJECT_BUTTON_WIDTH, OPEN_PROJECT_LABEL_BUTTON_HEIGHT);
  projectInfoPanelOP->setGeometry(OPEN_PROJECT_WINDOW_LEFT_OFFSET + OPEN_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH + OPEN_PROJECT_WINDOW_MIDDLE_OFFSET, 0, OPEN_PROJECT_PROJECT_INFO_PANEL_WIDTH, OPEN_PROJECT_PROJECT_INFO_PANEL_HEIGHT);
  projectInfoScrollArea->setStyleSheet("background-color: transparent;");
  projectInfoScrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{background: transparent; height: 5px;} QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal, QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal, QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal{border: none; background: none; color: none;} QScrollBar::handle:horizontal{background-color: white; border: 1px solid black;}");
  projectInfoScrollArea->setFrameShape(QFrame::NoFrame);
  projectInfoScrollArea->setWidget(projectInfoPanelOP);
  projectInfoScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  projectInfoScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  projectInfoScrollArea->show();
  projectInfoScrollArea->setGeometry(OPEN_PROJECT_WINDOW_LEFT_OFFSET + OPEN_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH + OPEN_PROJECT_WINDOW_MIDDLE_OFFSET, 0, OPEN_PROJECT_PROJECT_INFO_PANEL_WIDTH, OPEN_PROJECT_PROJECT_INFO_PANEL_HEIGHT + 5);
  openProjectButtonOP->setText(OPEN_PROJECT_OPEN_BUTTON_TEXT);
  openProjectButtonOP->setAlignment(Qt::AlignCenter);
  openProjectButtonOP->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
  //openProjectButtonOP->setIcon(OPEN_PROJECT_OPEN_BUTTON_ICON);
  openProjectButtonOP->setGeometry(OPEN_PROJECT_WINDOW_LEFT_OFFSET + OPEN_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH + OPEN_PROJECT_WINDOW_MIDDLE_OFFSET + OPEN_PROJECT_DECIDE_BUTTONS_OFFSET, OPEN_PROJECT_VERTICAL_OFFSET + OPEN_PROJECT_LABEL_BUTTON_HEIGHT + OPEN_PROJECT_LABEL_LIST_OFFSET + PROJECT_LIST_HEIGHT(5), OPEN_PROJECT_DECIDE_BUTTONS_WIDTH, OPEN_PROJECT_LABEL_BUTTON_HEIGHT);
  cancelButtonOP->setText(OPEN_PROJECT_CANCEL_BUTTON_TEXT);
  cancelButtonOP->setAlignment(Qt::AlignCenter);
  cancelButtonOP->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
  //cancelButtonOP->setIcon(OPEN_PROJECT_CANCEL_BUTTON_ICON);
  cancelButtonOP->setGeometry(OPEN_PROJECT_WINDOW_LEFT_OFFSET + OPEN_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH + OPEN_PROJECT_WINDOW_MIDDLE_OFFSET + 2 * OPEN_PROJECT_DECIDE_BUTTONS_OFFSET + OPEN_PROJECT_DECIDE_BUTTONS_WIDTH,OPEN_PROJECT_VERTICAL_OFFSET + OPEN_PROJECT_LABEL_BUTTON_HEIGHT + OPEN_PROJECT_LABEL_LIST_OFFSET + PROJECT_LIST_HEIGHT(5), OPEN_PROJECT_DECIDE_BUTTONS_WIDTH, OPEN_PROJECT_LABEL_BUTTON_HEIGHT);
  if(noProjectsLabel){
      noProjectsLabel->setGeometry(OPEN_PROJECT_WINDOW_LEFT_OFFSET, OPEN_PROJECT_LABEL_BUTTON_HEIGHT + OPEN_PROJECT_LABEL_LIST_OFFSET, OPEN_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH, PROJECT_LIST_HEIGHT(5));
      noProjectsLabel->setAlignment(Qt::AlignCenter);
      noProjectsLabel->setText(QString("Brak projektów"));
      noProjectsLabel->setStyleSheet("font-family: \"Comic Sans MS\"; font-weight: normal; font-size: 12px;");
      noProjectsLabel->show();
  }
  if(scrollArea){
      scrollArea->setStyleSheet("background-color: transparent;");
      scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white; border: 1px solid black;}");
      scrollArea->setFrameShape(QFrame::NoFrame);
      scrollArea->setWidget(projectListOP);
      scrollArea->setGeometry(OPEN_PROJECT_WINDOW_LEFT_OFFSET, OPEN_PROJECT_LABEL_BUTTON_HEIGHT + OPEN_PROJECT_LABEL_LIST_OFFSET, OPEN_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH + 5, PROJECT_LIST_HEIGHT(5));
      scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
      scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      scrollArea->show();
  }
}

void InnerWindowCP::createLayoutForDeleteProject(){
  projectLabelDP->setText(DELETE_PROJECT_LIST_INFO_TEXT);
  projectLabelDP->setGeometry(DELETE_PROJECT_WINDOW_LEFT_OFFSET, 0, DELETE_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH, DELETE_PROJECT_LABEL_BUTTON_HEIGHT);
  projectLabelDP->setAlignment(Qt::AlignCenter);
  projectLabelDP->setStyleSheet("font-family: \"Comic Sans MS\"; font-weight: normal; font-size: 14px;");
  projectListDP->setGeometry(DELETE_PROJECT_WINDOW_LEFT_OFFSET, DELETE_PROJECT_LABEL_BUTTON_HEIGHT + DELETE_PROJECT_LABEL_LIST_OFFSET, DELETE_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH, PROJECT_LIST_HEIGHT(*(projectListDP->getNumbOfElements())));
  projectInfoPanelDP->setGeometry(DELETE_PROJECT_WINDOW_LEFT_OFFSET + DELETE_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH + DELETE_PROJECT_WINDOW_MIDDLE_OFFSET, 0, DELETE_PROJECT_PROJECT_INFO_PANEL_WIDTH, DELETE_PROJECT_PROJECT_INFO_PANEL_HEIGHT);
  projectInfoScrollArea->setStyleSheet("background-color: transparent;");
  projectInfoScrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{background: transparent; height: 5px;} QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal, QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal, QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal{border: none; background: none; color: none;} QScrollBar::handle:horizontal{background-color: white; border: 1px solid black;}");
  projectInfoScrollArea->setFrameShape(QFrame::NoFrame);
  projectInfoScrollArea->setWidget(projectInfoPanelDP);
  projectInfoScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  projectInfoScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  projectInfoScrollArea->show();
  projectInfoScrollArea->setGeometry(DELETE_PROJECT_WINDOW_LEFT_OFFSET + DELETE_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH + DELETE_PROJECT_WINDOW_MIDDLE_OFFSET, 0, DELETE_PROJECT_PROJECT_INFO_PANEL_WIDTH, DELETE_PROJECT_PROJECT_INFO_PANEL_HEIGHT + 5);
  deleteProjectButtonDP->setAlignment(Qt::AlignCenter);
  deleteProjectButtonDP->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
  deleteProjectButtonDP->setText(DELETE_PROJECT_DELETE_BUTTON_TEXT);
  //deleteProjectButtonDP->setIcon(DELETE_PROJECT_DELETE_BUTTON_ICON);
  deleteProjectButtonDP->setGeometry(DELETE_PROJECT_WINDOW_LEFT_OFFSET + DELETE_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH + DELETE_PROJECT_WINDOW_MIDDLE_OFFSET + DELETE_PROJECT_DECIDE_BUTTONS_OFFSET, DELETE_PROJECT_VERTICAL_OFFSET + DELETE_PROJECT_LABEL_BUTTON_HEIGHT + DELETE_PROJECT_LABEL_LIST_OFFSET + PROJECT_LIST_HEIGHT(5), DELETE_PROJECT_DECIDE_BUTTONS_WIDTH, DELETE_PROJECT_LABEL_BUTTON_HEIGHT);
  cancelButtonDP->setText(DELETE_PROJECT_CANCEL_BUTTON_TEXT);
  cancelButtonDP->setAlignment(Qt::AlignCenter);
  cancelButtonDP->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
  //cancelButtonDP->setIcon(DELETE_PROJECT_CANCEL_BUTTON_ICON);
  cancelButtonDP->setGeometry(DELETE_PROJECT_WINDOW_LEFT_OFFSET + DELETE_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH + DELETE_PROJECT_WINDOW_MIDDLE_OFFSET + 2 * DELETE_PROJECT_DECIDE_BUTTONS_OFFSET + DELETE_PROJECT_DECIDE_BUTTONS_WIDTH, DELETE_PROJECT_VERTICAL_OFFSET + DELETE_PROJECT_LABEL_BUTTON_HEIGHT + DELETE_PROJECT_LABEL_LIST_OFFSET + PROJECT_LIST_HEIGHT(5), DELETE_PROJECT_DECIDE_BUTTONS_WIDTH, DELETE_PROJECT_LABEL_BUTTON_HEIGHT);
  if(noProjectsLabel){
      noProjectsLabel->setGeometry(OPEN_PROJECT_WINDOW_LEFT_OFFSET, OPEN_PROJECT_LABEL_BUTTON_HEIGHT + OPEN_PROJECT_LABEL_LIST_OFFSET, OPEN_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH, PROJECT_LIST_HEIGHT(5));
      noProjectsLabel->setAlignment(Qt::AlignCenter);
      noProjectsLabel->setText(QString("Brak projektów"));
      noProjectsLabel->setStyleSheet("font-family: \"Comic Sans MS\"; font-weight: normal; font-size: 12px;");
      noProjectsLabel->show();
  }
  if(scrollArea){
      scrollArea->setStyleSheet("background-color: transparent;");
      scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white; padding: 3px 0px 0px 3px; border: 1px solid black;}");
      scrollArea->setFrameShape(QFrame::NoFrame);
      scrollArea->setWidget(projectListDP);
      scrollArea->setGeometry(DELETE_PROJECT_WINDOW_LEFT_OFFSET, DELETE_PROJECT_LABEL_BUTTON_HEIGHT + DELETE_PROJECT_LABEL_LIST_OFFSET, DELETE_PROJECT_PROJECT_LIST_PLUS_INFO_WIDTH + 5, PROJECT_LIST_HEIGHT(5));
      scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
      scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      scrollArea->show();
  }
}

void InnerWindowCP::deleteWidgetsForChooseProject(){
  delete newProjectButtonCP;
  newProjectButtonCP = nullptr;
  delete openProjectButtonCP;
  openProjectButtonCP = nullptr;
  delete deleteProjectButtonCP;
  deleteProjectButtonCP = nullptr;
  delete recentPanel;
  recentPanel = nullptr;
}

void InnerWindowCP::deleteWidgetsForNewProject(){
  if(*editProjectData){
      if(!(*editableData)){
          delete projectNameTextEdit;
          projectNameTextEdit = nullptr;
          delete resetToDefaultPathButton;
          resetToDefaultPathButton = nullptr;
          delete fileDialogButton;
          fileDialogButton = nullptr;
          delete defaultProjectCB;
          defaultProjectCB = nullptr;
          delete defaultProjectHelpButton;
          defaultProjectHelpButton = nullptr;
      }else{
          delete projectNameEditDataLabel;
          projectNameEditDataLabel = nullptr;
      }
      delete projectCreatedLabel;
      projectCreatedLabel = nullptr;
      delete projectCreatedDateLabel;
      projectCreatedDateLabel = nullptr;
      delete projectModifiedLabel;
      projectModifiedLabel = nullptr;
      delete projectModifiedDateLabel;
      projectModifiedDateLabel = nullptr;
      delete defaultProjectLabel;
      defaultProjectLabel = nullptr;
     delete projectNameLabel;
    projectNameLabel = nullptr;
    delete projectPathLabel;
    projectPathLabel = nullptr;
    delete optionalProjectDataPanel;
    optionalProjectDataPanel = nullptr;
    delete editButton;
    editButton = nullptr;
    delete finalizeButton;
    finalizeButton = nullptr;
  }else{
      delete projectNameLabel;
      projectNameLabel = nullptr;
      delete projectNameTextEdit;
      projectNameTextEdit = nullptr;
      delete projectPathLabel;
      projectPathLabel = nullptr;
      delete resetToDefaultPathButton;
      resetToDefaultPathButton = nullptr;
      delete fileDialogButton;
      fileDialogButton = nullptr;
      delete defaultProjectLabel;
      defaultProjectLabel = nullptr;
      delete defaultProjectCB;
      defaultProjectCB = nullptr;
      delete defaultProjectHelpButton;
      defaultProjectHelpButton = nullptr;
      delete optionalProjectDataPanel;
      optionalProjectDataPanel = nullptr;
      delete createProjectButtonNP;
      createProjectButtonNP = nullptr;
      delete cancelButtonNP;
      cancelButtonNP = nullptr;
  }
  delete projectPathEditDataLabel;
  projectPathEditDataLabel = nullptr;
}

void InnerWindowCP::deleteWidgetsForOpenProject(){
  delete projectListOP;
  projectListOP = nullptr;
  delete scrollArea;
  scrollArea = nullptr;
  delete projectLabelOP;
  projectLabelOP = nullptr;
  delete otherProjectButtonOP;
  otherProjectButtonOP = nullptr;
  delete projectInfoPanelOP;
  projectInfoPanelOP = nullptr;
  delete openProjectButtonOP;
  openProjectButtonOP = nullptr;
  delete cancelButtonOP;
  cancelButtonOP = nullptr;
  if(noProjectsLabel != nullptr)
      delete noProjectsLabel;
  noProjectsLabel = nullptr;
  if(projectInfoScrollArea){
      delete projectInfoScrollArea;
      projectInfoScrollArea = nullptr;
  }
  }

void InnerWindowCP::deleteWidgetsForDeleteProject(){
    delete projectListDP;
    projectListDP = nullptr;
    delete scrollArea;
    scrollArea = nullptr;
    delete projectInfoPanelDP;
    projectInfoPanelDP = nullptr;
    delete projectLabelDP;
    projectLabelDP = nullptr;
    delete deleteProjectButtonDP;
    deleteProjectButtonDP = nullptr;
    delete cancelButtonDP;
    cancelButtonDP = nullptr;
    if(noProjectsLabel != nullptr)
        delete noProjectsLabel;
    noProjectsLabel = nullptr;
    if(projectInfoScrollArea){
        delete projectInfoScrollArea;
        projectInfoScrollArea = nullptr;
    }
}

void InnerWindowCP::connectAll(){
  switch((uint) *innerWinStatCP){
    case (uint)CHOOSE_PROJECT_INNER:
      connectChooseProject();
      break;
    case (uint)NEW_PROJECT:
      connectNewProject();
      break;
    case (uint)OPEN_PROJECT:
      connectOpenProject();
      break;
    case (uint)DELETE_PROJECT:
      connectDeleteProject();
      break;
    }

}

void InnerWindowCP::connectChooseProject(){
  newProjectButtonCP->installEventFilter(innerWindowCPCatchEvents);
  openProjectButtonCP->installEventFilter(innerWindowCPCatchEvents);
  deleteProjectButtonCP->installEventFilter(innerWindowCPCatchEvents);
  }

void InnerWindowCP::connectNewProject(){
  if(*editProjectData){
   if(*editableData){
       projectNameTextEdit->getDataUTextEdit()->installEventFilter(innerWindowCPCatchEvents);
       resetToDefaultPathButton->installEventFilter(innerWindowCPCatchEvents);
       fileDialogButton->installEventFilter(innerWindowCPCatchEvents);
       defaultProjectHelpButton->installEventFilter(innerWindowCPCatchEvents);
       }
   if(defaultProjectCB != nullptr)
        defaultProjectCB->installEventFilter(innerWindowCPCatchEvents);
   editButton->installEventFilter(innerWindowCPCatchEvents);
   finalizeButton->installEventFilter(innerWindowCPCatchEvents);
  }else{
    projectNameTextEdit->getDataUTextEdit()->installEventFilter(innerWindowCPCatchEvents);
    resetToDefaultPathButton->installEventFilter(innerWindowCPCatchEvents);
    fileDialogButton->installEventFilter(innerWindowCPCatchEvents);
    defaultProjectHelpButton->installEventFilter(innerWindowCPCatchEvents);
    defaultProjectCB->installEventFilter(innerWindowCPCatchEvents);
    createProjectButtonNP->installEventFilter(innerWindowCPCatchEvents);
    cancelButtonNP->installEventFilter(innerWindowCPCatchEvents);
  }
}

void InnerWindowCP::connectOpenProject(){
    openProjectButtonOP->installEventFilter(innerWindowCPCatchEvents);
    otherProjectButtonOP->installEventFilter(innerWindowCPCatchEvents);
    cancelButtonOP->installEventFilter(innerWindowCPCatchEvents);
}

void InnerWindowCP::connectDeleteProject(){
  deleteProjectButtonDP->installEventFilter(innerWindowCPCatchEvents);
  cancelButtonDP->installEventFilter(innerWindowCPCatchEvents);
}

void InnerWindowCP::disconnectAll(){
  switch((uint) *((*innerChange == true)? innerWinStatCP : (innerWinStatCP + 1))){
    case (uint)CHOOSE_PROJECT_INNER:
      disconnectChooseProject();
      break;
    case (uint)NEW_PROJECT:
      disconnectNewProject();
      break;
    case (uint)OPEN_PROJECT:
      disconnectOpenProject();
      break;
    case (uint)DELETE_PROJECT:
      disconnectDeleteProject();
      break;
    }
}

void InnerWindowCP::disconnectChooseProject(){

}

void InnerWindowCP::disconnectNewProject(){
    if(*editProjectData){
        if(*editableData)
            *editableData = false;
        else
            *editableData = true;
    }
 }

void InnerWindowCP::disconnectOpenProject(){

}

void InnerWindowCP::disconnectDeleteProject(){

}

AppWindow* InnerWindowCP::getParent(){
    return parent;
}

void InnerWindowCP::newProjectButtonCPReleased(){
    WINDOW_STATUS_SAVE(innerWinStatCP)
    *innerWinStatCP = NEW_PROJECT;
    innerWinStatusChanged = true;
}

void InnerWindowCP::openProjectButtonCPReleased(){
    WINDOW_STATUS_SAVE(innerWinStatCP)
    *innerWinStatCP = OPEN_PROJECT;
    innerWinStatusChanged = true;
}

void InnerWindowCP::deleteProjectButtonCPReleased(){
    WINDOW_STATUS_SAVE(innerWinStatCP)
    *innerWinStatCP = DELETE_PROJECT;
    innerWinStatusChanged = true;
}

void InnerWindowCP::createProjectButtonNPReleased(){
    if(checkProjectNameToSave() && checkProjectPath()){
    parent->getApplication()->addProjectInfo();
    parent->getApplication()->setProjectToShow(*parent->getApplication()->getProjectInfo());
    (*parent->getApplication()->getProjectInfo())->createNewProject();
    parent->getTabBar()->reload();
    parent->setWinStat(CALC_ENTER_PARAMS);
    }
}

void InnerWindowCP::cancelButtonNPReleased(){
    WINDOW_STATUS_SAVE(innerWinStatCP)
    *innerWinStatCP = CHOOSE_PROJECT_INNER;
    innerWinStatusChanged = true;
}

void InnerWindowCP::otherProjectButtonOPReleased(){
    QString path;
    if(!((path = QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Wybierz folder zapisu:"), QDir::homePath(), QString("OPC Projects(*") + QString(PROJECT_FILE_EXTENSTION) + QString(")")))).isNull()))
    {
        int projectNumb = 0;
        if((projectNumb = parent->getApplication()->getConfiguration()->findPath(path)) != 0){
            if(projectNumb == -1)
                return;
            ushort *recentNumb = nullptr;
            parent->getApplication()->addProjectInfo();
            parent->getApplication()->getConfiguration()->readProject(projectNumb, &recentNumb);
            for(uint j = 1; j < (*parent->getApplication()->getProjectInfo())->getPosition(); j++)
                if((uint)projectNumb == *(*(parent->getApplication()->getProjectInfo() + j))->getProjectNumb()){
                parent->getApplication()->removeProjectInfo((*parent->getApplication()->getProjectInfo())->getPosition());
                parent->getApplication()->setProjectToShow((*(parent->getApplication()->getProjectInfo() + j - 1)));
                break;
            }
            if(parent->getApplication()->getProjectToShow() == nullptr){
                int ans = ProjectMenager::readProject(*parent->getApplication()->getProjectInfo());
                switch(ans){
                case -1:
                case -2:
                    parent->getApplication()->removeProjectInfo((*parent->getApplication()->getProjectInfo())->getPosition());
                    delete recentNumb;
                    recentNumb = nullptr;
                    return;
                default:
                    break;
                }
               parent->getApplication()->setProjectToShow(*parent->getApplication()->getProjectInfo());
               parent->getApplication()->getConfiguration()->rewriteRecents(*projectListOP->getSelectedProjectNumb(), recentNumb, (recentNumb)? 1 : 0, false, false);
               }
            delete recentNumb;
            recentNumb = nullptr;
        }else{
            parent->getApplication()->addProjectInfo();
            (*parent->getApplication()->getProjectInfo())->setPath(new QString(path));
            int ans = ProjectMenager::readProject((*parent->getApplication()->getProjectInfo()));
            switch(ans)
            {
             case -2:
                (*projectListOP->getProjectListElementOP() + projectNumb - 1)->removeButtonReleased();
             case -1:
                parent->getApplication()->removeProjectInfo((*parent->getApplication()->getProjectInfo())->getPosition());
                return;
            default:
                break;
            }
            (*parent->getApplication()->getProjectInfo())->setProjectNumb(0);
            parent->getApplication()->setProjectToShow(*parent->getApplication()->getProjectInfo());
        }
         parent->getTabBar()->reload();
        *innerChange = true;
        parent->setWinStat(CALC_ENTER_PARAMS);
    }

}

void InnerWindowCP::openProjectButtonOPReleased(){
    if(projectListOP != nullptr && projectListOP->getSelectedProjectNumb() != nullptr){
        ushort *recentNumb = nullptr;
        parent->getApplication()->addProjectInfo();
        parent->getApplication()->getConfiguration()->readProject(*projectListOP->getSelectedProjectNumb(), &recentNumb);
        for(uint j = 1; j < (*parent->getApplication()->getProjectInfo())->getPosition(); j++){
           if(*((*parent->getApplication()->getProjectInfo())->getProjectNumb()) == *(*(parent->getApplication()->getProjectInfo() + j))->getProjectNumb())
           {
               parent->getApplication()->removeProjectInfo((*parent->getApplication()->getProjectInfo())->getPosition());
               parent->getApplication()->setProjectToShow((*(parent->getApplication()->getProjectInfo() + j - 1)));
               break;
           }
        }
       if(parent->getApplication()->getProjectToShow() == nullptr){
           int ans = ProjectMenager::readProject(*parent->getApplication()->getProjectInfo());
          switch(ans)
          {
          case -2:
              parent->getApplication()->removeProjectInfo((*parent->getApplication()->getProjectInfo())->getPosition());
              delete recentNumb;
              recentNumb = nullptr;
              (*projectListOP->getProjectListElementOP() + *projectListOP->getSelectedProjectNumb() - 1)->removeButtonReleased();
              return;
          case -1:
              parent->getApplication()->removeProjectInfo((*parent->getApplication()->getProjectInfo())->getPosition());
              delete recentNumb;
              recentNumb = nullptr;
              return;
          default:
              break;
          }
          parent->getApplication()->setProjectToShow(*parent->getApplication()->getProjectInfo());
          parent->getApplication()->getConfiguration()->rewriteRecents(*projectListOP->getSelectedProjectNumb(), recentNumb, (recentNumb)? 1 : 0, false, false);
       }
       delete recentNumb;
       recentNumb = nullptr;
        parent->getTabBar()->reload();
        *innerChange = true;
        WINDOW_STATUS_SAVE(parent->getWinStat())
        parent->setWinStat(CALC_ENTER_PARAMS);

  }
}

void InnerWindowCP::cancelButtonOPReleased(){
    WINDOW_STATUS_SAVE(innerWinStatCP)
    *innerWinStatCP = CHOOSE_PROJECT_INNER;
    innerWinStatusChanged = true;
}

void InnerWindowCP::deleteProjectButtonDPReleased(){
  uint *projects = nullptr;
    ushort *recents = nullptr;
    uint numberOfProjects = 0;
    ushort numberOfRecents = 0;
    for(uint i = 0; i < *projectListDP->getNumbOfElements(); i++)
        if((*projectListDP->getProjectListElementDP() + i)->getSelectBox()->isChecked()){
            if(projects != nullptr){
                numberOfProjects++;
                uint *temp = new uint[numberOfProjects];
                for(uint j = 0; j < (numberOfProjects - 1); j++)
                *(temp + j) = *(projects + j);
                *(temp + numberOfProjects - 1) = i + 1;
                delete []projects;
                projects = temp;
            }else{
                projects = new uint[1];
                numberOfProjects++;
                *projects = i + 1;
            }
        }
    if(numberOfProjects != 0){
        uint *positions = nullptr;
        uint numbOfPositions = 0;
        uint *tempProjects = new uint[numberOfProjects];
        for(uint i = 0;  i < numberOfProjects; i++){
            *(tempProjects + i) = *(projects + i);
        }
        if(parent->getApplication()->getProjectInfo() != nullptr){
            for(uint i = 0; i < numberOfProjects; i++){
                for(uint j = 0; j < (*parent->getApplication()->getProjectInfo())->getPosition(); j++){
                    if(*(tempProjects + i) < *(*(parent->getApplication()->getProjectInfo() + j))->getProjectNumb())
                        (*(parent->getApplication()->getProjectInfo() + j))->setProjectNumb(*(*(parent->getApplication()->getProjectInfo() + j))->getProjectNumb() - 1);
                    else
                    {
                        if(*(tempProjects + i) == *(*(parent->getApplication()->getProjectInfo() + j))->getProjectNumb())
                        {
                            if(positions != nullptr){
                                uint *temp = new uint[numbOfPositions + 1];
                                for(uint k = 0; k < numbOfPositions; k++){
                                    *(temp + k) = *(positions + k);
                                }
                                *(temp + numbOfPositions) = (*(parent->getApplication()->getProjectInfo() + j))->getPosition();
                                delete []positions;
                                positions = temp;
                                numbOfPositions++;
                            }else{
                                positions = new uint[++numbOfPositions];
                                *positions = (*(parent->getApplication()->getProjectInfo() + j))->getPosition();
                                (*(parent->getApplication()->getProjectInfo() + j))->setProjectNumb(0);
                            }
                        }
                    }
                }
                for(uint j = i + 1; j < numberOfProjects; j++){
                    if(*(tempProjects + i) < *(tempProjects + j)){
                        (*(tempProjects + j))--;
                    }
                }
            }
            delete []tempProjects;
            if(numbOfPositions != 0){
                for(uint i = 0; i < numbOfPositions; i++){
                    parent->getApplication()->removeProjectInfo(*(positions + i));
                    for(uint j = i + 1; j < numbOfPositions; j++)
                        if(*(positions + i) < *(positions + j))
                            (*(positions + j))--;
                }
                delete []positions;
            }
            parent->getApplication()->getMainWin()->getTabBar()->reload();
        }
        parent->getApplication()->getConfiguration()->removeProjects(projects, numberOfProjects, &recents, &numberOfRecents);
        parent->getApplication()->getConfiguration()->rewriteRecents(0, recents, numberOfRecents, true, false);
        delete []recents;
        recents = nullptr;
        delete []projects;
        projects = nullptr;
        WINDOW_STATUS_SAVE(innerWinStatCP)
                *innerWinStatCP = CHOOSE_PROJECT_INNER;
        innerWinStatusChanged = true;
    }
}

void InnerWindowCP::cancelButtonDPReleased(){
    WINDOW_STATUS_SAVE(innerWinStatCP)
    *innerWinStatCP = CHOOSE_PROJECT_INNER;
    innerWinStatusChanged = true;
}

void InnerWindowCP::reloadTimerTask(){
    if(innerWinStatusChanged == true)
        reload();
}

void InnerWindowCP::reload(){
    reloadTimer->stop();
    innerWinStatusChanged = false;
    if(!(*innerChange))
        parent->getTitleWidget()->setTitleWinStatus(*innerWinStatCP);
    disconnectAll();
    deleteWidgets();
    createWidgets();
    createLayout();
    connectAll();
    switch((uint) *innerWinStatCP){
    case (uint) CHOOSE_PROJECT_INNER:
        if(parent->getApplication()->getConfiguration()->getNumbOfRecentProjects() != 0)
        parent->getApplication()->getConfiguration()->readForRecentList();
        break;
    case (uint) NEW_PROJECT:
        if(*editProjectData)
        parent->getApplication()->getProjectToShow()->readForEditData();
        break;
    case (uint) OPEN_PROJECT:
        parent->getApplication()->getConfiguration()->readForProjectList(true);
        break;
    case (uint) DELETE_PROJECT:
        parent->getApplication()->getConfiguration()->readForProjectList(false);
        break;
    }
    show();
    *innerChange = false;
    reloadTimer->start();
}

InnerWinStatusCP* InnerWindowCP::getInnerWinStatCP(){
    return innerWinStatCP;
}

ProjectInfoPanelCP* InnerWindowCP::getProjectInfoPanelOP(){
    return projectInfoPanelOP;
}

ProjectInfoPanelCP* InnerWindowCP::getProjectInfoPanelDP(){
    return projectInfoPanelDP;
}

ProjectList* InnerWindowCP::getProjectListOP(){
  return projectListOP;
}

ProjectList* InnerWindowCP::getProjectListDP(){
  return projectListDP;
}

void InnerWindowCP::setInnerWinStatCP(InnerWinStatusCP setStat){
    WINDOW_STATUS_SAVE(innerWinStatCP)
    *innerWinStatCP = setStat;
    innerWinStatusChanged = true;
}

void InnerWindowCP::checkStatusToUse(){
    if(parent->getApplication()->getStatusToUse() != nullptr){
        WINDOW_STATUS_SAVE(innerWinStatCP)
        *innerWinStatCP = *parent->getApplication()->getStatusToUse();
        parent->getApplication()->removeStatusToUse();
    }
}

QScrollArea* InnerWindowCP::getScrollArea(){
    return scrollArea;
}

RecentPanel* InnerWindowCP::getRecentPanel(){
    return recentPanel;
}

FormatableDataField<ProjectMenagerParam>* InnerWindowCP::getProjectNameTextEdit(){
  return projectNameTextEdit;
}

QLabel* InnerWindowCP::getProjectNameLabel(){
    return projectNameLabel;
}

QLabel* InnerWindowCP::getProjectPathLabel(){
    return projectPathLabel;
}

OptionalProjectDataPanel* InnerWindowCP::getOptionalProjectDataPanel(){
  return optionalProjectDataPanel;
}

bool* InnerWindowCP::getEditableData(){
    return editableData;
}

bool* InnerWindowCP::getEditProjectData(){
    return editProjectData;
}

QCheckBox* InnerWindowCP::getDefaultProjectCB(){
    return defaultProjectCB;
}

bool InnerWindowCP::checkProjectNameTextEdit(QKeyEvent* ev){
    uint len = projectNameTextEdit->getDataUTextEdit()->text().length();
    if(len > 30){
        switch(ev->key()){
        case Qt::Key_Left:
            return false;
        case Qt::Key_Right:
            return false;
        case Qt::Key_Backspace:
            projectNameTextEdit->getDataUTextEdit()->backspace();
            break;
        case Qt::Key_Delete:
            projectNameTextEdit->getDataUTextEdit()->del();
            break;
        default:
            return true;
        }
    }
    else{
        switch(ev->key()){
        case Qt::Key_Left:
            return false;
        case Qt::Key_Right:
            return false;
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
        case Qt::Key_Alt:
            return true;
        case Qt::Key_Shift:
            return true;
        case Qt::Key_Control:
            return true;
        case Qt::Key_Space:
            projectNameTextEdit->getDataUTextEdit()->insert(ev->text());
            break;
        case Qt::Key_Backspace:
            projectNameTextEdit->getDataUTextEdit()->backspace();
            break;
        case Qt::Key_Delete:
            projectNameTextEdit->getDataUTextEdit()->del();
            break;
        default:
            projectNameTextEdit->getDataUTextEdit()->insert(ev->text());
            break;
        }
    }
    projectNameTextEdit->multiSpaceKeyDetection();
    if(projectNameTextEdit->getDataUTextEdit()->text().isEmpty() || projectNameTextEdit->getDataUTextEdit()->text().length() > 30)
        projectNameTextEdit->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
    else
        projectNameTextEdit->getDataUTextEdit()->setStyleSheet("");
    return true;
}

void InnerWindowCP::fillProjectPath(){
    uint i = projectPathEditDataLabel->text().length() - 1;
    while(projectPathEditDataLabel->text().at(i) != '\\')
        i--;
    auto tempStr = projectPathEditDataLabel->text();
    tempStr.chop(projectPathEditDataLabel->text().length() - i - 1);
    projectPathEditDataLabel->setText(tempStr + ((projectNameTextEdit->getDataUTextEdit()->text().isEmpty())? QString() : (projectNameTextEdit->getDataUTextEdit()->text() + PROJECT_FILE_EXTENSTION)));
}

void InnerWindowCP::resetToDefaultPathButtonReleased(){
    projectPathEditDataLabel->setText(parent->getApplication()->getConfiguration()->getDefaultPath() + "OPCalc\\Projects\\" + ((!projectNameTextEdit->getDataUTextEdit()->text().isEmpty())? projectNameTextEdit->getDataUTextEdit()->text() + PROJECT_FILE_EXTENSTION : QString("")));
}

void InnerWindowCP::fileDialogButtonReleased(){
    QString projectPath = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Wybierz folder zapisu:"), parent->getApplication()->getConfiguration()->getDefaultPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
    if(!projectPath.isNull())
    {
        if(projectPath.at(projectPath.length() - 1) != '\\')
            projectPathEditDataLabel->setText(projectPath + ((!projectNameTextEdit->getDataUTextEdit()->text().isEmpty())? ("\\" + projectNameTextEdit->getDataUTextEdit()->text() + PROJECT_FILE_EXTENSTION) : QString("\\")));
        else
            projectPathEditDataLabel->setText(projectPath + ((!projectNameTextEdit->getDataUTextEdit()->text().isEmpty())? (projectNameTextEdit->getDataUTextEdit()->text() + PROJECT_FILE_EXTENSTION) : QString("")));
    }
}

void InnerWindowCP::defaultProjectHelpButtonReleased(){
    MyDialog dlg(QUESTION_DIALOG, QString("Projekt domyślny"), QString("Gdy projekt jest domyślny oznacza, że jest przechowywany w lokalnym folderze zapisu programu i nie można zmienić jego ścieżki zapisu."), false, QString("Zamknij"), QString(), QString(), nullptr);
    dlg.setWindowModality(Qt::ApplicationModal);
    dlg.exec();
}

bool InnerWindowCP::checkProjectNameToSave(){
    if(projectNameTextEdit->getDataUTextEdit()->text().isEmpty()){
        MyDialog dlg(QUESTION_DIALOG, QString("Brak nazwy projektu"), QString("Podaj nazwę projektu."), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
    }else{
        return true;
    }
}

bool InnerWindowCP::checkProjectPath(){
    if(QFile(projectPathEditDataLabel->text()).exists()){
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_NEW_PATH_FILE_EXIST_TITLE), QString(ERROR_NEW_PATH_FILE_EXIST_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
    }else{
        return true;
    }
}

void InnerWindowCP::editButtonReleased(){
    if(*editableData){
    if(checkProjectNameToSave()){
    if(!App::compareStr(*parent->getApplication()->getProjectToShow()->getProjectName(), projectNameTextEdit->getDataUTextEdit()->text()) || !App::compareStr(*parent->getApplication()->getProjectToShow()->getPath(), projectPathEditDataLabel->text()) || !App::compareStr((parent->getApplication()->getProjectToShow()->getFlangeName() != nullptr)? *parent->getApplication()->getProjectToShow()->getFlangeName() : QString(""), optionalProjectDataPanel->getFlangeTextEdit()->getDataUTextEdit()->text()) || !App::compareStr((parent->getApplication()->getProjectToShow()->getObjectName() != nullptr)? *parent->getApplication()->getProjectToShow()->getObjectName() : QString(""), optionalProjectDataPanel->getObjectTextEdit()->getDataUTextEdit()->text()) || !App::compareStr((parent->getApplication()->getProjectToShow()->getCompanyName() != nullptr)? *parent->getApplication()->getProjectToShow()->getCompanyName() : QString(""), optionalProjectDataPanel->getCompanyTextEdit()->getDataUTextEdit()->text()) || defaultProjectCB->isChecked() != parent->getApplication()->getProjectToShow()->isDefaultProj()){
    if(!App::compareStr(*parent->getApplication()->getProjectToShow()->getPath(), projectPathEditDataLabel->text()))
    if(!ProjectMenager::moveProject(parent->getApplication()->getProjectToShow(), projectPathEditDataLabel->text())){
        return;
    }
    parent->getApplication()->getProjectToShow()->rewriteConfigData();
    parent->getTabBar()->reload();
    parent->getApplication()->getConfiguration()->rewriteProject((*parent->getApplication()->getProjectInfo())->getPosition() - parent->getApplication()->getProjectToShow()->getPosition());
    ProjectMenager::writeProject(parent->getApplication()->getProjectToShow());
    ProjectMenager::rewriteForConfigFile(new QFile(*parent->getApplication()->getProjectToShow()->getPath()));
    }
    *innerChange = true;
    innerWinStatusChanged = true;
    return;
    }
    }else{
    *innerChange = true;
    innerWinStatusChanged = true;
    }
}

void InnerWindowCP::finalizeButtonReleased(){
    if(parent->getApplication()->getProjectToShow() != nullptr)
    {
        *innerChange = true;
        parent->setWinStat(CALC_ENTER_PARAMS);
        innerWinStatusChanged = true;
        return;
    }
    WINDOW_STATUS_SAVE(innerWinStatCP)
    *innerWinStatCP = CHOOSE_PROJECT_INNER;
    innerWinStatusChanged = true;
}

bool* InnerWindowCP::getInnerChange(){
    return innerChange;
}

QLabel* InnerWindowCP::getNewProjectButtonCP(){
    return newProjectButtonCP;
}

QLabel* InnerWindowCP::getOpenProjectButtonCP(){
    return openProjectButtonCP;
}

QLabel* InnerWindowCP::getDeleteProjectButtonCP(){
    return deleteProjectButtonCP;
}

QLabel* InnerWindowCP::getCreateProjectButtonNP(){
    return createProjectButtonNP;
}

QLabel* InnerWindowCP::getCancelButtonNP(){
    return cancelButtonNP;
}

QLabel* InnerWindowCP::getOpenProjectButtonOP(){
    return openProjectButtonOP;
}

QLabel* InnerWindowCP::getOtherButtonOP(){
    return otherProjectButtonOP;
}

QLabel* InnerWindowCP::getCancelButtonOP(){
    return cancelButtonOP;
}

QLabel* InnerWindowCP::getDeleteProjectButtonDP(){
    return deleteProjectButtonDP;
}

QLabel* InnerWindowCP::getCancelButtonDP(){
    return cancelButtonDP;
}

QLabel* InnerWindowCP::getEditButton(){
    return editButton;
}

QLabel* InnerWindowCP::getFinalizeButton(){
    return finalizeButton;
}

QLabel* InnerWindowCP::getResetToDefaultPathButton(){
    return resetToDefaultPathButton;
}

QLabel* InnerWindowCP::getFileDialogButton(){
    return fileDialogButton;
}

QLabel* InnerWindowCP::getDefaultProjectHelpButton(){
    return defaultProjectHelpButton;
}

QLabel* InnerWindowCP::getDefaultProjectHelpPopUp(){
    return defaultProjectHelpPopUp;
}

InnerWindowCPCatchEvents* InnerWindowCP::getInnerWindowCPCatchEvents(){
    return innerWindowCPCatchEvents;
}

void InnerWindowCP::setEditableData(bool set){
    *editableData = set;
}

void InnerWindowCP::setInnerChange(bool set){
    *innerChange = set;
}

void InnerWindowCP::setProjectNameEditDataLabel(QString set){
    projectNameEditDataLabel->setText(set);
}

void InnerWindowCP::setProjectPathEditDataLabel(QString set){
    projectPathEditDataLabel->setText(set);
}

void InnerWindowCP::setProjectCreatedDateLabel(QString set){
    projectCreatedDateLabel->setText(set);
}

void InnerWindowCP::setProjectModifiedDateLabel(QString set){
    projectModifiedDateLabel->setText(set);
}

void InnerWindowCP::setProjectNameTextEdit(QString set){
    projectNameTextEdit->getDataUTextEdit()->setText(set);
}

void InnerWindowCP::setDefaulProjectCB(bool set){
    defaultProjectCB->setChecked(set);
}

QLabel* InnerWindowCP::getDefaultProjectLabel(){
    return defaultProjectLabel;
}

void InnerWindowCP::deleteDefaultProjectPopUp(){
    defaultProjectHelpPopUp->deleteLater();
}

void InnerWindowCP::defaultProjectCBReleased(){
    if(!defaultProjectCB->isChecked())
        resetToDefaultPathButtonReleased();
}

QLabel* InnerWindowCP::getProjectPathEditDataLabel(){
    return projectPathEditDataLabel;
}
