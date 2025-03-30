#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QResizeEvent>
#include "parachuteview.h"

/**
 * @brief Implémentation Qt de la vue du parachute
 */
class QtParachuteView : public QGraphicsView, public ParachuteView {
    Q_OBJECT

private:
    QGraphicsScene* scene;

protected:
    /**
     * @brief Gère le redimensionnement de la vue
     * @param event Événement de redimensionnement
     */
    void resizeEvent(QResizeEvent* event) override;

public:
    /**
     * @brief Constructeur
     * @param parent Widget parent
     */
    explicit QtParachuteView(QWidget* parent = nullptr);

    /**
     * @brief Destructeur
     */
    ~QtParachuteView() override;

    /**
     * @brief Efface la vue
     */
    void clear() override;

    /**
     * @brief Dessine un trapèze
     * @param vertices Sommets du trapèze
     * @param color Couleur de remplissage
     * @param emptyCenter Indique si le centre doit être vide
     */
    void drawTrapezoid(const std::vector<Point>& vertices, const QColor& color, bool emptyCenter = false) override;

    /**
     * @brief Dessine le contour du parachute
     * @param radius Rayon du parachute
     */
    void drawParachuteOutline(double radius) override;

    /**
     * @brief Dessine les lignes de séparation entre les secteurs
     * @param radius Rayon du parachute
     * @param numSectors Nombre de secteurs
     */
    void drawSectorLines(double radius, int numSectors) override;

    /**
     * @brief Dessine les cercles concentriques
     * @param radius Rayon du parachute
     * @param numTracks Nombre de pistes
     */
    void drawTrackCircles(double radius, int numTracks) override;
};
