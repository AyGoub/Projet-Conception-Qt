// main.cpp
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QIcon>
#include "ui/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Configurer l'application
    app.setApplicationName("ParachuteEncoder");
    app.setOrganizationName("ENSICAEN");
    app.setApplicationDisplayName("Message Encoder for Parachute");
    app.setWindowIcon(QIcon(":/images/parachute_icon.svg"));
    app.setOrganizationDomain("ensicaen.fr");
    app.setApplicationVersion("1.2.0");

    // Charger les traductions selon la locale système
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "parachute_" + QLocale(locale).name();
        if (translator.load(":/translations/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    // Créer et afficher la fenêtre principale
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
