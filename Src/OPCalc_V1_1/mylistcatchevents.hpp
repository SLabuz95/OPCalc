#ifndef MYLISTCATCHEVENTS_HPP
#define MYLISTCATCHEVENTS_HPP
#include<QObject>
#include<QTimer>

class MyList;
class MyListCatchEvents : public QObject{
    Q_OBJECT
public:
    // Konstruktor
    MyListCatchEvents(MyList* setParent);

    // Destruktor
    ~MyListCatchEvents();

public slots:
    void pressedSetFalse();

private:
    // Parent
    MyList* parent = nullptr;
    bool pressed = false;
    QTimer timer;
    QObject *timerObj = nullptr;

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

};

#endif // MYLISTCATCHEVENTS_HPP
