
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QSpinBox>
#include <algorithm>
#include <QFileInfo>
#include <QPixmap>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    
    messageModel = new MessageModel("ENSICAEN_RULES");
    configModel = new ParachuteConfigModel();

    // Créer les vues
    parachuteView = new QtParachuteView();
    binaryView = new BinaryView(messageModel, configModel);

    // Créer le présentateur
    presenter = new ParachutePresenter(messageModel, configModel, parachuteView);

    // Initialiser l'interface
    createWidgets();
    createActions();
    connectSignals();

    // Mettre à jour l'affichage initial
    presenter->updateDisplay();

    // Configurer la fenêtre principale
    setWindowTitle(tr("Message Encoder for Parachute"));
    setMinimumSize(800, 600);
}

MainWindow::~MainWindow() {
    delete presenter;
    delete binaryView;
    delete messageModel;
    delete configModel;
}

void MainWindow::createWidgets() {
    
    QWidget* centralWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    messageGroup = new QGroupBox(tr("Message"));
    QVBoxLayout* messageLayout = new QVBoxLayout(messageGroup);
    messageEdit = new QTextEdit();
    messageEdit->setPlainText(QString::fromStdString(messageModel->getText()));
    messageEdit->setMaximumHeight(60);
    messageLayout->addWidget(messageEdit);

    
    controlsGroup = new QGroupBox(tr("Configuration"));
    QFormLayout* controlsLayout = new QFormLayout(controlsGroup);

    
    QHBoxLayout* sectorsLayout = new QHBoxLayout();
    sectorsSlider = new QSlider(Qt::Horizontal);
    
    
    int step = messageModel->getUseBlockFormat() ? 10 : 7;
    int minSectors = step; // Au moins un multiple de 7 ou 10
    int maxSectors = 70;   // Maximum 70 secteurs (10 caractères ou ~10 caractères)
    
    
    int currentSectors = configModel->getNumSectors();
    int adjustedSectors = ((currentSectors + step/2) / step) * step;
    adjustedSectors = std::max(minSectors, std::min(maxSectors, adjustedSectors));
    
    // Configurer le slider avec des étapes
    sectorsSlider->setRange(minSectors, maxSectors);
    sectorsSlider->setSingleStep(step);
    sectorsSlider->setPageStep(step);
    sectorsSlider->setValue(adjustedSectors);
    
    // Configurer le spinbox
    sectorsSpinBox = new QSpinBox();
    sectorsSpinBox->setRange(minSectors, maxSectors);
    sectorsSpinBox->setSingleStep(step);
    sectorsSpinBox->setValue(adjustedSectors);
    
    // Mettre à jour le modèle avec la valeur ajustée
    configModel->setNumSectors(adjustedSectors);
    
    sectorsLayout->addWidget(sectorsSlider);
    sectorsLayout->addWidget(sectorsSpinBox);
    sectorsLabel = new QLabel(tr("Sectors:"));
    controlsLayout->addRow(sectorsLabel, sectorsLayout);

    // Contrôles pour les pistes
    QHBoxLayout* tracksLayout = new QHBoxLayout();
    tracksSlider = new QSlider(Qt::Horizontal);
    tracksSlider->setRange(1, 10);
    tracksSlider->setValue(configModel->getNumTracks());
    tracksSpinBox = new QSpinBox();
    tracksSpinBox->setRange(1, 10);
    tracksSpinBox->setValue(configModel->getNumTracks());
    tracksLayout->addWidget(tracksSlider);
    tracksLayout->addWidget(tracksSpinBox);
    tracksLabel = new QLabel(tr("Tracks:"));
    controlsLayout->addRow(tracksLabel, tracksLayout);

    // Label pour afficher le nombre de pistes nécessaires
    tracksRequiredLabel = new QLabel();
    tracksRequiredLabel->setStyleSheet("font-weight: bold; color: #3366CC;");
    controlsLayout->addRow("", tracksRequiredLabel);
    updateRequiredTracks();

    // Contrôle pour le caractère de référence
    referenceCharEdit = new QLineEdit();
    referenceCharEdit->setMaxLength(1);
    referenceCharEdit->setText(QString(messageModel->getReferenceChar()));
    referenceCharLabel = new QLabel(tr("Reference Character:"));
    controlsLayout->addRow(referenceCharLabel, referenceCharEdit);

    // Options supplémentaires
    QHBoxLayout* optionsLayout = new QHBoxLayout();

    // Options pour le format
    formatGroup = new QGroupBox(tr("Format Options"));
    QVBoxLayout* formatLayout = new QVBoxLayout(formatGroup);

    useBlockFormat = new QCheckBox(tr("Use 10-element blocks (7 bits + 3 zeros)"));
    useBlockFormat->setChecked(messageModel->getUseBlockFormat());
    formatLayout->addWidget(useBlockFormat);

    addMarkers = new QCheckBox(tr("Add start/end markers"));
    addMarkers->setChecked(messageModel->getAddMarkers());
    formatLayout->addWidget(addMarkers);

    optionsLayout->addWidget(formatGroup);

    // Options pour l'affichage
    displayGroup = new QGroupBox(tr("Display Options"));
    QVBoxLayout* displayLayout = new QVBoxLayout(displayGroup);

    emptyCenter = new QCheckBox(tr("Empty center disk"));
    emptyCenter->setChecked(configModel->hasEmptyCenter());
    displayLayout->addWidget(emptyCenter);

    randomColors = new QCheckBox(tr("Random colors for bits"));
    randomColors->setChecked(configModel->usesRandomColors());
    displayLayout->addWidget(randomColors);

    sawtoothEdges = new QCheckBox(tr("Sawtooth edges"));
    sawtoothEdges->setChecked(configModel->usesSawtoothEdges());
    displayLayout->addWidget(sawtoothEdges);

    // Boutons pour les couleurs
    QHBoxLayout* colorsLayout = new QHBoxLayout();
    colorOneBtn = new QPushButton(tr("Color for 1"));
    colorZeroBtn = new QPushButton(tr("Color for 0"));
    colorsLayout->addWidget(colorOneBtn);
    colorsLayout->addWidget(colorZeroBtn);
    displayLayout->addLayout(colorsLayout);

    optionsLayout->addWidget(displayGroup);

    // Onglets pour les vues
    viewsTabWidget = new QTabWidget();
    viewsTabWidget->addTab(parachuteView, tr("Parachute View"));
    viewsTabWidget->addTab(binaryView, tr("Binary View"));

    // Créer un layout horizontal pour les contrôles et options
    QHBoxLayout* controlsOptionsLayout = new QHBoxLayout();
    controlsOptionsLayout->addWidget(controlsGroup);
    controlsOptionsLayout->addLayout(optionsLayout);

    // Ajouter tous les widgets au layout principal
    mainLayout->addWidget(messageGroup);
    mainLayout->addLayout(controlsOptionsLayout);
    mainLayout->addWidget(viewsTabWidget, 3);

    // Définir le widget central
    setCentralWidget(centralWidget);

    // Connecter les boutons de couleurs
    connect(colorOneBtn, &QPushButton::clicked, this, &MainWindow::onColorOneChooser);
    connect(colorZeroBtn, &QPushButton::clicked, this, &MainWindow::onColorZeroChooser);
}

