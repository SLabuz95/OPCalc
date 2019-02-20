#ifndef INNERWINDOWCP_HPP
#define INNERWINDOWCP_HPP
#include<QFrame>
#include"innerwinstatuscpenum.hpp"
#include"projectmenagerparamenum.hpp"
template <typename ParamName>
class FormatableDataField;
class QLabel;
class QLineEdit;
class QCheckBox;
class QScrollArea;
class AppWindow;
class RecentPanel;
class OptionalProjectDataPanel;
class ProjectList;
class ProjectInfoPanelCP;
class InnerWindowCPCatchEvents;

class InnerWindowCP : public QFrame{
    Q_OBJECT

    // Okno nadrzędne
    AppWindow *parent = nullptr;

    // Stany okna
    InnerWinStatusCP *innerWinStatCP = nullptr;
    bool innerWinStatusChanged = false;

    // Obsługa zdarzeń InnerWindowCPCatchEvents
    InnerWindowCPCatchEvents *innerWindowCPCatchEvents = nullptr;

    // Timer dla zmian stanów
    QTimer *reloadTimer = nullptr;

    // Elementy dla stanow okna
    // - CHOOSE_PROJECT
    QLabel *newProjectButtonCP = nullptr;
    QLabel *openProjectButtonCP = nullptr;
    QLabel *deleteProjectButtonCP = nullptr;
    RecentPanel *recentPanel = nullptr;

    // - NEW_PROJECT (Wersja. Wprowadzanie danych dla nowego projektu)
    QLabel *projectNameLabel = nullptr;
    FormatableDataField<ProjectMenagerParam> *projectNameTextEdit = nullptr;
    QLabel *projectPathLabel = nullptr;
    QLabel *resetToDefaultPathButton = nullptr;
    QLabel *fileDialogButton = nullptr;
    QLabel *defaultProjectLabel = nullptr;
    QCheckBox *defaultProjectCB = nullptr;
    QLabel *defaultProjectHelpButton = nullptr;
    QLabel *defaultProjectHelpPopUp = nullptr;
    OptionalProjectDataPanel *optionalProjectDataPanel = nullptr;
    QLabel *createProjectButtonNP = nullptr;
    QLabel *cancelButtonNP = nullptr;
    QScrollArea *projectInfoScrollArea = nullptr;

    // - NEW_PROJECT (Wersja. Odczytu danych oraz ich zmianych)
    //QLabel *projectNameLabel = nullptr;
    //QLabel *projectPathLabel = nullptr;
    //OptionalProjectDataPanel *optionalProjectDataPanel = nullptr;
    QLabel *projectNameEditDataLabel = nullptr;
    QLabel *projectPathEditDataLabel = nullptr;
    QLabel *projectCreatedLabel = nullptr;
    QLabel *projectModifiedLabel = nullptr;
    QLabel *projectCreatedDateLabel = nullptr;
    QLabel *projectModifiedDateLabel = nullptr;
    //FormatableDataField<ProjectMenagerParam> *projectNameTextEdit = nullptr;
    //QLabel *projectPathTextEdit = nullptr;
    //QLabel *resetToDefaultPathButton = nullptr;
    QLabel *editButton = nullptr;
    QLabel *saveButton = nullptr;
    QLabel *finalizeButton = nullptr;

    // - OPEN_PROJECT
    QScrollArea* scrollArea = nullptr;
    QLabel *noProjectsLabel = nullptr;
    QLabel *projectLabelOP = nullptr;
    ProjectList *projectListOP = nullptr;
    QLabel *otherProjectButtonOP = nullptr;
    ProjectInfoPanelCP *projectInfoPanelOP = nullptr;
    QLabel *openProjectButtonOP = nullptr;
    QLabel *cancelButtonOP = nullptr;

    // - DELETE_PROJECT
    // QScrollArea* scrollArea = nullptr;
    QLabel *projectLabelDP = nullptr;
    ProjectList *projectListDP = nullptr;
    ProjectInfoPanelCP *projectInfoPanelDP =nullptr;
    QLabel *deleteProjectButtonDP = nullptr;
    QLabel *cancelButtonDP = nullptr;

    //Konstruktor
public:
    InnerWindowCP(AppWindow*, bool, bool);

    // Destruktor
    ~InnerWindowCP();

    // Zmienna określająca czy NewProject ma zmieniać dane istniejącego projektu
    bool *editProjectData = nullptr;
    bool *editableData = nullptr;
    bool *innerChange = nullptr;

