#pragma once
#include <string>
#include <vector>
#include "../utils/Observer.h"

/**
 * @brief Modèle représentant le message à encoder
 */
class MessageModel : public Observable {
private:
    std::string text;                // Texte du message
    char referenceChar;              // Caractère de référence (zéro de l'encodage)
    std::vector<bool> bits;          // Bits du message encodé
    bool useBlockFormat;             // Utiliser des blocs de 10 éléments (7 bits + 3 zéros)
    bool addMarkers;                 // Ajouter des marqueurs de début/fin
    int numSectors;                 // Nombre de secteurs par piste

    /**
     * @brief Convertit le texte en séquence de bits
     */
    void convertTextToBits();

public:
    /**
     * @brief Constructeur
     * @param text Texte initial
     * @param referenceChar Caractère de référence (par défaut '@')
     * @param useBlockFormat Utiliser des blocs de 10 éléments
     * @param addMarkers Ajouter des marqueurs de début/fin
     * @param numSectors Nombre de secteurs par piste
     */
    MessageModel(
        const std::string& text = "",
        char referenceChar = '@',
        bool useBlockFormat = false,
        bool addMarkers = false,
        int numSectors = 21
        );

    /**
     * @brief Définit un nouveau texte
     * @param newText Nouveau texte
     */
    void setText(const std::string& newText);

    /**
     * @brief Récupère la valeur d'un bit
     * @param index Index du bit
     * @return Valeur du bit (true pour 1, false pour 0)
     */
    bool getBit(size_t index) const;

    /**
     * @brief Récupère le nombre total de bits
     * @return Nombre de bits
     */
    size_t getBitsCount() const;

    /**
     * @brief Récupère le texte du message
     * @return Texte du message
     */
    std::string getText() const;

    /**
     * @brief Récupère le caractère de référence
     * @return Caractère de référence
     */
    char getReferenceChar() const;

    /**
     * @brief Définit un nouveau caractère de référence
     * @param newRef Nouveau caractère de référence
     */
    void setReferenceChar(char newRef);

    /**
     * @brief Active/désactive le format par blocs
     * @param useBlocks Utiliser des blocs
     */
    void setUseBlockFormat(bool useBlocks);

    /**
     * @brief Active/désactive les marqueurs
     * @param useMarkers Utiliser des marqueurs
     */
    void setAddMarkers(bool useMarkers);

    /**
     * @brief Indique si le format par blocs est activé
     * @return true si le format par blocs est activé
     */
    bool getUseBlockFormat() const;

    /**
     * @brief Indique si les marqueurs sont activés
     * @return true si les marqueurs sont activés
     */
    bool getAddMarkers() const;

    /**
     * @brief Définit le nombre de secteurs par piste
     * @param newNumSectors Nouveau nombre de secteurs
     */
    void setNumSectors(int newNumSectors);

    /**
     * @brief Récupère le nombre de secteurs par piste
     * @return Nombre de secteurs
     */
    int getNumSectors() const;
};
