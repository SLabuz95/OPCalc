#ifndef PANELCEPELEMENTFLANGETYPE_HPP
#define PANELCEPELEMENTFLANGETYPE_HPP
#include"panelcepelement.hpp"
class PanelCEP;
class PanelCEPElementFlangeType : public PanelCEPElement{
  public:
    PanelCEPElementFlangeType(PanelCEP*, uint);
    virtual ~PanelCEPElementFlangeType();

protected:
    // Funkcje sterujące
    void init();
    void setWindow();
    void createWidgets();
    void createLayout();
    void deleteWidgets();
    void connectAll();
    void disconnectAll();

    // Elementy
    QLabel *chooseFlangeTypeButton = nullptr;

public:
    QLabel* getChooseFlangeTypeButton();

    virtual void chooseFlangeTypeButtonReleased();

};

#endif // PANELCEPELEMENTFLANGETYPE_HPP
