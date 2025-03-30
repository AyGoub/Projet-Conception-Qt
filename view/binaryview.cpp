// BinaryView.cpp
#include "binaryview.h"
#include <QPainter>
#include <QResizeEvent>
#include <cmath>
#include "../utils/trapezecalcul.h"

BinaryView::BinaryView(
    MessageModel* messageModel,
    ParachuteConfigModel* configModel,
    QWidget* parent
    ) : QWidget(parent),
    messageModel(messageModel),
    configModel(configModel),
    cellSize(20),
    margin(10),
    columns(16) {

    messageModel->addObserver(this);
    configModel->addObserver(this);

    setMinimumSize(200, 100);
    calculateGridDimensions();
}

BinaryView::~BinaryView() {
    messageModel->removeObserver(this);
    configModel->removeObserver(this);
}

void BinaryView::calculateGridDimensions() {
    // Pour l'affichage vertical, chaque caractère est une colonne de bits
    // On calcule combien de caractères (colonnes) on peut afficher
    int availableWidth = width() - 2 * margin;
    int availableHeight = height() - 2 * margin;
    
    // Nombre de caractères dans le message
    int charCount = messageModel->getText().length();
    if (charCount == 0) charCount = 1; // Au moins une colonne
    
    // Nombre de bits par caractère (7 bits standard + 3 bits supplémentaires si format par blocs)
    int bitsPerChar = 7;
    if (messageModel->getUseBlockFormat()) {
        bitsPerChar = 10;
    }
    
    // Ajuster la hauteur disponible en tenant compte des marqueurs
    if (messageModel->getAddMarkers()) {
        // Réserver de l'espace pour les marqueurs (2 lignes + séparation)
        availableHeight -= (2 * cellSize + 10);
    }
    
    // Calculer la taille optimale des cellules
    int maxCellWidth = availableWidth / charCount;
    int maxCellHeight = availableHeight / bitsPerChar;
    
    // Prendre la plus petite dimension pour garder des cellules carrées
    cellSize = std::min(maxCellWidth, maxCellHeight);
    cellSize = std::max(6, cellSize); // Taille minimale
    
    // Ajuster le nombre de colonnes visibles en fonction de la largeur disponible
    columns = std::max(1, availableWidth / cellSize);
}

void BinaryView::update() {
    calculateGridDimensions();
    QWidget::update(); // Déclencher un repaint
}

