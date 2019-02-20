#ifndef PANELCEP_HPP
#define PANELCEP_HPP
#include<QFrame>
class PanelCEPElement;
class PanelCEPCatchEvents;
class AppWindow;
class PanelCEP : public QFrame{
  Q_OBJECT
 public:
  // Konstruktor
  PanelCEP(AppWindow*);

  // Destruktor
  ~PanelCEP();

 private:
  // Parent
  AppWindow *parent = nullptr;

  // Elementy okna
  PanelCEPElement **panelCEPElement = nullptr;

  // Obiekt obsługujący zdarzenia
  PanelCEPCatchEvents *catchEvents = nullptr;

  // Funkcje sterujące
  void init();
  void setWindow();
  void createWidgets();
  void createLayout();
  void deleteWidgets();
  void connectAll();
  void disconnectAll();
public:
  AppWindow* getParent();
  PanelCEPElement **getPanelCEPElement();
  PanelCEPCatchEvents* getCatchEvents();
  void saveInnerWinCEPData();
  void reload();
};

#endif // PANELCEP_HPP
