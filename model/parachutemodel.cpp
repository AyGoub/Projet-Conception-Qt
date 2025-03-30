#include "parachutemodel.h"

ParachuteConfigModel::ParachuteConfigModel(
    double radius,
    int numTracks,
    int numSectors,
    bool emptyCenter
    ) : radius(radius),
    numTracks(numTracks),
    numSectors(numSectors),
    emptyCenter(emptyCenter),
    colorOne(Qt::red),
    colorZero(Qt::white),
    randomColors(false),
    sawtoothEdges(false) {
}

double ParachuteConfigModel::getRadius() const {
    return radius;
}

int ParachuteConfigModel::getNumTracks() const {
    return numTracks;
}

int ParachuteConfigModel::getNumSectors() const {
    return numSectors;
}

bool ParachuteConfigModel::hasEmptyCenter() const {
    return emptyCenter;
}

QColor ParachuteConfigModel::getColorOne() const {
    return colorOne;
}

QColor ParachuteConfigModel::getColorZero() const {
    return colorZero;
}

bool ParachuteConfigModel::usesRandomColors() const {
    return randomColors;
}

bool ParachuteConfigModel::usesSawtoothEdges() const {
    return sawtoothEdges;
}

void ParachuteConfigModel::setRadius(double newRadius) {
    if (newRadius > 0 && radius != newRadius) {
        radius = newRadius;
        notifyObservers();
    }
}

void ParachuteConfigModel::setNumTracks(int newNumTracks) {
    if (newNumTracks > 0 && numTracks != newNumTracks) {
        numTracks = newNumTracks;
        notifyObservers();
    }
}

void ParachuteConfigModel::setNumSectors(int newNumSectors) {
    if (newNumSectors > 0 && numSectors != newNumSectors) {
        numSectors = newNumSectors;
        notifyObservers();
    }
}

void ParachuteConfigModel::setEmptyCenter(bool isEmpty) {
    if (emptyCenter != isEmpty) {
        emptyCenter = isEmpty;
        notifyObservers();
    }
}

void ParachuteConfigModel::setColorOne(const QColor& color) {
    if (colorOne != color) {
        colorOne = color;
        notifyObservers();
    }
}

void ParachuteConfigModel::setColorZero(const QColor& color) {
    if (colorZero != color) {
        colorZero = color;
        notifyObservers();
    }
}

void ParachuteConfigModel::setRandomColors(bool useRandom) {
    if (randomColors != useRandom) {
        randomColors = useRandom;
        notifyObservers();
    }
}

void ParachuteConfigModel::setSawtoothEdges(bool useSawtooth) {
    if (sawtoothEdges != useSawtooth) {
        sawtoothEdges = useSawtooth;
        notifyObservers();
    }
}
