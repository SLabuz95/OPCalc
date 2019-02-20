#ifndef PROJECTINFOPANELCEP_HPP
#define PROJECTINFOPANELCEP_HPP

#include<QFrame>
class QLabel;
class AppWindow;
class ProjectInfoPanelCEPCatchEvents;
class ProjectInfoPanelCEP : public QFrame{
    Q_OBJECT

public:
    // Konstruktor
    ProjectInfoPanelCEP(AppWindow*);

    // Destruktor
    ~ProjectInfoPanelCEP();
private:
    // Parent
    AppWindow* parent = nullptr;

    // CatchEvents
    ProjectInfoPanelCEPCatchEvents *projectInfoPanelCEPCatchEvents = nullptr;

    // Elementy okna
    QLabel *projectNameLabel = nullptr;
    QLabel *infoButton = nullptr;

    // Funkcje sterujące
    void init();
    void setWindow();
    void createWidgets();
    void createLayout();
    void deleteWidgets();
    void connectAll();
    void disconnectAll();

   public:
    QLabel* getInfoButton();
    void infoButtonReleased();
    AppWindow* getParent();

};

#endif // PROJECTINFOPANELCEP_HPP
