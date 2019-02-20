#ifndef PANELCEPELEMENT_HPP
#define PANELCEPELEMENT_HPP
#include"innerwinstatuscepenum.hpp"
#include<QFrame>
class PanelCEP;
class QLabel;
class PanelCEPElement : public QFrame{
    Q_OBJECT
public:
  PanelCEPElement(PanelCEP*, uint);
  virtual ~PanelCEPElement();
 protected:
  // Parent
  PanelCEP* parent = nullptr;

  //Numer porządkowy panelu (nazwa stanu mu odpowiadającemu)
  InnerWinStatusCEP numberOfPanel = (InnerWinStatusCEP)0;

  // Elementy okna
  QLabel* infoLabel = nullptr;

  // Funkcje sterujące
  void init();
  void setWindow();
  void createWidgets();
  void createLayout();
  void deleteWidgets();
  void connectAll();
  void disconnectAll();

 public:
  QLabel* getInfoLabel();
  uint getNumberOfPanel();
  void setNumber(InnerWinStatusCEP);
  virtual void chooseFlangeTypeButtonReleased();
};

#endif // PANELCEPELEMENT_HPP
