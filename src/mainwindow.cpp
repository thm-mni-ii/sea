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

MainWindow::
MainWindow(QWidget* parent) :
        QMainWindow(parent)
{
    ui_.setupUi(this);

    ui_.graphView->layout();
    ui_.graphView->drawGraph();
}