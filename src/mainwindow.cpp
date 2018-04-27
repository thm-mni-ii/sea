//
// Created by jmeintrup on 24.04.18.
//

#include <QDebug>
#include <QPen>
#include <QGraphicsRectItem>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

#include "mainwindow.h"
#include <QTimer>

using namespace ogdf;

QRectF transformNode(const GraphAttributes& GA, node n)
{
    double x = GA.x(n);
    double y = GA.y(n);
    double w = GA.width(n);
    double h = GA.height(n);

    return QRectF(x-w/2, y-h/2, w, h);
}

MainWindow::
MainWindow(QWidget* parent) :
        QMainWindow(parent)
{
    ui_.setupUi(this);

    ui_.graphView->layout();
    ui_.graphView->drawGraph();
}