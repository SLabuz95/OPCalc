#ifndef FORMATCHANGEPOPUP_HPP
#define FORMATCHANGEPOPUP_HPP
#include<QFrame>
class QLabel;
class QComboBox;
template <typename ParamName> class FormatableDataField;
template <typename ParamName> class FormatChangePopUpCatchEvents;
template <typename ParamName> class FormatChangePopUp : public QFrame{

public:
  // Konstruktory
    FormatChangePopUp(FormatableDataField<ParamName>*);

  // Destruktory
    ~FormatChangePopUp();

private:
    // Event Filter
    FormatChangePopUpCatchEvents<ParamName> *catchEvents = nullptr;

    // Modyfikowany element
    FormatableDataField<ParamName> *modifiedElement = nullptr;

    // Elementy okna
    QLabel *closeButton = nullptr;
    QLabel *precisionLabel = nullptr;
    QLabel *formatLabel = nullptr;
    QLabel *decrementButton = nullptr;
    QLabel *incrementButton = nullptr;
    QLabel *precisionDisplayLabel = nullptr;
    QComboBox *formatList = nullptr;

    // Metody zarządzania oknem
    void init();
    void setWindow();
    void createWidgets();
    void createLayout();
    void deleteWidgets();
    void connectAll();
    void disconnectAll();

public:
    QLabel* getCloseButton();
    QLabel* getDecrementButton();
    QLabel* getIncrementButton();
    QLabel* getPrecisionDisplayLabel();
    QComboBox* getFormatList();
    FormatChangePopUpCatchEvents<ParamName>* getFormatChangePopUpCatchEvents();
    void decrement();
    void increment();
    void formatChanged();
    void changeFormatType();

    // Paint Event
    void paintEvent(QPaintEvent*);
};

#endif // FORMATCHANGEPOPUP_HPP
