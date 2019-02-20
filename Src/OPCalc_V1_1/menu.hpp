#ifndef MENU_HPP
#define MENU_HPP
#include<QObject>
class QMenu;
class QAction;
class InnerWindowCP;
class AppWindow;

class Menu : public QObject{
    Q_OBJECT
public:
    // Konstruktor
    Menu(AppWindow*);

    // Destruktor
    ~Menu();
private:
    // Parent
    AppWindow *parent = nullptr;

    // Elementy menu dla poszczegolnych stanów:
    // - NIEZALEŻNE:
    // 1.1.FileMenu
    QMenu *fileMenu = nullptr;
    // 1.2.Actions dla fileMenu
    QAction *chooseProjectAction = nullptr;
    QAction *newProjectAction = nullptr;
    QAction *openProjectAction = nullptr;
    QAction *deleteProjectAction = nullptr;
    QAction *quitAction = nullptr;
    // 2.1.ToolsMenu
    QMenu *toolsMenu = nullptr;
    // 2.2.Actions dla ToolsMenu
    QAction *optionsAction = nullptr;
    // 3.1.HelpMenu
    QMenu *helpMenu = nullptr;
    // 3.2.Actions dla HelpMenu
    QAction *helpAction = nullptr;
    QAction *programInformationAction = nullptr;
    QAction *authorsAction = nullptr;

    // - CALC_ENTER_PARAMS:
    // 1.FileMenu - Actions
    QAction *saveAction = nullptr;
    QAction *closeProjectAction = nullptr;
    
    // Funkcje sterujace
    void init();
    void createWidgets();
    void createLayout();
    void addActionsToMenu();
    void addActionsToMenuAtStart();
    void deleteWidgets();
    void connectAllAtStart();
    void connectAll();
    void disconnectAll();
    void disconnectAllToDelete();

 public:
    void setMenuBar();
    void reloadMenu();

public slots:
    void chooseProjectActionTriggered();
    void newProjectActionTrigerred();
    void openProjectActionTrigerred();
    void deleteProjectActionTrigerred();
    void optionsActionTrigerred();
    void helpActionTrigerred();
    void saveActionTrigerred();
    void closeProjectActionTrigerred();
    void programInformationActionTriggered();
    void authorsActionTriggered();

    // Funkcje obsługi zdarzeń

};

#endif // MENU_HPP
