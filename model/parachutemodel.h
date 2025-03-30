
#pragma once
#include <QColor>
#include "../utils/Observer.h"

/**
 * @brief Modèle représentant la configuration du parachute
 */
class ParachuteConfigModel : public Observable {
private:
    double radius;           // Rayon du parachute
    int numTracks;           // Nombre de pistes concentriques
    int numSectors;          // Nombre de secteurs
    bool emptyCenter;        // Disque central vide
    QColor colorOne;         // Couleur pour les bits à 1
    QColor colorZero;        // Couleur pour les bits à 0
    bool randomColors;       // Utiliser des couleurs aléatoires
    bool sawtoothEdges;      // Utiliser des bords en dents de scie

public:
    /**
     * @brief Constructeur
     * @param radius Rayon du parachute
     * @param numTracks Nombre de pistes
     * @param numSectors Nombre de secteurs
     * @param emptyCenter Disque central vide
     */
    ParachuteConfigModel(
        double radius = 200.0,
        int numTracks = 5,
        int numSectors = 21,
        bool emptyCenter = false
        );

    /**
     * @brief Récupère le rayon du parachute
     * @return Rayon
     */
    double getRadius() const;

    /**
     * @brief Récupère le nombre de pistes
     * @return Nombre de pistes
     */
    int getNumTracks() const;

    /**
     * @brief Récupère le nombre de secteurs
     * @return Nombre de secteurs
     */
    int getNumSectors() const;

    /**
     * @brief Vérifie si le centre est vide
     * @return true si le centre est vide
     */
    bool hasEmptyCenter() const;

    /**
     * @brief Récupère la couleur pour les bits à 1
     * @return Couleur
     */
    QColor getColorOne() const;

    /**
     * @brief Récupère la couleur pour les bits à 0
     * @return Couleur
     */
    QColor getColorZero() const;

    /**
     * @brief Vérifie si les couleurs aléatoires sont activées
     * @return true si les couleurs aléatoires sont activées
     */
    bool usesRandomColors() const;

    /**
     * @brief Vérifie si les bords en dents de scie sont activés
     * @return true si les bords en dents de scie sont activés
     */
    bool usesSawtoothEdges() const;

    /**
     * @brief Définit le rayon du parachute
     * @param newRadius Nouveau rayon
     */
    void setRadius(double newRadius);

    /**
     * @brief Définit le nombre de pistes
     * @param newNumTracks Nouveau nombre de pistes
     */
    void setNumTracks(int newNumTracks);

    /**
     * @brief Définit le nombre de secteurs
     * @param newNumSectors Nouveau nombre de secteurs
     */
    void setNumSectors(int newNumSectors);

    /**
     * @brief Active/désactive le centre vide
     * @param isEmpty Centre vide
     */
    void setEmptyCenter(bool isEmpty);

    /**
     * @brief Définit la couleur pour les bits à 1
     * @param color Nouvelle couleur
     */
    void setColorOne(const QColor& color);

    /**
     * @brief Définit la couleur pour les bits à 0
     * @param color Nouvelle couleur
     */
    void setColorZero(const QColor& color);

    /**
     * @brief Active/désactive les couleurs aléatoires
     * @param useRandom Utiliser des couleurs aléatoires
     */
    void setRandomColors(bool useRandom);

    /**
     * @brief Active/désactive les bords en dents de scie
     * @param useSawtooth Utiliser des bords en dents de scie
     */
    void setSawtoothEdges(bool useSawtooth);
};
