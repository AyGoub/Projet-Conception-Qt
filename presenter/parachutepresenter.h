
#pragma once
#include "../model/messagemodel.h"
#include "../model/parachutemodel.h"
#include "../view/parachuteview.h"
#include "../utils/Observer.h"

/**
 * @brief Présentateur pour la vue du parachute
 */
class ParachutePresenter : public IObserver {
private:
    MessageModel* messageModel;
    ParachuteConfigModel* configModel;
    ParachuteView* view;

public:
    /**
     * @brief Constructeur
     * @param messageModel Modèle du message
     * @param configModel Modèle de configuration
     * @param view Vue du parachute
     */
    ParachutePresenter(
        MessageModel* messageModel,
        ParachuteConfigModel* configModel,
        ParachuteView* view
        );

    /**
     * @brief Destructeur
     */
    ~ParachutePresenter() override;

    /**
     * @brief Met à jour la vue (implémentation de IObserver)
     */
    void update() override;

    /**
     * @brief Met à jour l'affichage du parachute
     */
    void updateDisplay();

    /**
     * @brief Sauvegarde la configuration et le message dans un fichier
     * @param filePath Chemin du fichier
     * @return true si la sauvegarde a réussi
     */
    bool saveToFile(const std::string& filePath) const;

    /**
     * @brief Charge la configuration et le message depuis un fichier
     * @param filePath Chemin du fichier
     * @return true si le chargement a réussi
     */
    bool loadFromFile(const std::string& filePath);
};