void MainWindow::createActions() {
    // Menu Fichier
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    QAction* openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openAction);
    connect(openAction, &QAction::triggered, this, &MainWindow::onLoad);

    QAction* saveAction = new QAction(tr("&Save..."), this);
    saveAction->setShortcut(QKeySequence::Save);
    fileMenu->addAction(saveAction);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSave);

    // Ajouter menu d'exportation
    QMenu* exportMenu = menuBar()->addMenu(tr("&Export"));
    
    // Export du parachute
    QAction* exportParachutePNG = new QAction(tr("Parachute as &PNG"), this);
    exportMenu->addAction(exportParachutePNG);
    connect(exportParachutePNG, &QAction::triggered, this, &MainWindow::onExportParachutePNG);
    
    QAction* exportParachuteJPG = new QAction(tr("Parachute as &JPG"), this);
    exportMenu->addAction(exportParachuteJPG);
    connect(exportParachuteJPG, &QAction::triggered, this, &MainWindow::onExportParachuteJPG);
    
    // Export de la vue binaire
    exportMenu->addSeparator();
    QAction* exportBinaryPNG = new QAction(tr("Binary View as P&NG"), this);
    exportMenu->addAction(exportBinaryPNG);
    connect(exportBinaryPNG, &QAction::triggered, this, &MainWindow::onExportBinaryPNG);
    
    QAction* exportBinaryJPG = new QAction(tr("Binary View as J&PG"), this);
    exportMenu->addAction(exportBinaryJPG);
    connect(exportBinaryJPG, &QAction::triggered, this, &MainWindow::onExportBinaryJPG);

    fileMenu->addSeparator();

    QAction* exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    // Menu Langue
    QMenu* languageMenu = menuBar()->addMenu(tr("&Language"));

    QAction* frenchAction = new QAction(tr("&French"), this);
    languageMenu->addAction(frenchAction);
    connect(frenchAction, &QAction::triggered, [this]() { switchLanguage("fr"); });

    QAction* englishAction = new QAction(tr("&English"), this);
    languageMenu->addAction(englishAction);
    connect(englishAction, &QAction::triggered, [this]() { switchLanguage("en"); });

    // Menu Aide
    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));

    QAction* aboutAction = new QAction(tr("&About"), this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);

    // Barre d'état
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::connectSignals() {
    // Synchroniser les contrôles
    connect(sectorsSlider, &QSlider::valueChanged, sectorsSpinBox, &QSpinBox::setValue);
    connect(sectorsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), sectorsSlider, &QSlider::setValue);
    connect(tracksSlider, &QSlider::valueChanged, tracksSpinBox, &QSpinBox::setValue);
    connect(tracksSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), tracksSlider, &QSlider::setValue);

    // Connecter les contrôles aux actions
    connect(messageEdit, &QTextEdit::textChanged, this, &MainWindow::onMessageChanged);
    connect(sectorsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onSectorsChanged);
    connect(tracksSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onTracksChanged);
    connect(emptyCenter, &QCheckBox::toggled, this, &MainWindow::onEmptyCenterChanged);
    connect(useBlockFormat, &QCheckBox::toggled, this, &MainWindow::onBlockFormatChanged);
    connect(addMarkers, &QCheckBox::toggled, this, &MainWindow::onMarkersChanged);
    connect(randomColors, &QCheckBox::toggled, this, &MainWindow::onRandomColorsChanged);
    connect(sawtoothEdges, &QCheckBox::toggled, this, &MainWindow::onSawtoothEdgesChanged);
    connect(referenceCharEdit, &QLineEdit::textChanged, this, &MainWindow::onReferenceCharChanged);
}

