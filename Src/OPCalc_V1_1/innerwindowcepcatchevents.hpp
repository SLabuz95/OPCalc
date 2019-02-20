#ifndef INNERWINDOWCEPCATCHEVENTS_HPP
#define INNERWINDOWCEPCATCHEVENTS_HPP
#include<QObject>
#include"projectmenagerparamenum.hpp"
class QObject;
class InnerWindowCEP;
template <typename ParamName> class FormatableDataField;
class InnerWindowCEPCatchEvents : public QObject{
public:
    // Konstruktor
    InnerWindowCEPCatchEvents(InnerWindowCEP*);

    // Destruktor
    ~InnerWindowCEPCatchEvents();

    // Wywołanie zdarzenia QTimera
    inline void mouseButtonReleasedSetFalse();

private:
    InnerWindowCEP * parent = nullptr;
    bool mouseButtonReleased = false;
    FormatableDataField<ProjectMenagerParam> *ptrToFormatableDataField = nullptr;
protected:
    bool eventFilter(QObject*, QEvent*);
};
#endif // INNERWINDOWCEPCATCHEVENTS_HPP
