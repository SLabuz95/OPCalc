#ifndef MYPALLETE_HPP
#define MYPALLETE_HPP
#include"resultsgraphicpage.hpp"

#define DIMLINE_TEXT_OFFSET (5)
#define DIMLINE_OBJECT_OFFSET (4)
#define PAINT_OFFSET (5)
#define ARROW_WIDTH (5)
#define ARROW_HEIGHT (10)
#define KOL_OFFSET (8)
#define KOL_WIDTH (10)

class MyPallete : public QWidget{
public:
    MyPallete(ResultsGraphicPage *parent);

    ResultsGraphicPage *parent = nullptr;
    void paint(QPainter &p);
    void paintEvent(QPaintEvent *ev);
};

#endif // MYPALLETE_HPP
