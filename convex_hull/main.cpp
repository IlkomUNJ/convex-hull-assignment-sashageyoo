#include <QApplication>
#include "convexhullsimulator.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ConvexHullSimulator simulator;
    simulator.show();
    return a.exec();
}
