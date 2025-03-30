#pragma once
#include <vector>
#include <string>
#include <QColor>
#include "../utils/Point.h"

/**
 * @brief Interface abstraite pour la vue du parachute
 */
class ParachuteView {
public:
    virtual ~ParachuteView() = default;

    /**
     * @brief Efface la vue
     */
    virtual void clear() = 0;

    /**
     * @brief Dessine un trapèze
     * @param vertices Sommets du trapèze
     * @param color Couleur de remplissage
     * @param emptyCenter Indique si le centre doit être vide
     */
    virtual void drawTrapezoid(const std::vector<Point>& vertices, const QColor& color, bool emptyCenter = false) = 0;

    /**
     * @brief Dessine le contour du parachute
     * @param radius Rayon du parachute
     */
    virtual void drawParachuteOutline(double radius) = 0;

    /**
     * @brief Dessine les lignes de séparation entre les secteurs
     * @param radius Rayon du parachute
     * @param numSectors Nombre de secteurs
     */
    virtual void drawSectorLines(double radius, int numSectors) = 0;

    /**
     * @brief Dessine les cercles concentriques
     * @param radius Rayon du parachute
     * @param numTracks Nombre de pistes
     */
    virtual void drawTrackCircles(double radius, int numTracks) = 0;
};
