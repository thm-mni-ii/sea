//
// Created by jmeintrup on 24.04.18.
//

#ifndef SEA_GRAPHVIEW_H
#define SEA_GRAPHVIEW_H
#include <QGraphicsView>
#include <QPolygonF>
#include <QVector>
#include <QTimer>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/fileformats/GraphIO.h>

class QSize;
class QGraphicsScene;
class QPointF;

class GraphView : public QGraphicsView
{
Q_OBJECT

public:
    bool waiting;

    GraphView(QWidget* parent=0);

    void drawArrow(const QPointF& start, const QPointF& end, const QColor& color);

    QPolygonF createArrow(const QPointF& start, const QPointF& end);


    ogdf::GraphAttributes* getGraphAttributes();
    void drawGraph();
    void layout();
    void drawGraphDelayed();

public slots:


    void save();
    void load();
    void stop();
    void start();

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void scaleXUp();
    void scaleYUp();
    void scaleXDown();
    void scaleYDown();

    void generateRandomGraph();


private:
    QTimer * timer;
    ogdf::Graph* graph_;
    ogdf::GraphAttributes* GA_;

    QVector<ogdf::node> nodeList_;
    QVector<ogdf::edge> edgeList_;

    QGraphicsScene* scene_;


};
#endif //SEA_GRAPHVIEW_H
