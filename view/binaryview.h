#pragma once
#include <QWidget>
#include <QColor>
#include <vector>
#include "../model/messagemodel.h"
#include "../model/parachutemodel.h"
#include "../utils/Observer.h"

/**
 * @brief Vue binaire du message
 */
class BinaryView : public QWidget, public IObserver {
    Q_OBJECT

private:
    MessageModel* messageModel;
    ParachuteConfigModel* configModel;
    int cellSize;
    int margin;
    int columns;

    /**
     * @brief Calcule les dimensions optimales pour la grille
     */
    void calculateGridDimensions();

protected:
    /**
     * @brief Gère l'événement de peinture
     * @param event Événement de peinture
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief Gère le redimensionnement
     * @param event Événement de redimensionnement
     */
    void resizeEvent(QResizeEvent* event) override;

public:
    /**
     * @brief Constructeur
     * @param messageModel Modèle du message
     * @param configModel Modèle de configuration
     * @param parent Widget parent
     */
    explicit BinaryView(
        MessageModel* messageModel,
        ParachuteConfigModel* configModel,
        QWidget* parent = nullptr
        );

    /**
     * @brief Destructeur
     */
    ~BinaryView() override;

    /**
     * @brief Met à jour la vue (implémentation de IObserver)
     */
    void update() override;

    /**
     * @brief Taille minimale suggérée
     * @return Taille minimale
     */
    QSize sizeHint() const override;

    /**
     * @brief Taille minimale
     * @return Taille minimale
     */
    QSize minimumSizeHint() const override;
};