    //Metody zarządzania oknem
private:
    void init();
    void initAndStartTimers();
    void createWidgets();
    void createLayout();
    void deleteWidgets();
    void createWidgetsForChooseProject();
    void createWidgetsForNewProject();
    void createWidgetsForOpenProject();
    void createWidgetsForDeleteProject();
    void createLayoutForChooseProject();
    void createLayoutForNewProject();
    void createLayoutForOpenProject();
    void createLayoutForDeleteProject();
    void deleteWidgetsForChooseProject();
    void deleteWidgetsForNewProject();
    void deleteWidgetsForOpenProject();
    void deleteWidgetsForDeleteProject();
    void connectAll();
    void connectChooseProject();
    void connectNewProject();
    void connectOpenProject();
    void connectDeleteProject();
    void disconnectAll();
    void disconnectChooseProject();
    void disconnectNewProject();
    void disconnectOpenProject();
    void disconnectDeleteProject();
    void setWindow();
    void checkStatusToUse();

public slots:
    void reloadTimerTask();

public:
    void newProjectButtonCPReleased();
    void openProjectButtonCPReleased();
    void deleteProjectButtonCPReleased();
    void createProjectButtonNPReleased();
    void cancelButtonNPReleased();
    void otherProjectButtonOPReleased();
    void openProjectButtonOPReleased();
    void cancelButtonOPReleased();
    void deleteProjectButtonDPReleased();
    void cancelButtonDPReleased();
    void reload();
    bool checkProjectPath();
    bool checkProjectNameToSave();
    void editButtonReleased();
    void saveButtonReleased();
    void finalizeButtonReleased();
    bool checkProjectNameTextEdit(QKeyEvent*);
    void fillProjectPath();
    void resetToDefaultPathButtonReleased();
    void fileDialogButtonReleased();
    void defaultProjectHelpButtonReleased();
    void defaultProjectHelpPopUpLeaved();
    void defaultProjectCBReleased();

    //Funkcje get
    AppWindow* getParent();
    InnerWinStatusCP* getInnerWinStatCP();
    ProjectInfoPanelCP* getProjectInfoPanelOP();
    ProjectInfoPanelCP* getProjectInfoPanelDP();
    QScrollArea* getScrollArea();
    RecentPanel* getRecentPanel();
    ProjectList* getProjectListOP();
    ProjectList* getProjectListDP();
    FormatableDataField<ProjectMenagerParam>* getProjectNameTextEdit();
    OptionalProjectDataPanel* getOptionalProjectDataPanel();
    bool* getEditableData();
    bool* getEditProjectData();
    bool* getInnerChange();
    QLabel* getNewProjectButtonCP();
    QLabel* getOpenProjectButtonCP();
    QLabel* getDeleteProjectButtonCP();
    QLabel* getCreateProjectButtonNP();
    QLabel* getCancelButtonNP();
    QLabel* getOpenProjectButtonOP();
    QLabel* getOtherButtonOP();
    QLabel* getCancelButtonOP();
    QLabel* getDeleteProjectButtonDP();
    QLabel* getCancelButtonDP();
    QLabel* getProjectNameLabel();
    QLabel* getProjectPathLabel();
    QLabel* getEditButton();
    QLabel* getFinalizeButton();
    InnerWindowCPCatchEvents* getInnerWindowCPCatchEvents();
    QLabel* getProjectCreatedDateLabel();
    QLabel* getProjectModifiedDateLabel();
    QLabel* getResetToDefaultPathButton();
    QLabel* getFileDialogButton();
    QLabel* getDefaultProjectHelpButton();
    QLabel* getDefaultProjectHelpPopUp();
    QCheckBox* getDefaultProjectCB();
    QLabel* getProjectPathEditDataLabel();
    QLabel* getDefaultProjectLabel();

    //Funkcje set
    void setInnerWinStatCP(InnerWinStatusCP);
    void setEditableData(bool);
    void setInnerChange(bool);
    void setProjectNameEditDataLabel(QString);
    void setProjectPathEditDataLabel(QString);
    void setProjectCreatedDateLabel(QString);
    void setProjectModifiedDateLabel(QString);
    void setProjectNameTextEdit(QString);
    void setDefaulProjectCB(bool);

    // Funkcje usuwające
    void deleteDefaultProjectPopUp();
};

#endif // INNERWINDOWCP_HPP
