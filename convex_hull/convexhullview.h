#ifndef CONVEXHULLVIEW_H
#define CONVEXHULLVIEW_H

#include <QWidget>
#include <QVector>
#include <QPointF>

// --- ConvexHullView Class ---
class ConvexHullView : public QWidget {
    Q_OBJECT

public:
    explicit ConvexHullView(QWidget *parent = nullptr);

    void addPoint(const QPointF& p);
    void clearPoints();
    bool isSimulationRunning() const;

public slots:
    void runFastHull();   // Graham Scan
    void runSlowHull();   // Brute Force O(n^3)

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPointF> inputPoints;
    QVector<QPointF> sortedPoints;
    QVector<QPointF> hullPoints;

    bool simulationRunning;
    int currentStepIndex;
    int iterations;

    QPointF p0, p_prev, p_top, p_next;

    // --- Utility Functions ---
    QPointF findLowestPoint(const QVector<QPointF>& points);
    int orientation(const QPointF& p, const QPointF& q, const QPointF& r);
    qreal distSq(const QPointF& p1, const QPointF& p2);
};

#endif // CONVEXHULLVIEW_H