void MainWindow::onMessageChanged() {
    messageModel->setText(messageEdit->toPlainText().toStdString());
    updateRequiredTracks();
}

void MainWindow::onSectorsChanged(int value) {
    // Ajuster la valeur au multiple le plus proche
    int step = messageModel->getUseBlockFormat() ? 10 : 7;
    int adjustedValue = (value / step) * step;
    
    // Si la valeur est plus proche du multiple suivant, utiliser celui-ci
    if (value % step > step / 2 && adjustedValue + step <= sectorsSlider->maximum()) {
        adjustedValue += step;
    }
    
    // Si la valeur ajustée est différente, mettre à jour les contrôles
    if (value != adjustedValue) {
        sectorsSlider->setValue(adjustedValue);
        sectorsSpinBox->setValue(adjustedValue);
    }
    
    // Mettre à jour le modèle
    configModel->setNumSectors(adjustedValue);
    updateRequiredTracks();
}

void MainWindow::onTracksChanged(int value) {
    configModel->setNumTracks(value);
}

void MainWindow::onColorOneChooser() {
    QColor color = QColorDialog::getColor(configModel->getColorOne(), this, tr("Choose Color for Bit Value 1"));
    if (color.isValid()) {
        configModel->setColorOne(color);
    }
}

void MainWindow::onColorZeroChooser() {
    QColor color = QColorDialog::getColor(configModel->getColorZero(), this, tr("Choose Color for Bit Value 0"));
    if (color.isValid()) {
        configModel->setColorZero(color);
    }
}

void MainWindow::onEmptyCenterChanged(bool checked) {
    configModel->setEmptyCenter(checked);
    updateRequiredTracks();
}

void MainWindow::onBlockFormatChanged(bool checked) {
    messageModel->setUseBlockFormat(checked);
    
    // Mettre à jour les contrôles de secteurs pour utiliser le bon pas (7 ou 10)
    int step = checked ? 10 : 7;
    int minSectors = step;
    int maxSectors = 70;
    
    // Ajuster la valeur actuelle au multiple le plus proche
    int currentSectors = configModel->getNumSectors();
    int adjustedSectors = ((currentSectors + step/2) / step) * step;
    adjustedSectors = std::max(minSectors, std::min(maxSectors, adjustedSectors));
    
    // Mettre à jour les contrôles
    sectorsSlider->setRange(minSectors, maxSectors);
    sectorsSlider->setSingleStep(step);
    sectorsSlider->setPageStep(step);
    sectorsSlider->setValue(adjustedSectors);
    
    sectorsSpinBox->setRange(minSectors, maxSectors);
    sectorsSpinBox->setSingleStep(step);
    sectorsSpinBox->setValue(adjustedSectors);
    
    // Mettre à jour le modèle
    configModel->setNumSectors(adjustedSectors);
    updateRequiredTracks();
}

