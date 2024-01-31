#include "fenetreprincipale.h"
#include "ui_fenetreprincipale.h"
#include <Contact.h>
#include <iostream>
#include <QMessageBox>
#include <QFile>

FenetrePrincipale::FenetrePrincipale(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FenetrePrincipale)
{
    ui->setupUi(this);

    barreMenu = new QMenuBar(nullptr);
    setMenuBar(barreMenu);

    //Ajout des entrées pour le Menu
    QMenu *MenuCreer = new QMenu(QString::fromUtf8("&Créer"),this);
    barreMenu->addMenu(MenuCreer);

    QMenu *MenuRechercher = new QMenu(QString::fromUtf8("&Rechercher"),this);
    barreMenu->addMenu(MenuRechercher);

    QMenu *MenuExport = new QMenu(QString::fromUtf8("&Export"),this);
    barreMenu->addMenu(MenuExport);

    //Ajout de QAction
    QAction *actionContact = new QAction(QString::fromUtf8("&Contact"),this);
    connect(actionContact, SIGNAL(triggered()),this,SLOT(lancerCreationContact()));
    MenuCreer->addAction(actionContact);

    QAction *actionContacts = new QAction(QString::fromUtf8("&Contacts"),this);
    connect(actionContacts,SIGNAL(triggered()),this,SLOT(lancerRechercheContact()));
    MenuRechercher->addAction(actionContacts);

    QAction *actionInteractions = new QAction(QString::fromUtf8("&Interactions"),this);
    connect(actionInteractions,SIGNAL(triggered()),this,SLOT(lancerRechercheInteraction()));
    MenuRechercher->addAction(actionInteractions);

    QAction *actionModifications = new QAction(QString::fromUtf8("&Modifications"),this);
    connect(actionModifications,SIGNAL(triggered()),this,SLOT(lancerHistorique()));
    MenuRechercher->addAction(actionModifications);

    QAction *actionJSON = new QAction(QString::fromUtf8("&JSON"),this);
    connect(actionJSON,SIGNAL(triggered(bool)),this,SLOT(exportJSON()));
    MenuExport->addAction(actionJSON);

    //Connexion des flux avec le gestionnaire de base de données
    connect(&gestionbd,SIGNAL(MiseAJour()),this,SLOT(MiseAJour()));
    connect(this,SIGNAL(deconnexionBD()),&gestionbd,SLOT(deconnexionBD()));
    connect(this,SIGNAL(AjoutInteractionTodo(Interaction,string)),&gestionbd,SLOT(ajoutInteractionTodo(Interaction,string)));
    connect(this,SIGNAL(modificationContact(Contact)),&gestionbd,SLOT(modificationContact(Contact)));
    connect(this,SIGNAL(MiseAJourListeContacts()),&gestionbd,SLOT(MiseAJourListeContacts()));
    connect(&gestionbd,SIGNAL(EnvoiListes(vector<Contact>, vector<Modification>)),this,SLOT(recupListes(vector<Contact>, vector<Modification>)));
    connect(this,SIGNAL(EnvoiEnregistrement(string,string)),&gestionbd,SLOT(EnregistrerModifiaction(string,string)));
    connect(this,SIGNAL(ExportJSON()),&gestionbd,SLOT(exportJSON()));
    connect(this,SIGNAL(suppressionContact(Contact)),&gestionbd,SLOT(suppressionContact(Contact)));
    connect(this,SIGNAL(ajoutContact(Contact)),&gestionbd,SLOT(ajoutContact(Contact)));

    //Ajout du logo de l'université de Bourgogne
    QString img = "../image/Ub.png";

    QFile file(img);
    if(file.exists()){
        QPixmap photo(img);
        ui->Ub->setPixmap(photo);
        ui->Ub->setFixedSize((photo.width()/2),(photo.height()/2));
        ui->Ub->setScaledContents(true);
    }
}

FenetrePrincipale::~FenetrePrincipale()
{
    emit deconnexionBD();  //On déconnecte la base de données
    delete ui;
}

void FenetrePrincipale::recupInteraction(Interaction I, string contactId){
    emit AjoutInteractionTodo(I, contactId);
}

void FenetrePrincipale::recupListes(vector<Contact> ListeC, vector<Modification> ListeM){
    ListeContacts = ListeC;
    ListeModifications = ListeM;
}

void FenetrePrincipale::recupModificationContact(Contact c){
    emit modificationContact(c);
}

void FenetrePrincipale::MiseAJour(){
    //On récupère la liste de contact de la BD puis on l'envoie à cherchContact
    emit MiseAJourListeContacts();
    emit envoiListeContact(ListeContacts);
    emit envoiListeInteraction(ListeContacts);
}

void FenetrePrincipale::lancerRechercheContact(){
    cherchContact.setWindowModality(Qt::ApplicationModal);
    cherchContact.show();

    //Connexion des flux avec le QWidget cherchercontact
    connect(&cherchContact,SIGNAL(EnvoiModificationContact(Contact)),this,SLOT(recupModificationContact(Contact)));
    connect(&cherchContact,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));
    connect(this,SIGNAL(envoiListeContact(vector<Contact>)),&cherchContact,SLOT(recupListeContact(vector<Contact>)));
    connect(&cherchContact,SIGNAL(SupprimerContact(Contact)),this,SLOT(recupSuppression(Contact)));
    connect(&cherchContact,SIGNAL(EnvoiInteraction(Interaction, string)),this,SLOT(recupInteraction(Interaction, string)));
    connect(&cherchContact,SIGNAL(SuppressionInteraction(Interaction)),this,SLOT(SuppressionInteraction(Interaction)));
    connect(&cherchContact,SIGNAL(EnregistrementModification(string,string)),this,SLOT(EnregistrementModification(string,string)));

    //On récupère la liste de contact de la BD puis on l'envoie à cherchContact
    emit MiseAJourListeContacts();
    emit envoiListeContact(ListeContacts);

    this->setEnabled(false);
}

