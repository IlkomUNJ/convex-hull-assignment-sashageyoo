#include "convexhullview.h"
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <algorithm>

// --- Utility Functions ---
QPointF ConvexHullView::findLowestPoint(const QVector<QPointF>& points) {
    QPointF lowest = points[0];
    for (const QPointF& p : points) {
        if (p.y() < lowest.y() || (p.y() == lowest.y() && p.x() < lowest.x())) {
            lowest = p;
        }
    }
    return lowest;
}

int ConvexHullView::orientation(const QPointF& p, const QPointF& q, const QPointF& r) {
    qreal val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());
    if (qFuzzyIsNull(val)) return 0;
    return (val > 0) ? 1 : 2;
}

qreal ConvexHullView::distSq(const QPointF& p1, const QPointF& p2) {
    return (p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y());
}

// --- Constructor ---
ConvexHullView::ConvexHullView(QWidget *parent)
    : QWidget(parent), simulationRunning(false), currentStepIndex(0), iterations(0) {
    setMinimumSize(400, 300);
    setStyleSheet("background-color: #f0f4f8; border: 1px solid #c8d1d9; border-radius: 8px;");
}

void ConvexHullView::addPoint(const QPointF& p) {
    if (simulationRunning) return;
    inputPoints.append(p);
    update();
}

void ConvexHullView::clearPoints() {
    inputPoints.clear();
    sortedPoints.clear();
    hullPoints.clear();
    simulationRunning = false;
    currentStepIndex = 0;
    iterations = 0;
    p0 = p_prev = p_top = p_next = QPointF();
    update();
}

bool ConvexHullView::isSimulationRunning() const { return simulationRunning; }

// --- Algorithms ---
void ConvexHullView::runFastHull() {
    if (inputPoints.size() < 3) return;
    simulationRunning = false;
    iterations = 0;

    p0 = findLowestPoint(inputPoints);
    sortedPoints = inputPoints;
    sortedPoints.removeAll(p0);
    sortedPoints.prepend(p0);

    std::sort(sortedPoints.begin() + 1, sortedPoints.end(), [&](const QPointF& a, const QPointF& b) {
        int o = orientation(p0, a, b);
        if (o == 0) return distSq(p0, a) < distSq(p0, b);
        return o == 2;
    });

    QVector<QPointF> uniqueSortedPoints;
    uniqueSortedPoints.append(sortedPoints[0]);
    for (int i = 1; i < sortedPoints.size(); ++i) {
        while (i + 1 < sortedPoints.size() && orientation(sortedPoints[0], sortedPoints[i], sortedPoints[i+1]) == 0) {
            i++;
        }
        uniqueSortedPoints.append(sortedPoints[i]);
    }
    sortedPoints = uniqueSortedPoints;

    if (sortedPoints.size() < 3) return;

    hullPoints.clear();
    hullPoints.append(sortedPoints[0]);
    hullPoints.append(sortedPoints[1]);
    hullPoints.append(sortedPoints[2]);

    for (int i = 3; i < sortedPoints.size(); ++i) {
        while (hullPoints.size() >= 2 && orientation(hullPoints[hullPoints.size()-2], hullPoints.last(), sortedPoints[i]) != 2) {
            hullPoints.pop_back();
            iterations++;
        }
        hullPoints.push_back(sortedPoints[i]);
        iterations++;
    }
    update();
}

void ConvexHullView::runSlowHull() {
    if (inputPoints.size() < 3) return;
    simulationRunning = false;
    iterations = 0;

    QVector<QPointF> hull;
    int n = inputPoints.size();

    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            int sidePos = 0, sideNeg = 0;
            for (int k = 0; k < n; ++k) {
                if (k == i || k == j) continue;
                int o = orientation(inputPoints[i], inputPoints[j], inputPoints[k]);
                if (o == 1) sidePos++;
                else if (o == 2) sideNeg++;
                iterations++;
            }
            if (sidePos == 0 || sideNeg == 0) {
                if (!hull.contains(inputPoints[i])) hull.append(inputPoints[i]);
                if (!hull.contains(inputPoints[j])) hull.append(inputPoints[j]);
            }
        }
    }

    // sort hull points CCW for drawing
    p0 = findLowestPoint(hull);
    std::sort(hull.begin(), hull.end(), [&](const QPointF& a, const QPointF& b){
        int o = orientation(p0, a, b);
        if (o == 0) return distSq(p0,a) < distSq(p0,b);
        return o == 2;
    });
    hullPoints = hull;
    update();
}

// --- Events ---
void ConvexHullView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && !simulationRunning) {
        addPoint(event->pos());
    }
}

void ConvexHullView::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw points
    painter.setBrush(QBrush(QColor(66, 135, 245)));
    painter.setPen(QPen(QColor(66, 135, 245), 5));
    for (const QPointF& p : inputPoints) {
        painter.drawEllipse(p, 5, 5);
    }

    // Draw hull
    if (!hullPoints.isEmpty()) {
        painter.setBrush(QColor(0, 100, 255, 60));
        painter.setPen(QPen(QColor(0, 100, 255), 3));
        QPolygonF finalHull;
        for (const QPointF& p : hullPoints) finalHull << p;
        painter.drawPolygon(finalHull);
    }

    // Info text
    painter.setPen(Qt::yellow);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    QString info = QString("Total points: %1\n Hull points: %2\n Iterations: %3")
                       .arg(inputPoints.size())
                       .arg(hullPoints.size())
                       .arg(iterations);
    painter.drawText(10, 20, info);
}
