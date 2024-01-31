#include "fiche.h"
#include "ui_fiche.h"
#include <QFile>


Fiche::Fiche(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Fiche)
{
    ui->setupUi(this);


    //Connection des flux pour les boutons
    connect(ui->PBAjout,SIGNAL(clicked()),this,SLOT(PBInteraction_clicked()));
    connect(ui->PBFermer,SIGNAL(clicked()),this, SLOT(on_PBFermer_clicked()));
    connect(ui->PBSupprimer,SIGNAL(clicked()),this,SLOT(supprimerContact()));
    connect(ui->PBModification,SIGNAL(clicked()),this,SLOT(PBModification_clicked()));
    connect(ui->PBInteractions,SIGNAL(clicked()),this,SLOT(PBInteractions_clicked()));
}

void Fiche::recupInteractionModifiee(Interaction I) {

    // Onrécupère l'intéraction modifiée puis on l'envoi à la base de données

    vector<Interaction> interactions = contact.getInteractions();


    for (Interaction& interaction : interactions) {
        if (interaction == I) {
            interaction = I;
            break;
        }
    }

    contact.setInteractions(interactions);
    emit EnvoiModificationsContact(contact);

    //Mise à jour des données de la fiche
    for (Interaction& interaction : interactions) {
        if (interaction == I) {
            interaction = I;

            // On repère les todos qui doivent être supprimés
            vector<Todo> ltodos = I.getTodos();

            for(int j=0;j<ltodos.size();j++){
                if(ltodos[j].getContenu() == "Null"){
                    ltodos.erase(ltodos.begin()+j);
                    j--;
                }
            }
            interaction.setTodos(ltodos);

            break;
        }
    }
    contact.setInteractions(interactions);

}


void Fiche::PBInteractions_clicked(){
    listeInteractions.setWindowModality(Qt::ApplicationModal);
    listeInteractions.show();

    //Connexion des flux avec la QWidget listeInteractions
    connect(this,SIGNAL(EnvoiInteractions(vector<Interaction>)),&listeInteractions,SLOT(initInteraction(vector<Interaction>)));
    connect(&listeInteractions,SIGNAL(EnvoiInteractionModifiee(Interaction)),this,SLOT(recupInteractionModifiee(Interaction)));
    connect(&listeInteractions,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));
    connect(&listeInteractions,SIGNAL(SuppressionInteraction(Interaction)),this,SLOT(SupprimerInteraction(Interaction)));

    emit EnvoiInteractions(contact.getInteractions());
    this->setEnabled(false);
}

void Fiche::PBInteraction_clicked(){
    creerinteraction.setWindowModality(Qt::ApplicationModal);
    creerinteraction.show();

    //Connexion des flux avec la QWidget CreerInteraction
    connect(&creerinteraction,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));
    connect(&creerinteraction,SIGNAL(EnvoiInteraction(Interaction)),this,SLOT(recupInteraction(Interaction)));

    this->setEnabled(false);
}

void Fiche::PBModification_clicked(){
    Modifications.setWindowModality(Qt::ApplicationModal);
    Modifications.show();

    //Connexion des flux avec la QWidget ListeModifications
    connect(&Modifications,SIGNAL(fermer(string)), this,SLOT(fermerQWidget(string)));
    connect(this,SIGNAL(EnvoiModification(vector<Modification>)),&Modifications,SLOT(recupListeModifications(vector<Modification>)));


    emit EnvoiModification(contact.getModifications());

    this->setEnabled(false);
}

void Fiche::recupInteraction(Interaction I){
    emit EnvoiInteraction(I,contact.getID());
}

void Fiche::SupprimerInteraction(Interaction I){
    emit SuppressionInteraction(I);
}

void Fiche::MiseAjourDonnees(){

    ui->Date->setText(QString::fromStdString(contact.DateHeureToString()));
    ui->Entreprise->setText(QString::fromStdString(contact.getEntreprise()));
    ui->Mail->setText(QString::fromStdString(contact.getMail()));
    ui->Nom->setText(QString::fromStdString(contact.getNom()));
    ui->Prenom->setText(QString::fromStdString(contact.getPrenom()));
    ui->ID->setText(QString::fromStdString(contact.getID()));
    ui->Telephone->setText(QString::fromStdString(contact.getTelephone()));

    QString img = "../image/"+QString::fromStdString(contact.getPhoto());

    bool png = img.endsWith(".png",Qt::CaseInsensitive);
    bool jpg = img.endsWith(".jpg",Qt::CaseInsensitive);
    QFile file(img);
    if(file.exists() && (png || jpg)){
        QPixmap photo(img);
        ui->Photo_2->setPixmap(photo);
        ui->Photo_2->setScaledContents(true);
    }
}

