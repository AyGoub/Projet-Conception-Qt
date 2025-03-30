
#include "parachutepresenter.h"
#include "../utils/trapezecalcul.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>
#include <QDebug>

ParachutePresenter::ParachutePresenter(
    MessageModel* messageModel,
    ParachuteConfigModel* configModel,
    ParachuteView* view
    ) : messageModel(messageModel),
    configModel(configModel),
    view(view) {

    messageModel->addObserver(this);
    configModel->addObserver(this);
}

ParachutePresenter::~ParachutePresenter() {
    messageModel->removeObserver(this);
    configModel->removeObserver(this);
}

void ParachutePresenter::update() {
    updateDisplay();
}

void ParachutePresenter::updateDisplay() {
    view->clear();

    // Dessiner la structure du parachute
    double radius = configModel->getRadius();
    int numTracks = configModel->getNumTracks();
    int numSectors = configModel->getNumSectors();

    view->drawParachuteOutline(radius);
    view->drawSectorLines(radius, numSectors);
    view->drawTrackCircles(radius, numTracks);

    // Nombre maximum de trapèzes
    int maxTrapezoids = numSectors * numTracks;
    int bitsCount = static_cast<int>(messageModel->getBitsCount());

    // Calculer et dessiner tous les trapèzes (pas seulement jusqu'à bitsCount)
    for (int k = 0; k < maxTrapezoids; k++) {
        // Obtenir la valeur du bit (0 si on dépasse la longueur du message)
        bool bitValue = (k < bitsCount) ? messageModel->getBit(k) : false;

        // Calculer les sommets du trapèze avec décalage si nécessaire
        std::vector<Point> vertices = TrapezoidCalculator::calculateTrapezoidVertices(
            k,
            radius,
            numTracks,
            numSectors,
            configModel->usesSawtoothEdges(),
            configModel->hasEmptyCenter()
        );

        // Déterminer la couleur
        QColor color = TrapezoidCalculator::getTrapezoidColor(bitValue, *configModel);

        // Dessiner le trapèze
        view->drawTrapezoid(vertices, color, configModel->hasEmptyCenter());
    }
}

bool ParachutePresenter::saveToFile(const std::string& filePath) const {
    QJsonObject root;

    // Enregistrer le message
    QJsonObject messageObj;
    messageObj["text"] = QString::fromStdString(messageModel->getText());
    messageObj["referenceChar"] = QString(messageModel->getReferenceChar());
    messageObj["useBlockFormat"] = messageModel->getUseBlockFormat();
    messageObj["addMarkers"] = messageModel->getAddMarkers();
    root["message"] = messageObj;

    // Enregistrer la configuration
    QJsonObject configObj;
    configObj["radius"] = configModel->getRadius();
    configObj["numTracks"] = configModel->getNumTracks();
    configObj["numSectors"] = configModel->getNumSectors();
    configObj["emptyCenter"] = configModel->hasEmptyCenter();
    configObj["colorOne"] = configModel->getColorOne().name();
    configObj["colorZero"] = configModel->getColorZero().name();
    configObj["randomColors"] = configModel->usesRandomColors();
    configObj["sawtoothEdges"] = configModel->usesSawtoothEdges();
    root["config"] = configObj;

    // Écrire dans le fichier
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();

    return true;
}

bool ParachutePresenter::loadFromFile(const std::string& filePath) {
    QString qFilePath = QString::fromStdString(filePath);
    
    // Vérifier l'extension du fichier
    if (!qFilePath.endsWith(".para", Qt::CaseInsensitive)) {
        qWarning() << "Le fichier n'a pas l'extension .para:" << qFilePath;
        // Continuer quand même, le fichier pourrait être valide
    }
    
    QFile file(qFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Impossible d'ouvrir le fichier:" << qFilePath 
                   << "- Erreur:" << file.errorString();
        return false;
    }

    QByteArray data = file.readAll();
    file.close();
    
    // Vérification rapide du contenu - les fichiers JSON commencent par {
    if (data.isEmpty() || (data[0] != '{' && data[0] != '[')) {
        qWarning() << "Le fichier ne semble pas être un fichier JSON valide:" << qFilePath;
        return false;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (doc.isNull()) {
        qWarning() << "Erreur d'analyse JSON:" << parseError.errorString() 
                   << "à la position:" << parseError.offset;
        return false;
    }
    
    if (!doc.isObject()) {
        qWarning() << "Le document JSON n'est pas un objet:" << qFilePath;
        return false;
    }

    QJsonObject root = doc.object();
    bool success = true;

    // Vérifier la présence des sections requises
    if (!root.contains("message") || !root["message"].isObject()) {
        qWarning() << "Section 'message' manquante ou invalide dans le fichier";
        success = false;
    }
    
    if (!root.contains("config") || !root["config"].isObject()) {
        qWarning() << "Section 'config' manquante ou invalide dans le fichier";
        success = false;
    }
    
    // Si une des sections principales est manquante, on arrête
    if (!success) {
        return false;
    }

    // Charger le message
    QJsonObject messageObj = root["message"].toObject();
    if (messageObj.contains("text") && messageObj["text"].isString()) {
        std::string text = messageObj["text"].toString().toStdString();
        messageModel->setText(text);
    } else {
        qWarning() << "Pas de texte trouvé dans le fichier";
        success = false;
    }

    if (messageObj.contains("referenceChar") && messageObj["referenceChar"].isString()) {
        QString refStr = messageObj["referenceChar"].toString();
        if (!refStr.isEmpty()) {
            messageModel->setReferenceChar(refStr.at(0).toLatin1());
        }
    } else {
        qWarning() << "Pas de caractère de référence trouvé dans le fichier";
        success = false;
    }

    if (messageObj.contains("useBlockFormat") && messageObj["useBlockFormat"].isBool()) {
        messageModel->setUseBlockFormat(messageObj["useBlockFormat"].toBool());
    }

    if (messageObj.contains("addMarkers") && messageObj["addMarkers"].isBool()) {
        messageModel->setAddMarkers(messageObj["addMarkers"].toBool());
    }

    // Charger la configuration
    QJsonObject configObj = root["config"].toObject();
    if (configObj.contains("radius") && configObj["radius"].isDouble()) {
        configModel->setRadius(configObj["radius"].toDouble());
    }

    if (configObj.contains("numTracks") && configObj["numTracks"].isDouble()) {
        configModel->setNumTracks(configObj["numTracks"].toInt());
    }

    if (configObj.contains("numSectors") && configObj["numSectors"].isDouble()) {
        configModel->setNumSectors(configObj["numSectors"].toInt());
    }

    if (configObj.contains("emptyCenter") && configObj["emptyCenter"].isBool()) {
        configModel->setEmptyCenter(configObj["emptyCenter"].toBool());
    }

    if (configObj.contains("colorOne") && configObj["colorOne"].isString()) {
        configModel->setColorOne(QColor(configObj["colorOne"].toString()));
    }

    if (configObj.contains("colorZero") && configObj["colorZero"].isString()) {
        configModel->setColorZero(QColor(configObj["colorZero"].toString()));
    }

    if (configObj.contains("randomColors") && configObj["randomColors"].isBool()) {
        configModel->setRandomColors(configObj["randomColors"].toBool());
    }

    if (configObj.contains("sawtoothEdges") && configObj["sawtoothEdges"].isBool()) {
        configModel->setSawtoothEdges(configObj["sawtoothEdges"].toBool());
    }

    // Même si certains champs sont manquants, on continue avec ce qu'on a pu charger
    return success;
}
