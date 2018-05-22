//
// Created by jmeintrup on 24.04.18.
//

#include <QRectF>
#include <QGraphicsScene>
#include <QBrush>
#include <QPen>
#include <QPointF>
#include <QPainterPath>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QString>
#include <QLineF>
#include <QPolygonF>
#include <QDebug>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <QtWidgets/QFileDialog>
#include <thread>
#include <sealib/graphalgorithms.h>
#include <ogdf/planarlayout/PlanarStraightLayout.h>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>
#include <QtCore/QThread>
#include <include/sealib/graphio.h>

#include "graphView.h"

using namespace ogdf;

GraphView::GraphView(QWidget* parent) :
        QGraphicsView(parent)
{
    graph_ = new Graph();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(generateRandomGraph()));

    //ogdf::randomPlanarBiconnectedGraph(*graph_, 5, 15);
    graph_ = Sealib::GraphIO::randomEulerianOgdfGrah(10,25);

    std::cout<<"generated"<<std::endl;
    GA_ = new GraphAttributes(*graph_,
                              GraphAttributes::nodeGraphics |
                              GraphAttributes::edgeGraphics |
                              GraphAttributes::nodeLabel |
                              GraphAttributes::edgeLabel |
                              GraphAttributes::nodeStyle |
                              GraphAttributes::edgeType |
                              GraphAttributes::edgeArrow |
                              GraphAttributes::edgeStyle);

    GA_->setAllHeight(30);
    GA_->setAllWidth(30);

    for(edge e: graph_->edges) {
        GA_->arrowType(e) = EdgeArrow::None;
    }

    scene_ = new QGraphicsScene(this);
    scene_->setBackgroundBrush(QBrush(QColor("white")));

    setScene(scene_);

    setRenderHint(QPainter::Antialiasing);
}

void GraphView::layout() {
    PlanarizationLayout layout;

    layout.call(*GA_);
}

