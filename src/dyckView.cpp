//
// Created by jmeintrup on 02.05.18.
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
#include <bitset>

#include "dyckView.h"

using namespace std;

DyckView::DyckView(QWidget *parent) : QGraphicsView(parent) {
    scene_ = new QGraphicsScene(this);
    scene_->setBackgroundBrush(QBrush(QColor("white")));

    setScene(scene_);

    setRenderHint(QPainter::Antialiasing);
}
/*
void DyckView::drawDyckWord(boost::dynamic_bitset<> word, unsigned int segmentLength, unsigned int* matches, boost::dynamic_bitset<> pioneers, boost::dynamic_bitset<> globals) {

    std::bitset<15> bits;



    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSize(8);

    scene_->clear();
    scene_->setBackgroundBrush(QBrush("white"));

    double w = 25;
    double h = 25;
    double x = 0;
    double y = 0;

    QList<QRectF> arr = QList<QRectF>();
    for(int i = 0; i < 15; i++) {
        if(i % 2 ==0) bits[i].flip();
        QRectF rect(x-w/2 + w * i, y-h/2, w, h);
        arr.push_back(rect);
        //scene_->addRect(rect, QPen(QColor("grey"), 1), QBrush(QColor("white")));
    }

    QRectF rect(x-w/2, y-h/2, w, h);


    QString open = QString("(");
    QString closed = QString(")");

    for(int i = 0; i < 15; i++) {
        QGraphicsRectItem* ri = scene_->addRect(arr[i], QPen(QColor("grey"), 1), QBrush(QColor("white")));

        QGraphicsSimpleTextItem *newLabel;
        if(bits[i] == 0) {
            newLabel = new QGraphicsSimpleTextItem(closed);
        } else {
            newLabel = new QGraphicsSimpleTextItem(open);
        }

        newLabel->setFont(font);

        QRectF textRect = newLabel->boundingRect();

        double newx = (w - textRect.width()) / 2;
        double newy = (h - textRect.height()) / 2;

        newLabel->setBrush(QColor("blue"));
        newLabel->setParentItem(ri);
        newLabel->setPos(arr[i].x() + newx, y - h / 2 + newy);
    }

    QPainterPath path(QPointF(arr[0].x() + w / 2, arr[0].y()));
    path.lineTo(QPointF(arr[0].x() + w / 2, arr[0].y() - 4));
    path.lineTo(QPointF(arr[1].x() + w / 2, arr[1].y() - 4));
    path.lineTo(QPointF(arr[1].x() + w / 2, arr[1].y()));
    scene_->addPath(path, QPen(QColor("red"), 1));

}*/
