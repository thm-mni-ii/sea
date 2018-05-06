#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <QGraphicsScene>

#include "ui_mainwindow.h"

class Ui_MainWindow;
class GraphView;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget* parent=0);

private:
    Ui_MainWindow ui_;

    GraphView* graphView_;
    DyckView* dyckView_;
};

#endif