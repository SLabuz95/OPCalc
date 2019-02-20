#ifndef PANELCEPELEMENTCLICKED_HPP
#define PANELCEPELEMENTCLICKED_HPP
#include<QObject>

class PanelCEP;
class PanelCEPCatchEvents : public QObject{
    public:
  // Konstruktor
    PanelCEPCatchEvents(PanelCEP*);
private:
    PanelCEP* parent = nullptr;
    //
    // Nadpisanie Event Filter
protected:
    bool eventFilter(QObject*, QEvent*);
public:
    // Dostęp do metody Event Filter dla innych celów
    void start();
};

#endif // PANELCEPELEMENTCLICKED_HPP
