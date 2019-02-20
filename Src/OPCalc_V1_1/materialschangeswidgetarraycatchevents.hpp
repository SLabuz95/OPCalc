#ifndef MATERIALSCHANGESWIDGETARRAYCATCHEVENTS_HPP
#define MATERIALSCHANGESWIDGETARRAYCATCHEVENTS_HPP
#include<QObject>
class MaterialsChangesWidgetArray;
class MaterialsChangesWidgetArrayCatchEvents : public QObject{
public:
    // Konstruktor
    MaterialsChangesWidgetArrayCatchEvents(MaterialsChangesWidgetArray*);

    // Destruktor
    ~MaterialsChangesWidgetArrayCatchEvents();
private:
    // Parent
    MaterialsChangesWidgetArray *parent = nullptr;

protected:
    // Event Filter
    bool eventFilter(QObject*, QEvent*);
};

#endif // MATERIALSCHANGESWIDGETARRAYCATCHEVENTS_HPP