void Fiche::initContact(Contact C){
    contact = C;

    emit EnvoiInteractions(contact.getInteractions());

    MiseAjourDonnees();
}


Fiche::~Fiche()
{
    delete ui;
}

void Fiche::on_PBFermer_clicked()
{
    close();
}

void Fiche::recupModificationContact(Contact c){
    //On récupère les modification du contact et on les envoies à la fenêtre principale

    emit EnvoiModificationsContact(c);

    //Mise à jour des données suite aux modifications
    ui->Nom->setText(QString::fromStdString(c.getNom()));
    ui->Prenom->setText(QString::fromStdString(c.getPrenom()));
    ui->Entreprise->setText(QString::fromStdString(c.getEntreprise()));
    ui->Mail->setText(QString::fromStdString(c.getMail()));
    ui->Telephone->setText(QString::fromStdString(c.getTelephone()));

    Modification modification("Modification",c.getID());

    contact.ajoutModification(modification);
}

void Fiche::supprimerContact(){
    emit SupprimerContact(contact);
    emit EnvoiEnregistrement("Suppression",contact.getID());
    this->close();
}


void Fiche::on_PBModifier_clicked()
{
    ModifContact.setWindowModality(Qt::ApplicationModal);
    ModifContact.show();

    //Connexion des flux avec la QWidget ModificationContact
    connect(this,SIGNAL(EnvoiContact(Contact)),&ModifContact,SLOT(recupContact(Contact)));
    connect(&ModifContact,SIGNAL(EnvoiModifications(Contact)),this,SLOT(recupModificationContact(Contact)));
    connect(&ModifContact,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));
    connect(&ModifContact,SIGNAL(EnvoiEnregistrement(string,string)),this,SLOT(EnregistrementModification(string,string)));

    //Envoi du contact dont on souhaite modifié les données
    emit EnvoiContact(contact);

    this->setEnabled(false);
}

void Fiche::EnregistrementModification(string type, string Id){
    emit EnvoiEnregistrement(type,Id);

}

void Fiche::fermerQWidget(string type){

    if(type == "modifiercontact"){
        //Déconnexion des flux avec la QWidget ModificationContact
        disconnect(this,SIGNAL(EnvoiContact(Contact)),&ModifContact,SLOT(recupContact(Contact)));
        disconnect(&ModifContact,SIGNAL(EnvoiModifications(Contact)),this,SLOT(recupModificationContact(Contact)));
        disconnect(&ModifContact,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));
        disconnect(&ModifContact,SIGNAL(EnvoiEnregistrement(string,string)),this,SLOT(EnregistrementModification(string,string)));
    }

    if(type == "listemodifications"){
        //Déconnexion des flux avec la QWidget ListeModifications
        disconnect(&Modifications,SIGNAL(fermer(string)), this,SLOT(fermerQWidget(string)));
        disconnect(this,SIGNAL(EnvoiModification(vector<Modification>)),&Modifications,SLOT(recupListeModifications(vector<Modification>)));
    }

    if(type == "creerinteraction"){
        //Déconnexion des flux avec la QWidget CreerInteraction
        disconnect(&creerinteraction,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));
        disconnect(&creerinteraction,SIGNAL(EnvoiInteraction(Interaction)),this,SLOT(recupInteraction(Interaction)));
    }

    if(type == "listeinteractioncontact"){
        //Déconnexion des flux avec la QWidget listeInteractions
        disconnect(this,SIGNAL(EnvoiInteractions(vector<Interaction>)),&listeInteractions,SLOT(initInteraction(vector<Interaction>)));
        disconnect(&listeInteractions,SIGNAL(EnvoiInteractionModifiee(Interaction)),this,SLOT(recupInteractionModifiee(Interaction)));
        disconnect(&listeInteractions,SIGNAL(fermer(string)),this,SLOT(fermerQWidget(string)));
        disconnect(&listeInteractions,SIGNAL(SuppressionInteraction(Interaction)),this,SLOT(SupprimerInteraction(Interaction)));
    }

    this->setEnabled(true);
}

void Fiche::closeEvent(QCloseEvent *event)
{
    emit fermer();
    QWidget::closeEvent(event);
}

