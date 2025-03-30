QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    model/messagemodel.cpp \
    model/parachutemodel.cpp \
    presenter/parachutepresenter.cpp \
    ui/mainwindow.cpp \
    utils/Observer.cpp \
    utils/trapezecalcul.cpp \
    view/binaryview.cpp \
    view/qtparachuteview.cpp

HEADERS += \
    model/messagemodel.h \
    model/parachutemodel.h \
    presenter/parachutepresenter.h \
    ui/mainwindow.h \
    utils/Observer.h \
    utils/Point.h \
    utils/trapezecalcul.h \
    view/binaryview.h \
    view/parachuteview.h \
    view/qtparachuteview.h

# Configuration pour les traductions
TRANSLATIONS += \
    translations/parachute_fr.ts

# Inclure les fichiers de traduction dans les ressources
RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