void MainWindow::onMarkersChanged(bool checked) {
    messageModel->setAddMarkers(checked);
    updateRequiredTracks();
}

void MainWindow::onRandomColorsChanged(bool checked) {
    configModel->setRandomColors(checked);
}

void MainWindow::onSawtoothEdgesChanged(bool checked) {
    configModel->setSawtoothEdges(checked);
}

void MainWindow::onReferenceCharChanged(const QString& text) {
    if (!text.isEmpty()) {
        messageModel->setReferenceChar(text.at(0).toLatin1());
    }
}

void MainWindow::onSave() {
    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Save Configuration"),
        QString(),
        tr("Parachute Files (*.para)")
        );

    if (!filePath.isEmpty()) {
        // Vérifier que le fichier a l'extension .para
        if (!filePath.endsWith(".para", Qt::CaseInsensitive)) {
            filePath += ".para";
        }
        
        if (presenter->saveToFile(filePath.toStdString())) {
            statusBar()->showMessage(tr("File saved successfully"), 3000);
        } else {
            QMessageBox::warning(this, tr("Save Error"), tr("Failed to save the file."));
        }
    }
}

void MainWindow::onLoad() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Load Configuration"),
        QString(),
        tr("Parachute Files (*.para);;All Files (*)")
        );

    if (!filePath.isEmpty()) {
        // Vérifier que le fichier existe
        QFileInfo fileInfo(filePath);
        if (!fileInfo.exists()) {
            QMessageBox::warning(this, tr("Load Error"), 
                                tr("File does not exist: %1").arg(filePath));
            return;
        }
        
        if (presenter->loadFromFile(filePath.toStdString())) {
            // Mettre à jour l'interface
            messageEdit->setPlainText(QString::fromStdString(messageModel->getText()));
            sectorsSlider->setValue(configModel->getNumSectors());
            tracksSlider->setValue(configModel->getNumTracks());
            emptyCenter->setChecked(configModel->hasEmptyCenter());
            useBlockFormat->setChecked(messageModel->getUseBlockFormat());
            addMarkers->setChecked(messageModel->getAddMarkers());
            randomColors->setChecked(configModel->usesRandomColors());
            sawtoothEdges->setChecked(configModel->usesSawtoothEdges());
            referenceCharEdit->setText(QString(messageModel->getReferenceChar()));

            statusBar()->showMessage(tr("File loaded successfully"), 3000);
        } else {
            QString errorDetails = tr("The file format is invalid or the file is corrupted.\n\n"
                                  "Make sure this is a valid .para file created by this application.");
            
            if (!filePath.endsWith(".para", Qt::CaseInsensitive)) {
                errorDetails += tr("\n\nNote: The file does not have a .para extension.");
            }
            
            QMessageBox::warning(this, tr("Load Error"), 
                                tr("Failed to load the file: %1\n\n%2").arg(filePath).arg(errorDetails));
        }
    }
}

void MainWindow::switchLanguage(const QString& locale) {
    // Charger le fichier de traduction
    translator.load(QString(":/translations/parachute_%1.qm").arg(locale));
    qApp->installTranslator(&translator);

    // Mettre à jour l'interface
    this->retranslateUi();
}

