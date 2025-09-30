#include "convexhullsimulator.h"
#include "convexhullview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

ConvexHullSimulator::ConvexHullSimulator(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Convex Hull Simulator");
    setMinimumSize(700, 500);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QLabel *instructionLabel = new QLabel(
        "<h3>Convex Hull Simulator</h3>"
        "<p>1. Left-click to add points.</p>"
        "<p>2. Choose <b>Slow Hull</b> or <b>Fast Hull</b>.</p>", centralWidget);
    instructionLabel->setTextFormat(Qt::RichText);
    mainLayout->addWidget(instructionLabel);

    canvas = new ConvexHullView(centralWidget);
    mainLayout->addWidget(canvas);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    fastButton = new QPushButton("Fast Hull (Graham Scan)", centralWidget);
    slowButton = new QPushButton("Slow Hull (Brute Force)", centralWidget);
    clearButton = new QPushButton("Clear", centralWidget);

    buttonLayout->addWidget(fastButton);
    buttonLayout->addWidget(slowButton);
    buttonLayout->addWidget(clearButton);
    mainLayout->addLayout(buttonLayout);

    connect(fastButton, &QPushButton::clicked, canvas, &ConvexHullView::runFastHull);
    connect(slowButton, &QPushButton::clicked, canvas, &ConvexHullView::runSlowHull);
    connect(clearButton, &QPushButton::clicked, canvas, &ConvexHullView::clearPoints);
}
