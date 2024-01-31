QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Contact.cpp \
    GestionBD.cpp \
    Interaction.cpp \
    Modification.cpp \
    Todo.cpp \
    cherchercontact.cpp \
    chercherinteraction.cpp \
    creercontact.cpp \
    creerinteraction.cpp \
    fenetreprincipale.cpp \
    fiche.cpp \
    ficheinteraction.cpp \
    historique.cpp \
    listeinteractioncontact.cpp \
    listemodifications.cpp \
    main.cpp \
    modifiercontact.cpp

HEADERS += \
    Contact.h \
    GestionBD.h \
    Interaction.h \
    Modification.h \
    Todo.h \
    cherchercontact.h \
    chercherinteraction.h \
    creercontact.h \
    creerinteraction.h \
    fenetreprincipale.h \
    fiche.h \
    ficheinteraction.h \
    historique.h \
    listeinteractioncontact.h \
    listemodifications.h \
    modifiercontact.h

FORMS += \
    cherchercontact.ui \
    chercherinteraction.ui \
    creercontact.ui \
    creerinteraction.ui \
    fenetreprincipale.ui \
    fiche.ui \
    ficheinteraction.ui \
    historique.ui \
    listeinteractioncontact.ui \
    listemodifications.ui \
    modifiercontact.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
