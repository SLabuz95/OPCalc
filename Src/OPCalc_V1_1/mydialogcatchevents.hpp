#ifndef MYDIALOGCATCHEVENTS_HPP
#define MYDIALOGCATCHEVENTS_HPP
#include<QObject>
class MyDialog;
class MyDialogCatchEvents : public QObject{
public:
    // Konstruktor
    MyDialogCatchEvents(MyDialog *setParent);

    // Destruktor
    ~MyDialogCatchEvents();
private:
    // Parent
    MyDialog* parent = nullptr;

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

};

#endif // MYDIALOGCATCHEVENTS_HPP