void BinaryView::paintEvent(QPaintEvent* /* event */) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Dessiner le fond
    painter.fillRect(rect(), Qt::white);

    // Nombre total de bits
    size_t bitsCount = messageModel->getBitsCount();
    if (bitsCount == 0) {
        return;
    }

    // Récupérer le texte pour déterminer le nombre de caractères
    std::string text = messageModel->getText();
    int charCount = text.length();
    
    // Nombre de bits par caractère
    const int bitsPerChar = 7;
    const int extraBits = messageModel->getUseBlockFormat() ? 3 : 0;
    const int totalBitsPerChar = bitsPerChar + extraBits;
    
    // Gérer les marqueurs si activés
    int markerOffset = 0;
    int markerHeight = 0;
    if (messageModel->getAddMarkers()) {
        // Si des marqueurs sont présents, ils occupent 20 bits au début
        markerOffset = 20;
        // Espace occupé par les marqueurs (2 lignes + séparation)
        markerHeight = 2 * cellSize + 10;
    }
    
    // Dimensions disponibles
    int availableWidth = width() - 2 * margin;
    
    // Calculer le nombre de caractères qu'on peut afficher
    int visibleChars = std::min(charCount, columns);
    
    // Recalculer la taille des cellules pour optimiser l'affichage
    if (visibleChars > 0) {
        cellSize = std::min(availableWidth / visibleChars, (height() - 2 * margin - markerHeight) / totalBitsPerChar);
        cellSize = std::max(6, cellSize); // Taille minimale
    }
    
    int circleSize = cellSize * 0.7; // Cercle plus petit que la cellule
    int circleMargin = (cellSize - circleSize) / 2;
    
    // Récupérer les couleurs depuis le modèle de configuration
    QColor colorOne = configModel->getColorOne();
    QColor colorZero = configModel->getColorZero();
    bool useRandomColors = configModel->usesRandomColors();
    
    // Calculer le nombre de pistes nécessaires pour encoder le message
    int numSectors = configModel->getNumSectors();
    int tracksRequired = 1; // Au moins une piste
    
    if (numSectors > 0) {
        // Nombre de bits par piste (= nombre de secteurs)
        int bitsPerTrack = numSectors;
        
        // Si le centre est vide, on ne peut pas utiliser la première piste
        int effectiveBitsPerTrack = bitsPerTrack;
        if (configModel->hasEmptyCenter()) {
            tracksRequired++; // On commence à la deuxième piste
        }
        
        // Calculer combien de pistes complètes sont nécessaires
        tracksRequired += static_cast<int>(bitsCount / effectiveBitsPerTrack);
        
        // Si il y a un reste, on a besoin d'une piste supplémentaire
        if (bitsCount % effectiveBitsPerTrack > 0) {
            tracksRequired++;
        }
    }
    
    // Afficher le nombre de pistes nécessaires en haut à droite
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    QString tracksText = tr("Pistes nécessaires: %1").arg(tracksRequired);
    QRect textRect = painter.fontMetrics().boundingRect(tracksText);
    painter.drawText(width() - textRect.width() - margin, margin + textRect.height(), tracksText);
    
    // Dessiner les marqueurs si nécessaire
    if (messageModel->getAddMarkers()) {
        // Dessiner les marqueurs (20 bits) en haut
        for (int i = 0; i < 20; i++) {
            int col = i % 10;  // 10 bits par ligne pour les marqueurs
            int row = i / 10;  // 2 lignes de marqueurs
            
            int x = margin + col * cellSize + circleMargin;
            int y = margin + row * cellSize + circleMargin;
            
            bool bitValue = messageModel->getBit(i);
            
            // Utiliser les couleurs du modèle
            QColor color;
            if (useRandomColors) {
                // Couleurs aléatoires basées sur la position
                int hue = (col * 25 + row * 50) % 360;
                color = bitValue ? QColor::fromHsv(hue, 255, 255) : QColor::fromHsv(hue, 50, 200);
            } else {
                // Couleurs définies par l'utilisateur
                color = bitValue ? colorOne : colorZero;
            }
            
            // Définir le style de pinceau selon la valeur du bit
            if (bitValue) {
                painter.setPen(Qt::NoPen); // Pas de contour pour les bits 1
            } else {
                painter.setPen(QPen(Qt::black, 1)); // Contour noir pour les bits 0
            }
            
            painter.setBrush(color);
            painter.drawEllipse(x, y, circleSize, circleSize);
        }
        
        // Dessiner une ligne de séparation
        painter.setPen(Qt::gray);
        painter.drawLine(margin, margin + 2 * cellSize + 5, 
                         margin + 10 * cellSize, margin + 2 * cellSize + 5);
    }
    
    // Dessiner les caractères verticalement
    for (int charIndex = 0; charIndex < visibleChars; charIndex++) {
        // Position de base pour ce caractère
        int baseX = margin + charIndex * cellSize;
        
        // Dessiner les 7 bits de ce caractère verticalement
        for (int bitPos = 0; bitPos < bitsPerChar; bitPos++) {
            // Calculer l'index du bit dans le tableau de bits
            int bitIndex = markerOffset + charIndex * totalBitsPerChar + bitPos;
            
            // Vérifier que l'index est valide
            if (bitIndex >= 0 && static_cast<size_t>(bitIndex) < bitsCount) {
                int x = baseX + circleMargin;
                int y = margin + bitPos * cellSize + circleMargin;
                
                if (messageModel->getAddMarkers()) {
                    // Ajouter un décalage si des marqueurs sont présents
                    y += markerHeight;
                }
                
                bool bitValue = messageModel->getBit(bitIndex);
                
                // Utiliser les couleurs du modèle
                QColor color;
                if (useRandomColors) {
                    // Couleurs aléatoires basées sur la position
                    int hue = (charIndex * 30 + bitPos * 20) % 360;
                    color = bitValue ? QColor::fromHsv(hue, 255, 255) : QColor::fromHsv(hue, 50, 200);
                } else {
                    // Couleurs définies par l'utilisateur
                    color = bitValue ? colorOne : colorZero;
                }
                
                // Définir le style de pinceau selon la valeur du bit
                if (bitValue) {
                    painter.setPen(Qt::NoPen); // Pas de contour pour les bits 1
                } else {
                    painter.setPen(QPen(Qt::black, 1)); // Contour noir pour les bits 0
                }
                
                painter.setBrush(color);
                painter.drawEllipse(x, y, circleSize, circleSize);
            }
        }
        
        // Si le format par blocs est activé, ajouter les 3 bits supplémentaires
        if (messageModel->getUseBlockFormat()) {
            for (int extraBit = 0; extraBit < 3; extraBit++) {
                int bitIndex = markerOffset + charIndex * totalBitsPerChar + bitsPerChar + extraBit;
                
                if (bitIndex >= 0 && static_cast<size_t>(bitIndex) < bitsCount) {
                    int x = baseX + circleMargin;
                    int y = margin + (bitsPerChar + extraBit) * cellSize + circleMargin;
                    
                    if (messageModel->getAddMarkers()) {
                        // Ajouter un décalage si des marqueurs sont présents
                        y += markerHeight;
                    }
                    
                    bool bitValue = messageModel->getBit(bitIndex);
                    
                    // Utiliser les couleurs du modèle
                    QColor color;
                    if (useRandomColors) {
                        // Couleurs aléatoires basées sur la position
                        int hue = (charIndex * 30 + (bitsPerChar + extraBit) * 20) % 360;
                        color = bitValue ? QColor::fromHsv(hue, 255, 255) : QColor::fromHsv(hue, 50, 200);
                    } else {
                        // Couleurs définies par l'utilisateur
                        color = bitValue ? colorOne : colorZero;
                    }
                    
                    // Définir le style de pinceau selon la valeur du bit
                    if (bitValue) {
                        painter.setPen(Qt::NoPen); // Pas de contour pour les bits 1
                    } else {
                        painter.setPen(QPen(Qt::black, 1)); // Contour noir pour les bits 0
                    }
                    
                    painter.setBrush(color);
                    painter.drawEllipse(x, y, circleSize, circleSize);
                }
            }
        }
    }
}

void BinaryView::resizeEvent(QResizeEvent* /* event */) {
    calculateGridDimensions();
}

QSize BinaryView::sizeHint() const {
    return QSize(columns * cellSize + 2 * margin, 200);
}

QSize BinaryView::minimumSizeHint() const {
    return QSize(200, 100);
}