void GraphView::drawGraph()
{
    scene_->clear();
    scene_->setBackgroundBrush(QBrush("white"));
    //
    // Draw the edges
    //
    List<edge> edges;
    graph_->allEdges(edges);

    for (auto e: edges) {
        /*
        double x1 = GA_->x(source), y1 = GA_->y(source);
        double x2 = GA_->x(target), y2 = GA_->y(target);

        Color strokeColor = GA_->strokeColor(e).toString();


        QPainterPath p;
        p.moveTo(x1, y1);
        p.lineTo(x2, y2);
        (void) scene_->addPath(p, QPen(QColor(strokeColor.red(), strokeColor.green(), strokeColor.blue()), GA_->strokeWidth(e)));*/

        DPolyline& points = GA_->bends(e);

        Color strokeColor = GA_->strokeColor(e).toString();

        List<DPoint>::const_iterator iter = points.begin();
        if ( iter != points.end() ) {
            QPointF startPoint((*iter).m_x, (*iter).m_y);
            QPainterPath path(startPoint);

            for ( ; iter != points.end(); ++iter )
            {
                DPoint dp = *iter;
                QPointF nextPoint(dp.m_x, dp.m_y);
                path.lineTo(nextPoint);
            }


            scene_->addPath(path, QPen(QColor(strokeColor.red(), strokeColor.green(), strokeColor.blue()), GA_->strokeWidth(e)));

            if(GA_->arrowType(e) == EdgeArrow::Last) {
                List<DPoint>::iterator arrowStartPoint =
                        points.get(points.size() - 2);
                List<DPoint>::iterator arrowEndPoint =
                        points.get(points.size() - 1);
                drawArrow(
                        QPointF((*arrowStartPoint).m_x, (*arrowStartPoint).m_y),
                        QPointF((*arrowEndPoint).m_x, (*arrowEndPoint).m_y),
                        QColor(strokeColor.red(), strokeColor.green(), strokeColor.blue())
                );
            } else if(GA_->arrowType(e) == EdgeArrow::First) {
                List<DPoint>::iterator arrowStartPoint =
                        points.get(1);
                List<DPoint>::iterator arrowEndPoint =
                        points.get(0);
                drawArrow(
                        QPointF((*arrowStartPoint).m_x, (*arrowStartPoint).m_y),
                        QPointF((*arrowEndPoint).m_x, (*arrowEndPoint).m_y),
                        QColor(strokeColor.red(), strokeColor.green(), strokeColor.blue())
                );
            }


            QString ls(GA_->label(e).c_str());

            if(ls.size()!= 0) {
                QPointF labelPoint;
                if(points.size() == 2) { //straight line, draw edge label in middle of edge
                    List<DPoint>::iterator s =
                            points.get(1);
                    List<DPoint>::iterator ee =
                            points.get(0);


                    labelPoint = QPointF(((*s).m_x + (*ee).m_x) / 2, ((*s).m_y + (*ee).m_y) / 2);

                } else {
                    List<DPoint>::iterator s = points.get(1);

                    labelPoint = QPointF((*s).m_x, (*s).m_y);
                }

                QRect rect((int) labelPoint.x() - 10, (int) labelPoint.y() - 10, 20, 20);

                QFont font;
                font.setFamily(font.defaultFamily());
                font.setPointSize(8);

                QGraphicsEllipseItem* ri =
                        scene_->addEllipse(rect, QPen(QColor(strokeColor.red(), strokeColor.green(), strokeColor.blue()), 2), QBrush("white"));


                QGraphicsSimpleTextItem* newLabel = new QGraphicsSimpleTextItem(ls);
                newLabel->setFont(font);

                QRectF textRect = newLabel->boundingRect();
                double newx = (20 - textRect.width())/2;
                double newy = (20 - textRect.height())/2;

                newLabel->setBrush(QColor("black"));
                newLabel->setParentItem(ri);
                newLabel->setPos(labelPoint.x() - 10 + newx, labelPoint.y() -10 + newy);
            }

            //QPolygonF arrow = createArrow(
                    //QPointF((*arrowStartPoint).m_x, (*arrowStartPoint).m_y),
                    //QPointF((*arrowEndPoint).m_x, (*arrowEndPoint).m_y));


            //path.addPolygon(arrow);

         }
/*
        if(GA_->arrowType(e) == EdgeArrow::Last) {
            drawArrow(QPointF((int) x1, (int) y1), QPoint((int) x2,(int) y2), QColor("black"));
        } else if((GA_->arrowType(e) == EdgeArrow::First)){
            drawArrow(QPoint((int) x2,(int) y2), QPointF((int) x1, (int) y1),  QColor("black")));
        }*/
    }

    for (auto e: edges) {
        DPolyline& points = GA_->bends(e);

        QString ls(GA_->label(e).c_str());
        Color strokeColor = GA_->strokeColor(e).toString();


        if(ls.size()!= 0) {
            QPointF labelPoint;
            if(points.size() == 2) { //straight line, draw edge label in middle of edge
                List<DPoint>::iterator s =
                        points.get(1);
                List<DPoint>::iterator ee =
                        points.get(0);


                labelPoint = QPointF(((*s).m_x + (*ee).m_x) / 2, ((*s).m_y + (*ee).m_y) / 2);

            } else {
                List<DPoint>::iterator s = points.get(1);

                labelPoint = QPointF((*s).m_x, (*s).m_y);
            }

            QRect rect((int) labelPoint.x() - 10, (int) labelPoint.y() - 10, 20, 20);

            QFont font;
            font.setFamily(font.defaultFamily());
            font.setPointSize(8);

            QGraphicsEllipseItem* ri =
                    scene_->addEllipse(rect, QPen(QColor(strokeColor.red(), strokeColor.green(), strokeColor.blue()), 2), QBrush("white"));


            QGraphicsSimpleTextItem* newLabel = new QGraphicsSimpleTextItem(ls);
            newLabel->setFont(font);

            QRectF textRect = newLabel->boundingRect();
            double newx = (20 - textRect.width())/2;
            double newy = (20 - textRect.height())/2;

            newLabel->setBrush(QColor("black"));
            newLabel->setParentItem(ri);
            newLabel->setPos(labelPoint.x() - 10 + newx, labelPoint.y() -10 + newy);
        }

    }


    //
    // Draw the nodes
    //
    ogdf::List<node> nodes;
    graph_->allNodes(nodes);
    int i = 0;
    for (auto n: nodes)
    {

        double x = GA_->x(n);
        double y = GA_->y(n);
        double w = GA_->width(n);
        double h = GA_->height(n);
        Color strokeColor = GA_->strokeColor(n);
        Color fillColor = GA_->fillColor(n);

        QRectF rect(x-w/2, y-h/2, w, h);

        QGraphicsRectItem* ri =
                scene_->addRect(rect, QPen(QColor(strokeColor.red(), strokeColor.green(), strokeColor.blue()), 2), QBrush(QColor(fillColor.red(), fillColor.green(), fillColor.blue())));

        QString ls = QString("%1").arg(i++);

        QGraphicsSimpleTextItem* newLabel =
                new QGraphicsSimpleTextItem(ls);
        QRectF textRect = newLabel->boundingRect();
        double newx = (w - textRect.width())/2;
        double newy = (h - textRect.height())/2;

        newLabel->setBrush(QColor("blue"));
        newLabel->setParentItem(ri);
        newLabel->setPos(x-w/2+newx, y-h/2+newy);
    }
}

