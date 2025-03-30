#include "qtparachuteview.h"
#include <QPolygonF>
#include <QGraphicsPolygonItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QBrush>
#include <QPen>
#include <cmath>

QtParachuteView::QtParachuteView(QWidget* parent) : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setDragMode(QGraphicsView::NoDrag);

    // Configurer les marges et l'aspect ratio
    setMinimumSize(300, 300);
}

QtParachuteView::~QtParachuteView() {
    delete scene;
}

void QtParachuteView::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);

    // Maintenir un aspect ratio carré
    QRectF sceneRect(-200, -200, 400, 400);
    scene->setSceneRect(sceneRect);

    // Ajuster la vue pour voir toute la scène
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void QtParachuteView::clear() {
    scene->clear();
    
    // Réinitialiser la variable statique pour le centre vide
    static bool& centerDrawn = []() -> bool& { static bool b = false; return b; }();
    centerDrawn = false;
}

void QtParachuteView::drawTrapezoid(const std::vector<Point>& vertices, const QColor& color, bool emptyCenter) {
    QPolygonF polygon;
    for (const auto& point : vertices) {
        polygon << QPointF(point.x, point.y);
    }

    QPen pen(Qt::black, 0.5);
    QBrush brush(color);

    QGraphicsPolygonItem* polygonItem = scene->addPolygon(polygon, pen, brush);
    polygonItem->setZValue(1.0); // S'assurer que les trapèzes sont au-dessus des lignes de grille
    
    // Si c'est le premier trapèze et que le centre est vide, dessiner un cercle blanc au centre
    if (emptyCenter) {
        static bool centerDrawn = false;
        if (!centerDrawn) {
            // Dessiner un cercle blanc au centre avec environ 20% du rayon du parachute
            double emptyRadius = 200 * 0.2; // 200 est la valeur utilisée dans resizeEvent pour le rayon de la scène
            QPen centerPen(Qt::lightGray, 0.5);
            QBrush centerBrush(Qt::white);
            scene->addEllipse(-emptyRadius, -emptyRadius, emptyRadius * 2, emptyRadius * 2, centerPen, centerBrush)->setZValue(0.5);
            centerDrawn = true;
        }
    }
}

void QtParachuteView::drawParachuteOutline(double radius) {
    QPen pen(Qt::black, 1.5);
    scene->addEllipse(-radius, -radius, radius * 2, radius * 2, pen);
}

void QtParachuteView::drawSectorLines(double radius, int numSectors) {
    QPen pen(Qt::gray, 0.5, Qt::DashLine);

    const double angleStep = 2.0 * M_PI / numSectors;
    for (int i = 0; i < numSectors; ++i) {
        double angle = i * angleStep;
        double x = radius * cos(angle);
        double y = radius * sin(angle);

        scene->addLine(0, 0, x, y, pen);
    }
}

void QtParachuteView::drawTrackCircles(double radius, int numTracks) {
    QPen pen(Qt::gray, 0.5, Qt::DashLine);

    const double trackWidth = radius / numTracks;
    for (int i = 1; i < numTracks; ++i) {
        double r = i * trackWidth;
        scene->addEllipse(-r, -r, r * 2, r * 2, pen);
    }
}
