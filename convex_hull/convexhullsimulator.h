#ifndef CONVEXHULLSIMULATOR_H
#define CONVEXHULLSIMULATOR_H

#include <QMainWindow>

class ConvexHullView;
class QPushButton;

class ConvexHullSimulator : public QMainWindow {
    Q_OBJECT

public:
    ConvexHullSimulator(QWidget *parent = nullptr);

private:
    ConvexHullView *canvas;
    QPushButton *fastButton;
    QPushButton *slowButton;
    QPushButton *clearButton;
};

#endif // CONVEXHULLSIMULATOR_H