QPolygonF GraphView::createArrow(const QPointF& start, const QPointF& end) {
    qreal Pi = 3.14;
    qreal arrowSize = 10;

    QPolygonF arrowHead;

    QLineF line(end, start);

    double angle = ::acos(line.dx() / line.length());

    if ( line.dy() >= 0 )
        angle = (Pi * 2) - angle;

    QPointF arrowP1 = line.p1() + QPointF(sin(angle+Pi/3)*arrowSize,
                                          cos(angle+Pi/3)*arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle+Pi-Pi/3)*arrowSize,
                                          cos(angle+Pi-Pi/3)*arrowSize);


    arrowHead.clear();
    arrowHead << line.p1() << arrowP1 << arrowP2;

    return arrowHead;
}

void GraphView::drawArrow(const QPointF& start, const QPointF& end, const QColor& color) {
    qreal Pi = 3.14;
    qreal arrowSize = 10;

    QPolygonF arrowHead;

    QLineF line(end, start);

    double angle = ::acos(line.dx() / line.length());

    if ( line.dy() >= 0 )
        angle = (Pi * 2) - angle;

    QPointF arrowP1 = line.p1() + QPointF(sin(angle+Pi/3)*arrowSize,
                                          cos(angle+Pi/3)*arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle+Pi-Pi/3)*arrowSize,
                                          cos(angle+Pi-Pi/3)*arrowSize);
    arrowHead.clear();
    arrowHead << line.p1() << arrowP1 << arrowP2;

    scene_->addPolygon(arrowHead, QPen(color), QBrush(color));

    scene_->addLine(line, QPen(color));
}

ogdf::GraphAttributes *GraphView::getGraphAttributes() {
    return GA_;
}

void GraphView::save() {
    QString qFileName = QFileDialog::getSaveFileName(0, tr("Save File"), ".");
    if(qFileName != nullptr) {
        std::string fileName = qFileName.toStdString();
        std::string extension;
        std::ofstream out (fileName);
        if(fileName.find_last_of('.') != std::string::npos) {
            extension = fileName.substr(fileName.find_last_of('.') + 1);
            for(unsigned int i = 0; i < extension.size(); i++){
                extension[i] = (char) tolower(extension[i]);
            }
        }
        if(extension == "gml") {
            GraphIO::writeGML(*GA_, out);
        } else if(extension == "dot") {
            GraphIO::writeDOT(*GA_, out);
        } else if(extension == "gdf") {
            GraphIO::writeGDF(*GA_, out);
        }
        else if(extension == "leda") {
            GraphIO::writeLEDA(GA_->constGraph(), out);
        } else {
            QMessageBox::information(this,  tr("Sealib Visual Graph"), tr("Please use one of the following file extensions: gml, gdf, dot, leda."));
        }
        out.close();
    }
}

