#ifndef FORMATCHANGEPOPUPCATCHEVENTS_HPP
#define FORMATCHANGEPOPUPCATCHEVENTS_HPP
#include<QObject>
class QObject;
class QEvent;
template <typename ParamName> class FormatChangePopUp;
template <typename ParamName> class FormatChangePopUpCatchEvents : public QObject{
public:
    // Konstruktor
    FormatChangePopUpCatchEvents(FormatChangePopUp<ParamName>*);
    ~FormatChangePopUpCatchEvents();
private:
    // Parent
    FormatChangePopUp<ParamName> *parent = nullptr;
protected:
    // Reimplementacja EventFilter
    bool eventFilter(QObject*, QEvent*);
};
#endif // FORMATCHANGEPOPUPCATCHEVENTS_HPP