void MainWindow::retranslateUi() {
    // Mettre à jour tous les textes de l'interface
    setWindowTitle(tr("Message Encoder for Parachute"));

    // Groupes
    messageGroup->setTitle(tr("Message"));
    controlsGroup->setTitle(tr("Configuration"));
    formatGroup->setTitle(tr("Format Options"));
    displayGroup->setTitle(tr("Display Options"));

    // Options
    useBlockFormat->setText(tr("Use 10-element blocks (7 bits + 3 zeros)"));
    addMarkers->setText(tr("Add start/end markers"));
    emptyCenter->setText(tr("Empty center disk"));
    randomColors->setText(tr("Random colors for bits"));
    sawtoothEdges->setText(tr("Sawtooth edges"));

    // Boutons et labels
    colorOneBtn->setText(tr("Color for 1"));
    colorZeroBtn->setText(tr("Color for 0"));
    sectorsLabel->setText(tr("Sectors:"));
    tracksLabel->setText(tr("Tracks:"));
    referenceCharLabel->setText(tr("Reference Character:"));
    
    // Mettre à jour le texte du nombre de pistes nécessaires
    updateRequiredTracks();

    // Onglets
    viewsTabWidget->setTabText(0, tr("Parachute View"));
    viewsTabWidget->setTabText(1, tr("Binary View"));

    // Menus (recréer pour mettre à jour)
    menuBar()->clear();
    createActions();

    // Barre d'état
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::onAbout() {
    QMessageBox::about(
        this,
        tr("About Message Encoder for Parachute"),
        tr("Message Encoder for Parachute\n\n"
           "This application was developed by Ayoub Goubraim and Saad Talemsi.\n\n"
           "Version: 1.2.0\n\n"
           "Inspired by NASA's Perseverance Mars rover parachute.\n\n"
           "Graphical Interface Design Course at ENSICAEN.\n\n"
           "© 2024-2025")
        );
}

// Fonction utilitaire pour exporter une vue en image
bool MainWindow::exportViewToImage(QWidget* view, const QString& format) {
    // 1. DEMANDER LE CHEMIN DE SAUVEGARDE
    // ==================================
    QString fileFormat = format.toLower();
    QString filter = tr("Image Files (*.%1)").arg(fileFormat);
    QString title = tr("Export as %1").arg(format);
    
    QString filePath = QFileDialog::getSaveFileName(
        this, 
        title,
        QString(), 
        filter
    );
    
    if (filePath.isEmpty()) {
        return false;  // L'utilisateur a annulé
    }
    
    // Ajouter l'extension si nécessaire
    if (!filePath.toLower().endsWith("." + fileFormat)) {
        filePath += "." + fileFormat;
    }
    
    // 2. CRÉER UNE IMAGE À PARTIR DE LA VUE
    // ===================================
    QPixmap pixmap(view->size());
    view->render(&pixmap);
    
    // 3. SAUVEGARDER L'IMAGE
    // ===================
    bool success = pixmap.save(filePath, fileFormat.toStdString().c_str());
    
    // 4. INFORMER L'UTILISATEUR
    // ======================
    if (success) {
        statusBar()->showMessage(tr("Image exported successfully to %1").arg(filePath), 3000);
    } else {
        QMessageBox::warning(
            this,
            tr("Export Error"),
            tr("Failed to export the image to %1").arg(filePath)
        );
    }
    
    return success;
}

void MainWindow::onExportParachutePNG() {
    // Trouver la vue parachute dans le widget à onglets
    QWidget* parachuteViewTab = viewsTabWidget->widget(0); // Supposant que la vue parachute est le premier onglet
    if (parachuteViewTab) {
        exportViewToImage(parachuteViewTab, "PNG");
    }
}

void MainWindow::onExportParachuteJPG() {
    QWidget* parachuteViewTab = viewsTabWidget->widget(0);
    if (parachuteViewTab) {
        exportViewToImage(parachuteViewTab, "JPG");
    }
}

void MainWindow::onExportBinaryPNG() {
    QWidget* binaryViewTab = viewsTabWidget->widget(1); // Supposant que la vue binaire est le deuxième onglet
    if (binaryViewTab) {
        exportViewToImage(binaryViewTab, "PNG");
    }
}

void MainWindow::onExportBinaryJPG() {
    QWidget* binaryViewTab = viewsTabWidget->widget(1);
    if (binaryViewTab) {
        exportViewToImage(binaryViewTab, "JPG");
    }
}

void MainWindow::updateRequiredTracks() {
    // Récupérer le nombre de secteurs actuel
    int numSectors = configModel->getNumSectors();
    
    // Récupérer le nombre total de bits du message
    size_t bitsCount = messageModel->getBitsCount();
    
    // Calculer le nombre de pistes nécessaires
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
    tracksRequired -= 1;
    
    // Mettre à jour le label avec le nombre de pistes nécessaires
    tracksRequiredLabel->setText(tr("Required tracks: %1").arg(tracksRequired));
    
    // Si le nombre de pistes nécessaires est supérieur au nombre configuré, afficher en rouge
    if (tracksRequired > configModel->getNumTracks()) {
        tracksRequiredLabel->setStyleSheet("font-weight: bold; color: red;");
    } else {
        tracksRequiredLabel->setStyleSheet("font-weight: bold; color: green;");
    }
}
