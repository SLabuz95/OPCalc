#ifndef INNERWINDOWCPCATCHEVENTS_HPP
#define INNERWINDOWCPCATCHEVENTS_HPP
#include<QObject>
class InnerWindowCP;
class InnerWindowCPCatchEvents : public QObject{
public:
    // Konstruktor
    InnerWindowCPCatchEvents(InnerWindowCP*);

    // Destruktor
    ~InnerWindowCPCatchEvents();
private:
    // Parent
    InnerWindowCP *parent = nullptr;

protected:
    bool eventFilter(QObject*, QEvent*);

};

#endif // INNERWINDOWCPCATCHEVENTS_HPP
