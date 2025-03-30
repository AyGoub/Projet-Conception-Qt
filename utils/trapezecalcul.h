
#pragma once
#include <vector>
#include <cmath>
#include <QColor>
#include <QRandomGenerator>
#include "Point.h"
#include "../model/parachutemodel.h"

/**
 * @brief Classe utilitaire pour calculer les coordonnées des trapèzes
 */
class TrapezoidCalculator {
public:
    /**
     * @brief Calcule les coordonnées des 4 sommets d'un trapèze
     * @param k Indice du trapèze
     * @param radius Rayon du parachute
     * @param numTracks Nombre de pistes
     * @param numSectors Nombre de secteurs
     * @param sawtoothEdges Utiliser des bords en dents de scie
     * @param emptyCenter Centre vide activé
     * @return Vecteur de 4 points représentant les sommets du trapèze
     */
    static std::vector<Point> calculateTrapezoidVertices(
        int k,
        double radius,
        int numTracks,
        int numSectors,
        bool sawtoothEdges,
        bool emptyCenter = false
    );

    /**
     * @brief Détermine la couleur d'un trapèze
     * @param bitValue Valeur du bit (vrai pour 1, faux pour 0)
     * @param config Configuration du parachute
     * @return Couleur du trapèze
     */
    static QColor getTrapezoidColor(bool bitValue, const ParachuteConfigModel& config);

    /**
     * @brief Vérifie si un trapèze doit être affiché (selon config centre vide)
     * @param k Indice du trapèze
     * @param numSectors Nombre de secteurs
     * @param emptyCenter Centre vide activé
     * @return true si le trapèze doit être affiché
     */
    static bool shouldDisplayTrapezoid(int k, int numSectors, bool emptyCenter);
};