void GraphView::load() {
    QString qFileName = QFileDialog::getOpenFileName(this, tr("Load Graph"),"/path/to/file/",tr("All file (*.);; GML (*.gml);; DOT (*.dot);; GDF (*.gdf);; LEDA (*.leda)"));
    if(qFileName != nullptr) {
        string fileName = qFileName.toStdString();
        std::string extension;
        std::ifstream in (fileName);

        if(fileName.find_last_of('.') != std::string::npos) {
            extension = fileName.substr(fileName.find_last_of(".") + 1);
            for(unsigned int i = 0; i < extension.size(); i++){
                extension[i] = (char) tolower(extension[i]);
            }
        }

        if(extension == "gml") {
            GraphIO::readGML(*GA_, *graph_, in);
            layout();
            drawGraph();
        } else if(extension == "dot") {
            GraphIO::readDOT(*GA_, *graph_, in);
            layout();
            drawGraph();
        } else if(extension == "gdf") {
            GraphIO::readGDF(*GA_, *graph_, in);
            layout();
            drawGraph();
        }
        else if(extension == "leda") {
            GraphIO::readLEDA(*graph_, in);
            GA_ = new GraphAttributes(*graph_,
                                      GraphAttributes::nodeGraphics |
                                      GraphAttributes::edgeGraphics |
                                      GraphAttributes::nodeLabel |
                                      GraphAttributes::edgeLabel |
                                      GraphAttributes::nodeStyle |
                                      GraphAttributes::edgeType |
                                      GraphAttributes::edgeArrow |
                                      GraphAttributes::edgeStyle);

            GA_->setAllHeight(30);
            GA_->setAllWidth(30);
            layout();
            drawGraph();
        }
        in.close();
    }
}


void GraphView::stop() {
    timer->stop();
}

void GraphView::start() {
    Sealib::Graph *sealibGraph = Sealib::GraphIO::sealibGraphFromOgdfGraph(graph_);

    Sealib::TrailStructure** ts = Sealib::GraphAlgorithms::hierholzer(sealibGraph);

    Sealib::GraphIO::graphAttributesFromTrail(GA_, sealibGraph, ts);

    /*timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(generateRandomGraph()));
    timer->start(1000);*/
    drawGraph();
}

void GraphView::moveUp() {
    GA_->translate(0, -20);
    drawGraph();
}

void GraphView::moveDown() {
    GA_->translate(0, 20);
    drawGraph();
}

void GraphView::moveLeft() {
    GA_->translate(-20, 0);
    drawGraph();
}

void GraphView::moveRight() {
    GA_->translate(20, 0);
    drawGraph();
}

void GraphView::scaleXUp() {
    GA_->scale(1.1, 1, true);
    drawGraph();
}

void GraphView::scaleYUp() {
    GA_->scale(1, 1.1, true);
    drawGraph();
}

void GraphView::scaleXDown() {
    GA_->scale(0.9, 1, true);
    drawGraph();
}

void GraphView::scaleYDown() {
    GA_->scale(1, 0.9, true);
    drawGraph();
}

void GraphView::generateRandomGraph() {
    int nodeNumber = 0;
    int edgeNumber = 0;
    QString graphType = nullptr;
    for(auto c: parent()->children()) {
        if(c->objectName().toStdString() == "nodeSpinBox") {
            QSpinBox *nodeSpinBox = (QSpinBox*) c;
            nodeNumber = nodeSpinBox->text().toInt(0);
        } else if(c->objectName().toStdString() == "edgeSpinBox") {
            QSpinBox *edgeSpinBox = (QSpinBox*) c;
            edgeNumber = edgeSpinBox->text().toInt(0);
        } else if(c->objectName().toStdString() == "graphTypeBox") {
            QComboBox *comboBox = (QComboBox*) c;
            graphType = comboBox->currentText();
        }
    }

    if (graphType.toStdString() == "Planar Bi Connected") {
        ogdf::randomPlanarBiconnectedGraph(*graph_, nodeNumber, edgeNumber);
    } else if (graphType.toStdString() == "Planar Connected") {
        ogdf::randomPlanarConnectedGraph(*graph_, nodeNumber, edgeNumber);
    }

    for(edge e: graph_->edges) {
        GA_->arrowType(e) = EdgeArrow::None;
    }

    layout();
    drawGraph();
}
