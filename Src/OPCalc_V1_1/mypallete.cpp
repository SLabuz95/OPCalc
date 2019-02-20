#include"mypallete.hpp"
#include"projectinfo.hpp"
#include"innerwindowcep.hpp"
#include<QPainter>
#include<QPoint>
MyPallete::MyPallete(ResultsGraphicPage *parent)
    :QWidget (parent)
{
    this->parent = parent;

}

void MyPallete::paint(QPainter &p){
    QFont font;
    QPen pen(QColor(0,0,0,255));
    font.setPointSize(8);
    // Charakterystyczne punkty
    // Linia wymiarowa idzie do granicy widgeta
    // Położenie linii wymiarowej dla minimalnych odległości od dysku
    double dimlineDeviceDistanceHeight = PAINT_OFFSET + font.pointSize() + DIMLINE_TEXT_OFFSET;

    // Położenie górnej części obudowy oraz jej wysokość
    double boxUpperCorner;
    switch(*parent->getParent()->getProjectInfo()->getMeasurementType()){
    case 0:
        boxUpperCorner = dimlineDeviceDistanceHeight + PAINT_OFFSET;
        break;
    default:
        boxUpperCorner = 2 * dimlineDeviceDistanceHeight + PAINT_OFFSET;
        break;
    }
    // Maksymalna wysokość urządzenia
    double deviceHeight = this->height() - PAINT_OFFSET - boxUpperCorner;
    double pixPerMm = deviceHeight / (1.6 * *parent->getParent()->getProjectInfo()->getPipeDiameter()->getData());
    double pipeDiameter = *parent->getParent()->getProjectInfo()->getPipeDiameter()->getData() * pixPerMm;
    double boxPartHeight = 0.3 * pipeDiameter;
    double boxPartWidth = boxPartHeight;
    double discWidth = static_cast<double>(pipeDiameter) * 0.025;
    double discDiameter = *parent->getParent()->getProjectInfo()->getFlangeDiameter()->getData() * pixPerMm;
    double boxPartOffset = 0.3 * boxPartWidth + discWidth;
    double deviceWidth = 2 * boxPartWidth + boxPartOffset;
    double upstreamPipeLength = (this->width() - 2 * PAINT_OFFSET - deviceWidth) * 0.666;
    double downStreamPipeLength = 0.5 * upstreamPipeLength;
    pen.setWidth(1);
    pen.setCapStyle(Qt::RoundCap);
    p.setPen(pen);
    p.setFont(font);
    p.setBrush(Qt::black);
    pen.setWidth(2);
    p.setPen(pen);
    // Grubsza linią bo zarys obiektu
    // LEWY GÓRNY
    p.drawLine(QLineF(PAINT_OFFSET, boxUpperCorner + boxPartHeight, PAINT_OFFSET + upstreamPipeLength + boxPartWidth, boxUpperCorner + boxPartHeight));
    p.drawLine(QLineF(PAINT_OFFSET + upstreamPipeLength + boxPartWidth, boxUpperCorner + boxPartHeight, PAINT_OFFSET + upstreamPipeLength + boxPartWidth, boxUpperCorner));
    p.drawLine(QLineF(PAINT_OFFSET + upstreamPipeLength + 0.333 * boxPartWidth, boxUpperCorner, PAINT_OFFSET + upstreamPipeLength + boxPartWidth, boxUpperCorner));
    p.drawLine(QLineF(PAINT_OFFSET + upstreamPipeLength + 0.333 * boxPartWidth, boxUpperCorner, PAINT_OFFSET + upstreamPipeLength + 0.333 * boxPartWidth, boxUpperCorner + 0.333 * boxPartHeight));
    p.drawLine(QLineF(PAINT_OFFSET + upstreamPipeLength, boxUpperCorner + boxPartHeight, PAINT_OFFSET + upstreamPipeLength, boxUpperCorner + 0.666 * boxPartHeight));
    p.drawLine(QLineF(PAINT_OFFSET + upstreamPipeLength, boxUpperCorner + 0.666 * boxPartHeight, PAINT_OFFSET + upstreamPipeLength + 0.333 * boxPartWidth, boxUpperCorner + 0.333 * boxPartHeight));
    // LEWY DOLNY
    p.drawLine(QLineF(PAINT_OFFSET, boxUpperCorner + boxPartHeight + pipeDiameter, PAINT_OFFSET + upstreamPipeLength + boxPartWidth, boxUpperCorner + boxPartHeight + pipeDiameter));
    p.drawLine(QLineF(PAINT_OFFSET + upstreamPipeLength + boxPartWidth, boxUpperCorner + boxPartHeight + pipeDiameter, PAINT_OFFSET + upstreamPipeLength + boxPartWidth, boxUpperCorner + deviceHeight));
    p.drawLine(QLineF(PAINT_OFFSET + upstreamPipeLength + 0.333 * boxPartWidth, boxUpperCorner + deviceHeight, PAINT_OFFSET + upstreamPipeLength + boxPartWidth, boxUpperCorner + deviceHeight));
    p.drawLine(QLineF(PAINT_OFFSET + upstreamPipeLength + 0.333 * boxPartWidth, boxUpperCorner + deviceHeight, PAINT_OFFSET + upstreamPipeLength + 0.333 * boxPartWidth, boxUpperCorner + 1.666 * boxPartHeight + pipeDiameter));
    p.drawLine(QLineF(PAINT_OFFSET + upstreamPipeLength, boxUpperCorner + boxPartHeight + pipeDiameter, PAINT_OFFSET + upstreamPipeLength, boxUpperCorner + pipeDiameter + 1.333 * boxPartHeight));
    p.drawLine(QLineF(PAINT_OFFSET + upstreamPipeLength, boxUpperCorner + pipeDiameter + 1.333 * boxPartHeight, PAINT_OFFSET + upstreamPipeLength + 0.333 * boxPartWidth, boxUpperCorner + pipeDiameter + 1.666 * boxPartHeight));
    // PRAWY GÓRNY
    int upRightPointX = PAINT_OFFSET + upstreamPipeLength + deviceWidth - boxPartWidth;
    p.drawLine(QLineF(upRightPointX, boxUpperCorner + boxPartHeight, upRightPointX + boxPartWidth + downStreamPipeLength, boxUpperCorner + boxPartHeight));
    p.drawLine(QLineF(upRightPointX, boxUpperCorner + boxPartHeight, upRightPointX, boxUpperCorner));
    p.drawLine(QLineF(upRightPointX, boxUpperCorner, upRightPointX + 0.666 * boxPartWidth, boxUpperCorner));
    p.drawLine(QLineF(upRightPointX + 0.666 * boxPartWidth, boxUpperCorner, upRightPointX + 0.666 * boxPartWidth, boxUpperCorner + 0.333 * boxPartHeight));
    p.drawLine(QLineF(upRightPointX + 0.666 * boxPartWidth, boxUpperCorner + 0.333 * boxPartHeight, upRightPointX + boxPartWidth, boxUpperCorner + 0.666 * boxPartHeight));
    p.drawLine(QLineF(upRightPointX + boxPartWidth, boxUpperCorner + 0.666 * boxPartHeight, upRightPointX + boxPartWidth, boxUpperCorner + boxPartHeight));
    // PRAWY DOLNY
    p.drawLine(QLineF(upRightPointX, boxUpperCorner + boxPartHeight + pipeDiameter, upRightPointX + boxPartWidth + downStreamPipeLength, boxUpperCorner + boxPartHeight + pipeDiameter));
    p.drawLine(QLineF(upRightPointX, boxUpperCorner + boxPartHeight + pipeDiameter, upRightPointX, boxUpperCorner + deviceHeight));
    p.drawLine(QLineF(upRightPointX, boxUpperCorner + deviceHeight, upRightPointX + 0.666 * boxPartWidth, boxUpperCorner + deviceHeight));
    p.drawLine(QLineF(upRightPointX + 0.666 * boxPartWidth, boxUpperCorner + deviceHeight, upRightPointX + 0.666 * boxPartWidth, boxUpperCorner + pipeDiameter + 1.666 * boxPartHeight));
    p.drawLine(QLineF(upRightPointX + 0.666 * boxPartWidth, boxUpperCorner + pipeDiameter + 1.666 * boxPartHeight, upRightPointX + boxPartWidth, boxUpperCorner + pipeDiameter + 1.333 * boxPartHeight));
    p.drawLine(QLineF(upRightPointX + boxPartWidth, boxUpperCorner + pipeDiameter + 1.333 * boxPartHeight, upRightPointX + boxPartWidth, boxUpperCorner + pipeDiameter + boxPartHeight));
    // Dysk
    upRightPointX -= boxPartOffset;
    p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5, boxUpperCorner + boxPartHeight, upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth, boxUpperCorner + boxPartHeight));
    p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth, boxUpperCorner + boxPartHeight, upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5));
    p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5, upRightPointX + (boxPartOffset - discWidth) * 0.5, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5));
    p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5, upRightPointX + (boxPartOffset - discWidth) * 0.5, boxUpperCorner + boxPartHeight));

    p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + discDiameter, upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + discDiameter));
    p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + discDiameter, upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth, boxUpperCorner + boxPartHeight + pipeDiameter));
    p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth, boxUpperCorner + boxPartHeight + pipeDiameter, upRightPointX + (boxPartOffset - discWidth) * 0.5, boxUpperCorner + boxPartHeight + pipeDiameter));
    p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5, boxUpperCorner + boxPartHeight + pipeDiameter, upRightPointX + (boxPartOffset - discWidth) * 0.5, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + discDiameter));
    pen.setWidth(1);
    p.setPen(pen);
    // Wymiar średnicy rurociągu
    p.drawLine(QLineF(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength, boxUpperCorner + boxPartHeight, PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength, boxUpperCorner + boxPartHeight + pipeDiameter));
    QPoint arrowPolygone[3] = {};
    arrowPolygone[0] = QPoint(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength, boxUpperCorner + boxPartHeight);
    arrowPolygone[1] = QPoint(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength - 0.5 * ARROW_WIDTH, boxUpperCorner + boxPartHeight + ARROW_HEIGHT);
    arrowPolygone[2] = QPoint(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength + 0.5 * ARROW_WIDTH, boxUpperCorner + boxPartHeight + ARROW_HEIGHT);
    p.drawPolygon(arrowPolygone, 3);
    arrowPolygone[0] = QPoint(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength, boxUpperCorner + boxPartHeight + pipeDiameter);
    arrowPolygone[1] = QPoint(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength - 0.5 * ARROW_WIDTH, boxUpperCorner + boxPartHeight + pipeDiameter - ARROW_HEIGHT);
    arrowPolygone[2] = QPoint(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength + 0.5 * ARROW_WIDTH, boxUpperCorner + boxPartHeight + pipeDiameter - ARROW_HEIGHT);
    p.drawPolygon(arrowPolygone, 3);
    // Dopiero po rotate
    p.rotate(90);
    p.drawText(boxUpperCorner + boxPartHeight +( boxUpperCorner + boxPartHeight + pipeDiameter - (boxUpperCorner + boxPartHeight) - this->fontMetrics().horizontalAdvance(QString::number(*parent->getParent()->getProjectInfo()->getPipeDiameter()->getData() ,'f', 2) + " mm")) * 0.5, -(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength + DIMLINE_TEXT_OFFSET) , QString::number(*parent->getParent()->getProjectInfo()->getPipeDiameter()->getData() ,'f', 2) + " mm");
    p.rotate(-90);
    // Wymiar średnicy dysku
    p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + DIMLINE_OBJECT_OFFSET, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5, upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + DIMLINE_OBJECT_OFFSET + 50, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5));
    p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + DIMLINE_OBJECT_OFFSET, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + discDiameter, upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + DIMLINE_OBJECT_OFFSET + 50, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + discDiameter));
    p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + 50 - 10, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5, upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + 50 - 10, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + discDiameter));
    arrowPolygone[0] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + 50 - 10, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5);
    arrowPolygone[1] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + 50 - 10 + 0.5 * ARROW_WIDTH, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + ARROW_HEIGHT);
    arrowPolygone[2] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + 50 - 10 - 0.5 * ARROW_WIDTH, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + ARROW_HEIGHT);
    p.drawPolygon(arrowPolygone, 3);
    arrowPolygone[0] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + 50 - 10, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + discDiameter);
    arrowPolygone[1] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + 50 - 10  + 0.5 * ARROW_WIDTH, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + discDiameter - ARROW_HEIGHT);
    arrowPolygone[2] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + 50 - 10 - 0.5 * ARROW_WIDTH, boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + discDiameter - ARROW_HEIGHT);
    p.drawPolygon(arrowPolygone, 3);
    // Dopiero po rotate
    p.rotate(90);
    p.drawText(boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + ((boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 + discDiameter) - (boxUpperCorner + boxPartHeight + (pipeDiameter - discDiameter) * 0.5 ) - this->fontMetrics().horizontalAdvance(QString::number(*parent->getParent()->getProjectInfo()->getFlangeDiameter()->getData() , 'f', 2) + " mm")) * 0.5, -(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + 50 + DIMLINE_TEXT_OFFSET ), QString::number(*parent->getParent()->getProjectInfo()->getFlangeDiameter()->getData() , 'f', 2) + " mm");
    p.rotate(-90);
    switch(*parent->getParent()->getProjectInfo()->getMeasurementType()){
    case 1:{
        // Wymiarowanie odleglosci (linie wymiarowe więc cieńsze)
        p.drawLine(QLineF(PAINT_OFFSET, boxUpperCorner + boxPartHeight - DIMLINE_OBJECT_OFFSET, PAINT_OFFSET, PAINT_OFFSET));
        p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5, boxUpperCorner + boxPartHeight - DIMLINE_OBJECT_OFFSET, upRightPointX + (boxPartOffset - discWidth) * 0.5, PAINT_OFFSET));
        p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth, boxUpperCorner + boxPartHeight - DIMLINE_OBJECT_OFFSET, upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth, PAINT_OFFSET));
        p.drawLine(QLineF(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength, boxUpperCorner + boxPartHeight - DIMLINE_OBJECT_OFFSET, upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength, PAINT_OFFSET));
        p.drawLine(QLineF(PAINT_OFFSET, dimlineDeviceDistanceHeight, upRightPointX + (boxPartOffset - discWidth) * 0.5, dimlineDeviceDistanceHeight));
        p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth, dimlineDeviceDistanceHeight, upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength, dimlineDeviceDistanceHeight));
        // Teskty wymiarowe
        p.drawText(PAINT_OFFSET+ (upRightPointX + (boxPartOffset - discWidth) * 0.5 - PAINT_OFFSET - this->fontMetrics().horizontalAdvance(QString::number(*parent->getParent()->getProjectInfo()->getUpstreamDeviceDistance()->getData(),'f', 2) + " m")) * 0.5, dimlineDeviceDistanceHeight - DIMLINE_TEXT_OFFSET, QString::number(*parent->getParent()->getProjectInfo()->getUpstreamDeviceDistance()->getData(),'f', 2) + " m" );
        p.drawText(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth +(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength - (upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth) - this->fontMetrics().horizontalAdvance(QString::number(*parent->getParent()->getProjectInfo()->getDownstreamDistance()->getData()))) * 0.5, dimlineDeviceDistanceHeight - DIMLINE_TEXT_OFFSET , QString::number(*parent->getParent()->getProjectInfo()->getDownstreamDistance()->getData(), 'f', 2) + " m");

        // Strzałki
        arrowPolygone[0] = QPoint(PAINT_OFFSET, dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(PAINT_OFFSET + ARROW_HEIGHT, dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(PAINT_OFFSET + ARROW_HEIGHT, dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5, dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 - ARROW_HEIGHT, dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 - ARROW_HEIGHT, dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth, dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + ARROW_HEIGHT, dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + discWidth + ARROW_HEIGHT, dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength, dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength - ARROW_HEIGHT, dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength - ARROW_HEIGHT, dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        // Strzałka kierunku
        p.drawLine(QLineF(PAINT_OFFSET + 0.1 * upstreamPipeLength, boxUpperCorner + boxPartHeight - 15, PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength, boxUpperCorner + boxPartHeight - 15));
        arrowPolygone[0] = QPoint(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength, boxUpperCorner + boxPartHeight - 15);
        arrowPolygone[1] = QPoint(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength - ARROW_HEIGHT, boxUpperCorner + boxPartHeight - 15 - 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength - ARROW_HEIGHT, boxUpperCorner + boxPartHeight - 15 + 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);

        // Otwory impulsowe
        pen.setStyle(Qt::DashDotLine);
        pen.setWidth(2);
        p.setPen(pen);
        p.drawLine(QLineF(upRightPointX - KOL_OFFSET - KOL_WIDTH, boxUpperCorner + boxPartHeight, upRightPointX - KOL_OFFSET - KOL_WIDTH, boxUpperCorner));
        p.drawLine(QLineF(upRightPointX - KOL_OFFSET , boxUpperCorner + boxPartHeight, upRightPointX - KOL_OFFSET, boxUpperCorner));
        p.drawLine(QLineF(upRightPointX - KOL_OFFSET - 0.5 * KOL_WIDTH, boxUpperCorner + boxPartHeight, upRightPointX - KOL_OFFSET - 0.5 * KOL_WIDTH, boxUpperCorner));
        upRightPointX += boxPartOffset;
        p.drawLine(QLineF(upRightPointX + KOL_OFFSET + KOL_WIDTH, boxUpperCorner + boxPartHeight, upRightPointX + KOL_OFFSET + KOL_WIDTH, boxUpperCorner));
        p.drawLine(QLineF(upRightPointX + KOL_OFFSET , boxUpperCorner + boxPartHeight, upRightPointX + KOL_OFFSET, boxUpperCorner));
        p.drawLine(QLineF(upRightPointX + KOL_OFFSET + 0.5 * KOL_WIDTH, boxUpperCorner + boxPartHeight, upRightPointX + KOL_OFFSET + 0.5 * KOL_WIDTH, boxUpperCorner));

        // Linie wymiarowe dla otworów impulsowych
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(1);
        p.setPen(pen);
        p.drawLine(QLineF(upRightPointX + KOL_OFFSET + 0.5 * KOL_WIDTH, boxUpperCorner - DIMLINE_OBJECT_OFFSET, upRightPointX + KOL_OFFSET + 0.5 * KOL_WIDTH, dimlineDeviceDistanceHeight + PAINT_OFFSET));
        p.drawLine(QLineF(upRightPointX + boxPartWidth, 2 * dimlineDeviceDistanceHeight, upRightPointX - (boxPartOffset - discWidth) * 0.5, 2 * dimlineDeviceDistanceHeight));
        arrowPolygone[0] = QPoint(upRightPointX + KOL_OFFSET + 0.5 * KOL_WIDTH,2 *dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX + KOL_OFFSET + 0.5 * KOL_WIDTH + ARROW_HEIGHT, 2 *dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + KOL_OFFSET + 0.5 * KOL_WIDTH + ARROW_HEIGHT, 2 *dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX - (boxPartOffset - discWidth) * 0.5, 2 *dimlineDeviceDistanceHeight );
        arrowPolygone[1] = QPoint(upRightPointX - (boxPartOffset - discWidth) * 0.5 + ARROW_HEIGHT,  2* dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX - (boxPartOffset - discWidth) * 0.5 + ARROW_HEIGHT, 2 *dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        p.drawText(upRightPointX + KOL_OFFSET + 0.5 * KOL_WIDTH + ARROW_HEIGHT , 2 * dimlineDeviceDistanceHeight - DIMLINE_TEXT_OFFSET, QString("25.4 mm"));

        upRightPointX -= boxPartOffset;

        p.drawLine(QLineF(upRightPointX - KOL_OFFSET - 0.5 * KOL_WIDTH, boxUpperCorner - DIMLINE_OBJECT_OFFSET, upRightPointX - KOL_OFFSET - 0.5 * KOL_WIDTH, dimlineDeviceDistanceHeight + PAINT_OFFSET));
        p.drawLine(QLineF(upRightPointX - boxPartWidth, 2 * dimlineDeviceDistanceHeight, upRightPointX + (boxPartOffset - discWidth) * 0.5, 2 * dimlineDeviceDistanceHeight));
        arrowPolygone[0] = QPoint(upRightPointX - KOL_OFFSET - 0.5 * KOL_WIDTH,2 *dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX - KOL_OFFSET - 0.5 * KOL_WIDTH - ARROW_HEIGHT, 2 *dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX - KOL_OFFSET - 0.5 * KOL_WIDTH - ARROW_HEIGHT, 2 *dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5, 2 *dimlineDeviceDistanceHeight );
        arrowPolygone[1] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 - ARROW_HEIGHT,  2* dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 - ARROW_HEIGHT, 2 *dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        p.drawText( upRightPointX - KOL_OFFSET - 0.5 * KOL_WIDTH - ARROW_HEIGHT - this->fontMetrics().horizontalAdvance(QString("25.4 mm")), 2 * dimlineDeviceDistanceHeight - DIMLINE_TEXT_OFFSET, QString("25.4 mm"));
    }
        break;
    case 0:
    {
        // Wymiarowanie odleglosci (linie wymiarowe więc cieńsze)
        p.drawLine(QLineF(PAINT_OFFSET, boxUpperCorner + boxPartHeight - DIMLINE_OBJECT_OFFSET, PAINT_OFFSET, PAINT_OFFSET));
        p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5, boxUpperCorner + boxPartHeight - DIMLINE_OBJECT_OFFSET, upRightPointX + (boxPartOffset - discWidth) * 0.5, PAINT_OFFSET));
        p.drawLine(QLineF(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength, boxUpperCorner + boxPartHeight - DIMLINE_OBJECT_OFFSET, upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength, PAINT_OFFSET));
        p.drawLine(QLineF(PAINT_OFFSET, dimlineDeviceDistanceHeight, upRightPointX + (boxPartOffset - discWidth) * 0.5, dimlineDeviceDistanceHeight));
        p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5, dimlineDeviceDistanceHeight, upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength, dimlineDeviceDistanceHeight));
        // Teskty wymiarowe
        p.drawText(PAINT_OFFSET +  (upRightPointX + (boxPartOffset - discWidth) * 0.5 - PAINT_OFFSET - this->fontMetrics().horizontalAdvance(QString::number(*parent->getParent()->getProjectInfo()->getUpstreamDeviceDistance()->getData(),'f', 2) + " m")) * 0.5, dimlineDeviceDistanceHeight - DIMLINE_TEXT_OFFSET , QString::number(*parent->getParent()->getProjectInfo()->getUpstreamDeviceDistance()->getData(),'f', 2) + " m" );
        p.drawText(upRightPointX + (boxPartOffset - discWidth) * 0.5 + ((width() - PAINT_OFFSET) - (upRightPointX + (boxPartOffset - discWidth) * 0.5) - this->fontMetrics().horizontalAdvance(QString::number(*parent->getParent()->getProjectInfo()->getDownstreamDistance()->getData(), 'f', 2) + " m")) * 0.5, dimlineDeviceDistanceHeight - DIMLINE_TEXT_OFFSET, QString::number(*parent->getParent()->getProjectInfo()->getDownstreamDistance()->getData(), 'f', 2) + " m");

        // Strzałki
        QPoint arrowPolygone[3] = { QPoint(PAINT_OFFSET, dimlineDeviceDistanceHeight), QPoint(PAINT_OFFSET + ARROW_HEIGHT, dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH), QPoint(PAINT_OFFSET + ARROW_HEIGHT, dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH)};
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5, dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 - ARROW_HEIGHT, dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 - ARROW_HEIGHT, dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5, dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + ARROW_HEIGHT, dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + ARROW_HEIGHT, dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength, dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength - ARROW_HEIGHT, dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength - ARROW_HEIGHT, dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);

        // Strzałka kierunku
        p.drawLine(QLineF(PAINT_OFFSET + 0.1 * upstreamPipeLength, boxUpperCorner + boxPartHeight - 15, PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength, boxUpperCorner + boxPartHeight - 15));
        arrowPolygone[0] = QPoint(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength, boxUpperCorner + boxPartHeight - 15);
        arrowPolygone[1] = QPoint(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength - ARROW_HEIGHT, boxUpperCorner + boxPartHeight - 15 - 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(PAINT_OFFSET + upstreamPipeLength - 0.2 * upstreamPipeLength - ARROW_HEIGHT, boxUpperCorner + boxPartHeight - 15 + 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);

    }
        break;
    case 2:
    {
        // Wymiarowanie odleglosci (linie wymiarowe więc cieńsze)
        p.drawLine(QLineF(PAINT_OFFSET, boxUpperCorner + boxPartHeight - DIMLINE_OBJECT_OFFSET, PAINT_OFFSET, PAINT_OFFSET));
        p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5, boxUpperCorner + boxPartHeight - DIMLINE_OBJECT_OFFSET, upRightPointX + (boxPartOffset - discWidth) * 0.5, PAINT_OFFSET));
        p.drawLine(QLineF(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength, boxUpperCorner + boxPartHeight - DIMLINE_OBJECT_OFFSET, upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength, PAINT_OFFSET));
        p.drawLine(QLineF(PAINT_OFFSET, dimlineDeviceDistanceHeight, upRightPointX + (boxPartOffset - discWidth) * 0.5, dimlineDeviceDistanceHeight));
        p.drawLine(QLineF(upRightPointX + (boxPartOffset - discWidth) * 0.5, dimlineDeviceDistanceHeight, upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength, dimlineDeviceDistanceHeight));
        // Teskty wymiarowe
        p.drawText(PAINT_OFFSET +(upRightPointX + (boxPartOffset - discWidth) * 0.5 - PAINT_OFFSET - this->fontMetrics().horizontalAdvance(QString::number(*parent->getParent()->getProjectInfo()->getUpstreamDeviceDistance()->getData(),'f', 2) + " m")) * 0.5, dimlineDeviceDistanceHeight - DIMLINE_TEXT_OFFSET , QString::number(*parent->getParent()->getProjectInfo()->getUpstreamDeviceDistance()->getData(),'f', 2) + " m" );
        p.drawText(upRightPointX + (boxPartOffset - discWidth) * 0.5 + (upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength - upRightPointX - (boxPartOffset - discWidth) * 0.5 - this->fontMetrics().horizontalAdvance(QString::number(*parent->getParent()->getProjectInfo()->getDownstreamDistance()->getData()))) * 0.5, dimlineDeviceDistanceHeight - DIMLINE_TEXT_OFFSET , QString::number(*parent->getParent()->getProjectInfo()->getDownstreamDistance()->getData(), 'f', 2) + " m");

        // Strzałki
        QPoint arrowPolygone[3] = { QPoint(PAINT_OFFSET, dimlineDeviceDistanceHeight), QPoint(PAINT_OFFSET + ARROW_HEIGHT, dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH), QPoint(PAINT_OFFSET + ARROW_HEIGHT, dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH)};
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5, dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 - ARROW_HEIGHT, dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 - ARROW_HEIGHT, dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5, dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + ARROW_HEIGHT, dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + (boxPartOffset - discWidth) * 0.5 + ARROW_HEIGHT, dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength, dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength - ARROW_HEIGHT, dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + boxPartOffset + boxPartWidth + downStreamPipeLength - ARROW_HEIGHT, dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);

        // Otwory impulsowe
        pen.setStyle(Qt::SolidLine);
        p.setPen(pen);
        upRightPointX += (boxPartOffset - discWidth)*0.5;
        p.drawLine(QLineF(upRightPointX - pipeDiameter - 0.5 * KOL_WIDTH, boxUpperCorner + boxPartHeight, upRightPointX - pipeDiameter - 0.5 * KOL_WIDTH, boxUpperCorner + 0.666 * boxPartHeight));
        p.drawLine(QLineF(upRightPointX - pipeDiameter + 0.5 * KOL_WIDTH, boxUpperCorner + boxPartHeight, upRightPointX - pipeDiameter + 0.5 * KOL_WIDTH, boxUpperCorner + 0.666 * boxPartHeight));
        pen.setStyle(Qt::DotLine);
        p.setPen(pen);
        p.drawLine(QLineF(upRightPointX - pipeDiameter , boxUpperCorner + boxPartHeight, upRightPointX - pipeDiameter , boxUpperCorner + 0.666 * boxPartHeight));
        pen.setStyle(Qt::SolidLine);
        p.setPen(pen);
        p.drawLine(QLineF(upRightPointX + 0.5 * pipeDiameter - 0.5 * KOL_WIDTH, boxUpperCorner + boxPartHeight, upRightPointX + 0.5 * pipeDiameter - 0.5 * KOL_WIDTH, boxUpperCorner + 0.666* boxPartHeight));
        p.drawLine(QLineF(upRightPointX + 0.5 * pipeDiameter + 0.5 * KOL_WIDTH, boxUpperCorner + boxPartHeight, upRightPointX + 0.5 * pipeDiameter + 0.5 * KOL_WIDTH, boxUpperCorner + 0.666* boxPartHeight));
        pen.setStyle(Qt::DotLine);
        p.setPen(pen);
        p.drawLine(QLineF(upRightPointX + 0.5 * pipeDiameter, boxUpperCorner + boxPartHeight, upRightPointX + 0.5 * pipeDiameter, boxUpperCorner + 0.666* boxPartHeight));

        // Linie wymiarowe dla otworów impulsowych
        pen.setStyle(Qt::SolidLine);
        p.setPen(pen);
        p.drawLine(QLineF(upRightPointX - pipeDiameter ,  boxUpperCorner + 0.666 * boxPartHeight -  DIMLINE_OBJECT_OFFSET, upRightPointX - pipeDiameter, dimlineDeviceDistanceHeight + PAINT_OFFSET));
        p.drawLine(QLineF(upRightPointX - pipeDiameter, 2 * dimlineDeviceDistanceHeight, upRightPointX, 2 * dimlineDeviceDistanceHeight));
        arrowPolygone[0] = QPoint(upRightPointX - pipeDiameter, 2 *dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX - pipeDiameter + ARROW_HEIGHT, 2 *dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX - pipeDiameter + ARROW_HEIGHT, 2 *dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX , 2 *dimlineDeviceDistanceHeight );
        arrowPolygone[1] = QPoint(upRightPointX - ARROW_HEIGHT, 2 * dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX - ARROW_HEIGHT, 2 *dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        p.drawText(upRightPointX - pipeDiameter + (upRightPointX - (upRightPointX - pipeDiameter) - this->fontMetrics().horizontalAdvance(QString::number(*parent->getParent()->getProjectInfo()->getPipeDiameter()->getData(),'f', 2) + " mm")) * 0.5, 2 * dimlineDeviceDistanceHeight - DIMLINE_TEXT_OFFSET, QString::number(*parent->getParent()->getProjectInfo()->getPipeDiameter()->getData(),'f', 2) + " mm");

        p.drawLine(QLineF(upRightPointX + 0.5 * pipeDiameter, boxUpperCorner + 0.666 * boxPartHeight - DIMLINE_OBJECT_OFFSET, upRightPointX + 0.5 * pipeDiameter, dimlineDeviceDistanceHeight + PAINT_OFFSET));
        p.drawLine(QLineF(upRightPointX, 2 * dimlineDeviceDistanceHeight, upRightPointX + 0.5 *pipeDiameter, 2 * dimlineDeviceDistanceHeight));
        arrowPolygone[0] = QPoint(upRightPointX,2 *dimlineDeviceDistanceHeight);
        arrowPolygone[1] = QPoint(upRightPointX + ARROW_HEIGHT, 2 *dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + ARROW_HEIGHT, 2 *dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        arrowPolygone[0] = QPoint(upRightPointX + 0.5 * pipeDiameter, 2 *dimlineDeviceDistanceHeight );
        arrowPolygone[1] = QPoint(upRightPointX + 0.5 * pipeDiameter - ARROW_HEIGHT,  2* dimlineDeviceDistanceHeight - 0.5 * ARROW_WIDTH);
        arrowPolygone[2] = QPoint(upRightPointX + 0.5 * pipeDiameter - ARROW_HEIGHT, 2 *dimlineDeviceDistanceHeight + 0.5 * ARROW_WIDTH);
        p.drawPolygon(arrowPolygone, 3);
        p.drawText(upRightPointX + (0.5 * pipeDiameter - this->fontMetrics().horizontalAdvance(QString::number(*parent->getParent()->getProjectInfo()->getPipeDiameter()->getData() * 0.5, 'f', 2) + " mm")) * 0.5, 2 * dimlineDeviceDistanceHeight - DIMLINE_TEXT_OFFSET, QString(QString::number(*parent->getParent()->getProjectInfo()->getPipeDiameter()->getData() * 0.5, 'f', 2) + " mm"));
    }        
    break;
    }
}

void MyPallete::paintEvent(QPaintEvent *ev){
    Q_UNUSED(ev)
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    //p.setCompositionMode(QPainter::CompositionMode_Screen);
    paint(p);
}
