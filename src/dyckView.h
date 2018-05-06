//
// Created by jmeintrup on 02.05.18.
//

#ifndef SEA_DYCKVIEW_H
#define SEA_DYCKVIEW_H
#include <QGraphicsView>
#include <QPolygonF>
#include <QVector>
#include <QTimer>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/fileformats/GraphIO.h>
#include <boost/dynamic_bitset.hpp>


class QSize;
class QGraphicsScene;
class QPointF;

class DyckView : public QGraphicsView
{
    Q_OBJECT

public:
    DyckView(QWidget* parent=0);
    void drawDyckWord();

private:
    QGraphicsScene* scene_;

    void drawDyckWord(boost::dynamic_bitset<> word, unsigned int segmentLength, unsigned int *matches,
                      boost::dynamic_bitset<> pioneers, boost::dynamic_bitset<> globals);
};
#endif //SEA_DYCKVIEW_H
