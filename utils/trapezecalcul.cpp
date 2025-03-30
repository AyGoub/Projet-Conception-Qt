#include "trapezecalcul.h"
#include <QRandomGenerator>

std::vector<Point> TrapezoidCalculator::calculateTrapezoidVertices(
    int k,
    double radius,
    int numTracks,
    int numSectors,
    bool sawtoothEdges,
    bool emptyCenter
    ) {
    const double angleSector = 2.0 * M_PI / numSectors;
    const double trackWidth = radius / numTracks;

    // Calcul des indices de secteur et de piste
    int s = k % numSectors;
    int t = k / numSectors;

    // Calcul des angles
    double alpha1 = s * angleSector;
    double alpha2 = (s + 1) * angleSector;

    // Calcul des rayons avec décalage si le centre est vide
    double r_p, r_q;
    
    if (emptyCenter) {
        // Si le centre est vide, nous réservons environ 20% du rayon pour le centre vide
        double emptyRadius = radius * 0.2;
        double usableRadius = radius - emptyRadius;
        
        // Recalcul de la largeur des pistes avec le rayon utilisable
        double adjustedTrackWidth = usableRadius / numTracks;
        
        // Calcul des rayons avec décalage
        r_p = emptyRadius + (t * adjustedTrackWidth);
        r_q = emptyRadius + ((t + 1) * adjustedTrackWidth);
    } else {
        // Calcul normal sans décalage
        r_p = t * trackWidth;
        r_q = (t + 1) * trackWidth;
    }

    // Variante avec dents de scie
    if (sawtoothEdges && t > 0) {
        // Ajoute un petit décalage pour créer l'effet de dents de scie
        double offset = trackWidth * 0.1;

        // Alterne le décalage selon le secteur
        if (s % 2 == 0) {
            r_p -= offset;
        } else {
            r_p += offset;
        }
    }

    // Calcul des sommets
    std::vector<Point> vertices = {
        Point(r_p * cos(alpha1), r_p * sin(alpha1)),  // p1
        Point(r_p * cos(alpha2), r_p * sin(alpha2)),  // p2
        Point(r_q * cos(alpha2), r_q * sin(alpha2)),  // q2
        Point(r_q * cos(alpha1), r_q * sin(alpha1))   // q1
    };

    return vertices;
}

QColor TrapezoidCalculator::getTrapezoidColor(bool bitValue, const ParachuteConfigModel& config) {
    if (bitValue) {
        // Pour les bits à 1
        if (config.usesRandomColors()) {
            // Générer une couleur aléatoire (rouge/orange/jaune)
            int r = QRandomGenerator::global()->bounded(150, 255);
            int g = QRandomGenerator::global()->bounded(0, 150);
            int b = QRandomGenerator::global()->bounded(0, 100);
            return QColor(r, g, b);
        } else {
            return config.getColorOne();
        }
    } else {
        // Pour les bits à 0
        return config.getColorZero();
    }
}

bool TrapezoidCalculator::shouldDisplayTrapezoid(int /* k */, int /* numSectors */, bool /* emptyCenter */) {
    // Cette fonction est prévue pour une utilisation future
    // Pour l'instant, tous les trapèzes sont affichés
    return true;
}