void FenetrePrincipale::EnregistrementModification(string type, string IDContact){
    emit EnvoiEnregistrement(type, IDContact);
}

void FenetrePrincipale::lancerRechercheInteraction(){
    cherchInteraction.setWindowModality(Qt::ApplicationModal);
    cherchInteraction.show();

    //Connesion des flux avec le QWidget chercherinteraction
    connect(&cherchInteraction,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));
    connect(this,SIGNAL(envoiListeInteraction(vector<Contact>)),&cherchInteraction,SLOT(initListes(vector<Contact>)));
    connect(&cherchInteraction,SIGNAL(EnvoiContact(Contact)),this,SLOT(recupModificationContact(Contact)));
    connect(&cherchInteraction,SIGNAL(SuppressionInteraction(Interaction)),this,SLOT(SuppressionInteraction(Interaction)));

    //On récupère la liste d'intéraction de la BD puis on l'envoie à cherchInteraction
    emit MiseAJourListeContacts();
    emit envoiListeInteraction(ListeContacts);

    this->setEnabled(false);
}

void FenetrePrincipale::lancerHistorique(){
    historique.setWindowModality(Qt::ApplicationModal);
    historique.show();

    connect(this,SIGNAL(envoiHistorique(vector<Modification>)),&historique,SLOT(ListModifications(vector<Modification>)));
    connect(&historique,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));

    emit MiseAJourListeContacts();
    emit envoiHistorique(ListeModifications);

    this->setEnabled(false);
}

void FenetrePrincipale::SuppressionInteraction(Interaction I){
    emit SupprimerInteraction(I);

    //On récupère la liste de contact de la BD puis on l'envoie à cherchContact
    emit MiseAJourListeContacts();
    emit envoiListeContact(ListeContacts);
    emit envoiListeInteraction(ListeContacts);
}

bool FenetrePrincipale::VerificationContact(Contact c){
    //On vérifie si le contact n'existe pas déjà dans la base de données

    vector<Contact> Liste = ListeContacts;
    for(Contact contact : Liste){
        if(contact.getNom() == c.getNom() && contact.getPrenom() == c.getPrenom()
            && contact.getEntreprise() == c.getEntreprise() && contact.getPhoto() == c.getPhoto()
            && contact.getMail() == c.getMail() && contact.getTelephone() == c.getTelephone()){
            return false;
        }
    }
    return true;
}

void FenetrePrincipale::lancerCreationContact(){
    cc.setWindowModality(Qt::ApplicationModal);
    cc.show();

    //Connexion des flux avec le QWidget creercontact
    connect(&cc, SIGNAL(validerContact(Contact)), this, SLOT(creerContact(Contact)));
    connect(&cc,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));

    this->setEnabled(false);
}

void FenetrePrincipale::exportJSON(){
    emit ExportJSON();
}

void FenetrePrincipale::recupSuppression(Contact c){
    emit suppressionContact(c);

}

void FenetrePrincipale::fermerQWidget(string type){

    if(type == "creercontact"){
        //Déconnexion des flux avec le QWidget creercontact
        disconnect(&cc, SIGNAL(validerContact(Contact)), this, SLOT(creerContact(Contact)));
        disconnect(&cc,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));
    }

    if(type == "cherchercontact"){
        //Déconnexion des flux avec le QWidget cherchercontact
        disconnect(&cherchContact,SIGNAL(EnvoiModificationContact(Contact)),this,SLOT(recupModificationContact(Contact)));
        disconnect(&cherchContact,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));
        disconnect(this,SIGNAL(envoiListeContact(vector<Contact>)),&cherchContact,SLOT(recupListeContact(vector<Contact>)));
        disconnect(&cherchContact,SIGNAL(SupprimerContact(Contact)),this,SLOT(recupSuppression(Contact)));
        disconnect(&cherchContact,SIGNAL(EnvoiInteraction(Interaction, string)),this,SLOT(recupInteraction(Interaction, string)));
        disconnect(&cherchContact,SIGNAL(SuppressionInteraction(Interaction)),this,SLOT(SuppressionInteraction(Interaction)));
        disconnect(&cherchContact,SIGNAL(EnregistrementModification(string,string)),this,SLOT(EnregistrementModification(string,string)));

    }

    if(type == "chercherinteraction"){
        //Déconnexion des flux avec le QWidget chercherinteraction
        disconnect(&cherchInteraction,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));
        disconnect(this,SIGNAL(envoiListeInteraction(vector<Contact>)),&cherchInteraction,SLOT(initListes(vector<Contact>)));
        disconnect(&cherchInteraction,SIGNAL(EnvoiContact(Contact)),this,SLOT(recupModificationContact(Contact)));
        disconnect(&cherchInteraction,SIGNAL(SuppressionInteraction(Interaction)),this,SLOT(SuppressionInteraction(Interaction)));
    }

    if(type == "Historique"){
        //Déconnexion des flux avec le QWidget Historique
        disconnect(this,SIGNAL(envoiHistorique(vector<Modification>)),&historique,SLOT(ListModifications(vector<Modification>)));
        disconnect(&historique,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));
    }

    this->setEnabled(true);
}

void FenetrePrincipale::creerContact(Contact c) {
    if(VerificationContact(c)){
        cc.Initialisation();
        emit ajoutContact(c);
    }
    else{
        QMessageBox::critical(this, "Erreur de vérification", "Le contact est déjà enregistré dans la Base de données.");
    }
}

