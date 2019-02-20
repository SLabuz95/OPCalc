#ifndef MATERIALSCHANGESWIDGETCATCHEVENTS_HPP
#define MATERIALSCHANGESWIDGETCATCHEVENTS_HPP
#include<QObject>
#include"flangematerialsfileparams.hpp"
#include"pipematerialsfileparams.hpp"
template <typename ParamName> class FormatableDataField;
template <typename ParamName> class MaterialsChangesWidget;
template <typename ParamName>
class MaterialsChangesWidgetCatchEvents : public QObject{
public:
    // Kontruktor
    MaterialsChangesWidgetCatchEvents(MaterialsChangesWidget<ParamName>*);

    // Destruktor
    ~MaterialsChangesWidgetCatchEvents();

private:
    // Parent
    MaterialsChangesWidget<ParamName> *parent = nullptr;
    FormatableDataField<ParamName> *ptrToFormatableDataField = nullptr;
protected:
    // EventFilter
    bool eventFilter(QObject*, QEvent*);
};

#endif // MATERIALSCHANGESWIDGETCATCHEVENTS_HPP
