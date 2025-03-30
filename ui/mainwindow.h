
#pragma once
#include <QMainWindow>
#include <QTextEdit>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QTabWidget>
#include <QTranslator>
#include <QColorDialog>
#include <QCheckBox>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>

#include "model/messagemodel.h"
#include "model/parachutemodel.h"
#include "view/qtparachuteview.h"
#include "view/binaryview.h"
#include "presenter/parachutepresenter.h"

/**
 * @brief Fenêtre principale de l'application
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    // Modèles
    MessageModel* messageModel;
    ParachuteConfigModel* configModel;

    // Vues
    QtParachuteView* parachuteView;
    BinaryView* binaryView;

    // Présentateur
    ParachutePresenter* presenter;

    // Widgets pour les contrôles
    QTextEdit* messageEdit;
    QSlider* sectorsSlider;
    QSpinBox* sectorsSpinBox;
    QSlider* tracksSlider;
    QSpinBox* tracksSpinBox;
    QTabWidget* viewsTabWidget;
    QCheckBox* emptyCenter;
    QCheckBox* useBlockFormat;
    QCheckBox* addMarkers;
    QCheckBox* randomColors;
    QCheckBox* sawtoothEdges;
    QLineEdit* referenceCharEdit;
    QLabel* tracksRequiredLabel;

    // Widgets pour les groupes et labels (nécessaires pour retranslateUi)
    QGroupBox* messageGroup;
    QGroupBox* controlsGroup;
    QGroupBox* formatGroup;
    QGroupBox* displayGroup;
    QLabel* sectorsLabel;
    QLabel* tracksLabel;
    QLabel* referenceCharLabel;
    QPushButton* colorOneBtn;
    QPushButton* colorZeroBtn;

    // Traduction
    QTranslator translator;

    /**
     * @brief Crée les actions et menus
     */
    void createActions();

    /**
     * @brief Crée les widgets et la mise en page
     */
    void createWidgets();

    /**
     * @brief Connecte les signaux et slots
     */
    void connectSignals();

    /**
     * @brief Calcule et met à jour le nombre de pistes nécessaires
     */
    void updateRequiredTracks();

    // Fonction utilitaire pour exporter une vue vers une image
    bool exportViewToImage(QWidget* view, const QString& format);

private slots:
    /**
     * @brief Gère le changement de texte dans le champ de message
     */
    void onMessageChanged();

    /**
     * @brief Gère le changement du nombre de secteurs
     * @param value Nouvelle valeur
     */
    void onSectorsChanged(int value);

    /**
     * @brief Gère le changement du nombre de pistes
     * @param value Nouvelle valeur
     */
    void onTracksChanged(int value);

    /**
     * @brief Ouvre la boîte de dialogue pour choisir la couleur des bits à 1
     */
    void onColorOneChooser();

    /**
     * @brief Ouvre la boîte de dialogue pour choisir la couleur des bits à 0
     */
    void onColorZeroChooser();

    /**
     * @brief Gère le changement de l'option centre vide
     * @param checked État de la case à cocher
     */
    void onEmptyCenterChanged(bool checked);

    /**
     * @brief Gère le changement de l'option format par blocs
     * @param checked État de la case à cocher
     */
    void onBlockFormatChanged(bool checked);

    /**
     * @brief Gère le changement de l'option marqueurs
     * @param checked État de la case à cocher
     */
    void onMarkersChanged(bool checked);

    /**
     * @brief Gère le changement de l'option couleurs aléatoires
     * @param checked État de la case à cocher
     */
    void onRandomColorsChanged(bool checked);

    /**
     * @brief Gère le changement de l'option bords en dents de scie
     * @param checked État de la case à cocher
     */
    void onSawtoothEdgesChanged(bool checked);

    /**
     * @brief Gère le changement du caractère de référence
     * @param text Nouveau caractère de référence
     */
    void onReferenceCharChanged(const QString& text);

    /**
     * @brief Sauvegarde la configuration et le message
     */
    void onSave();

    /**
     * @brief Charge la configuration et le message
     */
    void onLoad();

    /**
     * @brief Change la langue de l'interface
     * @param locale Code de langue (fr, en, etc.)
     */
    void switchLanguage(const QString& locale);

    /**
     * @brief Met à jour les textes de l'interface après un changement de langue
     */
    void retranslateUi();

    /**
     * @brief Affiche la boîte de dialogue À propos
     */
    void onAbout();

    
    void onExportParachutePNG();
    void onExportParachuteJPG();
    void onExportBinaryPNG();
    void onExportBinaryJPG();

public:
    /**
     * @brief Constructeur
     * @param parent Widget parent
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /**
     * @brief Destructeur
     */
    ~MainWindow() override;
};
