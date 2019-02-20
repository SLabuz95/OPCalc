#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP
// Biblioteki statndardowe QT
#include<QPushButton>
#include<QTimer>
#include<QMenuBar>
#include<QToolBar>
#include<QStatusBar>
#include<QLabel>
#include<QProgressBar>
#include<QWidget>
#include<QObject>

// Biblioteki stworzone
#include"windowmacros.hpp"

class App;
class Menu;
class ToolsForToolBar;
class InnerWindowCP;
class ProjectInfoPanelCEP;
class PanelCEP;
class InnerWindowCEP;
class TitleWidget;
class TabBar;
class PromptPanel;

enum WinStatus{
    START_LOADING,
    CHOOSE_PROJECT,
    CALC_ENTER_PARAMS,
    CALC_RESULT
};
//Klasa okna programu
class AppWindow : public QWidget{
    Q_OBJECT
  // Zmienna wyliczeniowa określająca stan okna
public:
    // Konstruktor okna
    AppWindow(App*);

    // Destruktor okna
    ~AppWindow();

private:
    //Zmienna pilnująca zmian stanu okna
    QTimer *reloadTimer = nullptr;
    QTimer *loadingTimer = nullptr;

    // Zmienna przetrzymująca adres klasy głównej programu
    App *application = nullptr;

    // Zmienna przechowująca stan okna głównego
    WinStatus *winStat = nullptr;
    // + zmienna przechowująca informacje o zmianie stanu okna
    bool winStatChanged = false;

    // ELEMENTY OKNA PROGRAMU:
    // NIEZALEŻNE
    QMenuBar *menuBar = nullptr;
    Menu *menu = nullptr;
    ToolsForToolBar *tools = nullptr;
    QToolBar *toolBar = nullptr;
    QStatusBar *statusBar = nullptr;
    TabBar *tabBar = nullptr;
    PromptPanel *promptPanel = nullptr;

    // - START_LOADING:
    QProgressBar* progressBarSL = nullptr;
    QLabel* windowIcon = nullptr;
    // + flaga informująca o ustawieniu maksymalnej wartości okna
    bool *progressBarSLMaxReady = nullptr;
    uint *progressBarSLValue = nullptr;

    // - CHOOSE_PROJECT:
    TitleWidget *titleWidget = nullptr;
    InnerWindowCP *innerWinCP = nullptr;

    // - CALC_ENTER_PARAMS:
    ProjectInfoPanelCEP *projectInfoPanelCEP = nullptr;
    PanelCEP *panelCEP = nullptr;
    InnerWindowCEP *innerWinCEP = nullptr;

    // - CALC_RESULT:
    ProjectInfoPanelCEP *projectInfoPanelCR = nullptr;
    QLabel *resultLabelCR =nullptr;

    // Funkcje zarządzające wyglądem okna
    void createMenu();
    void changeMenu();
    void deleteMenu();
    void createMenuBar();
    void changeMenuBar();
    void createToolBar();
    void deleteToolBar();
    void changeToolBar();
    void createStatusBar();
    void createWidgetsForStartLoading();
    void createWidgetsForChooseProject();
    void createWidgetsForCalcEnterParams();
    void createWidgetsForCalcResult();
    void createLayoutForStartLoading();
    void createLayoutForChooseProject();
    void createLayoutForCalcEnterParams();
    void createLayoutForCalcResult();
    void deleteWidgetsForStartLoading();
    void deleteWidgetsForChooseProject();
    void deleteWidgetsForCalcEnterParams();
    void deleteWidgetsForCalcResult();
    void createWidgets();
    void createLayout();
    void setWindow();
    void deleteWidgets();
    void configProgressBarSL();
    void reload();

    // Funkcje sterujące
    void init();
    void initAndStartTimers();
    void connectWidgets();
    void disconnectWidgets();
    void disconnectLoadingTimer();
    void deleteLoadingTimer();

public slots:
    // Fukncje sterujące Timerów
    void reloadTimerTask();
    void loadingTimerTask();

public:
    //Funkcje get
    App* getApplication();
    WinStatus* getWinStat();
    QMenuBar* getMenuBar();
    QToolBar* getToolBar();
    InnerWindowCP* getInnerWindowCP();
    Menu* getMenu();
    ToolsForToolBar* getTools();
    InnerWindowCEP* getInnerWinCEP();
    TitleWidget* getTitleWidget();
    QProgressBar* getProgressBarSL();
    TabBar* getTabBar();
    QStatusBar* getStatusBar();
    PanelCEP* getPanelCEP();
    PromptPanel* getPromptPanel();

    // Funkcje set
    void setWinStat(WinStatus);
    void setWinStatChanged(bool);
    void setProgressBarSLMaxReady(bool);
    void setProgressBarValue(uint );

    void saveAllProjects();
    void closeEvent(QCloseEvent*);
    };
#endif //APPWINDOW_HPP
