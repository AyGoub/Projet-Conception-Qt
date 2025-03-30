#include "messagemodel.h"

MessageModel::MessageModel(
    const std::string& text,
    char referenceChar,
    bool useBlockFormat,
    bool addMarkers,
    int numSectors
    ) : text(text),
    referenceChar(referenceChar),
    useBlockFormat(useBlockFormat),
    addMarkers(addMarkers),
    numSectors(numSectors) {
    convertTextToBits();
}

void MessageModel::setText(const std::string& newText) {
    if (text != newText) {
        text = newText;
        convertTextToBits();
        notifyObservers();
    }
}

void MessageModel::convertTextToBits() {
    bits.clear();

    // Marqueur 0001111111 1111111111
    std::vector<bool> startMarker = {
        false, false, false, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true
    };

    // Convertir chaque caractère en bits
    std::vector<bool> messageOnlyBits;

    for (char c : text) {
        int value = c - referenceChar;

        // Convertir en 7 bits
        for (int i = 6; i >= 0; i--) {
            messageOnlyBits.push_back((value >> i) & 1);
        }

        // Ajouter 3 zéros si le format par blocs est activé
        if (useBlockFormat) {
            messageOnlyBits.push_back(false);
            messageOnlyBits.push_back(false);
            messageOnlyBits.push_back(false);
        }
    }

    // Ajouter les marqueurs et les bits du message en tenant compte des pistes
    if (addMarkers) {
        for (size_t position = 0; position < messageOnlyBits.size(); position++) {
            // Si nous sommes au début d'une piste, ajoutez un marqueur
            if (position % numSectors == 0) {
                bits.insert(bits.end(), startMarker.begin(), startMarker.end());
            }
            
            // Ajoutez le bit du message
            if (position < messageOnlyBits.size()) {
                bits.push_back(messageOnlyBits[position]);
            }
        }
        
        // Ajouter des bits à la fin du message pour compléter la piste courante
        if (!messageOnlyBits.empty()) {
            size_t lastPosition = messageOnlyBits.size() - 1;
            size_t remainingBits = numSectors - (lastPosition % numSectors) - 1;
            
            // Ajouter des zéros pour compléter la piste
            for (size_t i = 0; i < remainingBits; i++) {
                bits.push_back(false);
            }
            
            // Ajouter le marqueur de fin (identique au marqueur de début)
            bits.insert(bits.end(), startMarker.begin(), startMarker.end());
        }
    } else {
        // Pas de marqueurs, on ajoute simplement les bits du message
        bits.insert(bits.end(), messageOnlyBits.begin(), messageOnlyBits.end());
    }
}

bool MessageModel::getBit(size_t index) const {
    if (index < bits.size()) {
        return bits[index];
    }
    return false;
}

size_t MessageModel::getBitsCount() const {
    return bits.size();
}

std::string MessageModel::getText() const {
    return text;
}

char MessageModel::getReferenceChar() const {
    return referenceChar;
}

void MessageModel::setReferenceChar(char newRef) {
    if (referenceChar != newRef) {
        referenceChar = newRef;
        convertTextToBits();
        notifyObservers();
    }
}

void MessageModel::setUseBlockFormat(bool useBlocks) {
    if (useBlockFormat != useBlocks) {
        useBlockFormat = useBlocks;
        convertTextToBits();
        notifyObservers();
    }
}

void MessageModel::setAddMarkers(bool useMarkers) {
    if (addMarkers != useMarkers) {
        addMarkers = useMarkers;
        convertTextToBits();
        notifyObservers();
    }
}

bool MessageModel::getUseBlockFormat() const {
    return useBlockFormat;
}

bool MessageModel::getAddMarkers() const {
    return addMarkers;
}

void MessageModel::setNumSectors(int newNumSectors) {
    if (numSectors != newNumSectors) {
        numSectors = newNumSectors;
        convertTextToBits();
        notifyObservers();
    }
}

int MessageModel::getNumSectors() const {
    return numSectors;
}
