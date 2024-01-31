#include "cherchercontact.h"
#include "ui_cherchercontact.h"
#include <fiche.h>

ChercherContact::ChercherContact(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChercherContact)
{
    ui->setupUi(this);

    //Connexion du flux pour le triage de la liste de contact
    connect(ui->PBAlphabet,SIGNAL(clicked()),this,SLOT(TrieAlphabetique()));
    connect(ui->PBDate,SIGNAL(clicked()),this,SLOT(TrieDate()));

    //Connexion des flux avec La liste de contact
    connect(ui->Table2,SIGNAL(clicked(QModelIndex)),this,SLOT(ContactCliqueTable(QModelIndex)));

    //Connexion des flux pour la recherche
    connect(ui->PBLancer,SIGNAL(clicked()),this,SLOT(CliqueLancerRecherche()));
    connect(ui->PBListeComplete,SIGNAL(clicked()),this,SLOT(ListeComplete()));
}

ChercherContact::~ChercherContact()
{
    delete ui;
}

void ChercherContact::recupInteraction(Interaction I, string contactId){
    emit EnvoiInteraction(I, contactId);
}

void ChercherContact::ListeComplete(){
    ListeCourante = Liste;
    MiseAJourTable(Liste);
}

void ChercherContact::CliqueLancerRecherche(){
    string Nom = ui->LineNom->text().toStdString();
    string Entreprise = ui->LineEntreprise->text().toStdString();

    string Date1 = ui->RDate->text().toStdString();

    //On récupère les jours, mois et années des dates dans des listes
    vector<string> D1;

    stringstream ss(Date1);
    string partie;

    //On sépare La Date et heure ("jj/mm/aaaa" et "hh:mm")
    vector<string> Dh1;
    while (std::getline(ss, partie, ' ')) {
        Dh1.push_back(partie);
    }

    //On découpe la Date pour séparer le jour le mois et l'année
    stringstream sd(Dh1.at(0));

    while (std::getline(sd, partie, '/')) {
        D1.push_back(partie);
    }

    //On découpe l'Heure pour séparer les heures et les minutes
    stringstream sh(Dh1.at(1));

    while (std::getline(sh, partie, ':')) {
        D1.push_back(partie);
    }

    seconds_point date1 = date::sys_days{date::year_month_day{date::year(stoi(D1.at(2))), date::month(stoi(D1.at(1))), date::day(stoi(D1.at(0)))}}+
                          std::chrono::hours{stoi(D1.at(3))} + std::chrono::minutes{stoi(D1.at(4))};

    //On récupère le nombre de jours écoulés depuis le 01/01/1970
    int nbjours1 = stoi(to_string(date1.time_since_epoch().count()));

    vector<Contact> liste;

    for(Contact c : Liste){

        int nbsecondsContact = stoi(c.getDate());
        bool verifDate = ((nbsecondsContact >= nbjours1) && (nbsecondsContact <= nbjours1+60));

        if(c.getNom() == Nom || c.getEntreprise() == Entreprise || verifDate){
            liste.push_back(c);
        }
    }
    ListeCourante = liste;

    MiseAJourTable(liste);
}

void ChercherContact::recupListeContact(vector<Contact> listeContacts){
    Liste = listeContacts;
    ListeCourante = listeContacts;
    MiseAJourTable(Liste);

    //Mise A Jour des données
    if(BFiche){
        for(Contact c : listeContacts){
            if(c == contactFiche){
                contactFiche = c;
                EnvoiContact(contactFiche);
            }
        }
    }
}



void ChercherContact::MiseAJourTable(vector<Contact> liste){

    ui->Table2->setColumnCount(3);
    ui->Table2->setHorizontalHeaderLabels({"Nom","Prénom","Date de création"});

    //Supprime toutes les lignes de la table (Réinitialise la table)
    for (int i=0; i<ui->Table2->rowCount();i++){
        ui->Table2->removeRow(i);
        i--;
    }

    for(int i=0; i<liste.size();i++){
        Contact c = liste.at(i);

        int ligne = ui->Table2->rowCount();
        ui->Table2->insertRow(ligne);

        QString Nom = QString::fromStdString(c.getNom());
        QString Prenom = QString::fromStdString(c.getPrenom());
        QString Date = QString::fromStdString(c.DateHeureToString());

        QTableWidgetItem* itemNom = new QTableWidgetItem(Nom);
        ui->Table2->setItem(ligne,0,itemNom);

        QTableWidgetItem* itemPrenom = new QTableWidgetItem(Prenom);
        ui->Table2->setItem(ligne,1,itemPrenom);

        QTableWidgetItem* itemDate = new QTableWidgetItem(Date);
        ui->Table2->setItem(ligne,2,itemDate);
    }

    ui->Table2->setColumnWidth(0,150);
    ui->Table2->setColumnWidth(1,150);
    ui->Table2->setColumnWidth(2,150);

    // Mise à jour du nombre de contacts dans la liste courante
    QString NombreDeContact = QString::number(liste.size(), 10);
    ui->NbContact->setText(NombreDeContact);
}

void ChercherContact::recupModificationContact(Contact c){
    emit EnvoiModificationContact(c);

    for(Contact contact : Liste){
        if (contact == c){
            contact = c;
        }
    }

    MiseAJourTable(Liste);

}

void ChercherContact::ContactCliqueTable(const QModelIndex &index) {

    Contact c = ListeCourante.at(index.row());
    fiche.setWindowModality(Qt::ApplicationModal);
    fiche.show();

    //Connexion des flux avec l'objet fiche
    connect(this, SIGNAL(EnvoiContact(Contact)), &fiche, SLOT(initContact(Contact)));
    connect(&fiche,SIGNAL(EnvoiModificationsContact(Contact)),this,SLOT(recupModificationContact(Contact)));
    connect(&fiche,SIGNAL(fermer()),this,SLOT(fermerQWidget()));
    connect(&fiche,SIGNAL(SupprimerContact(Contact)),this,SLOT(recupSuppressionContact(Contact)));
    connect(&fiche,SIGNAL(EnvoiInteraction(Interaction, string)),this,SLOT(recupInteraction(Interaction, string)));
    connect(&fiche,SIGNAL(SuppressionInteraction(Interaction)),this,SLOT(SupprimerInteraction(Interaction)));
    connect(&fiche,SIGNAL(EnvoiEnregistrement(string,string)),this,SLOT(recupEnregistrement(string,string)));

    // On envoie les données du contact à la fiche
    contactFiche = c;
    BFiche = true;
    emit EnvoiContact(c);

    this->setEnabled(false);
}

void ChercherContact::recupEnregistrement(string type, string Id){
    emit EnregistrementModification(type,Id);
}

void ChercherContact::SupprimerInteraction(Interaction I){
    emit SuppressionInteraction(I);

    //Mise à jour des données
    for(int i=0; i<Liste.size(); i++){
        if(Liste.at(i).getID() == I.getIDContact()){
            Liste.at(i).supprimerInteraction(I);
        }
    }
    MiseAJourTable(Liste);
}

void ChercherContact::recupSuppressionContact(Contact c){
    emit SupprimerContact(c);

    for(int i=0; i<Liste.size();i++){
        if(Liste.at(i) == c){
            Liste.erase(Liste.begin()+i);
        }
    }
    MiseAJourTable(Liste);
}

void ChercherContact::closeEvent(QCloseEvent *event)
{
    emit fermer("cherchercontact");
    QWidget::closeEvent(event);
}

void ChercherContact::fermerQWidget(){

    //Connexion des flux avec l'objet fiche
    disconnect(this, SIGNAL(EnvoiContact(Contact)), &fiche, SLOT(initContact(Contact)));
    disconnect(&fiche,SIGNAL(EnvoiModificationsContact(Contact)),this,SLOT(recupModificationContact(Contact)));
    disconnect(&fiche,SIGNAL(fermer()),this,SLOT(fermerQWidget()));
    disconnect(&fiche,SIGNAL(SupprimerContact(Contact)),this,SLOT(recupSuppressionContact(Contact)));
    disconnect(&fiche,SIGNAL(EnvoiInteraction(Interaction, string)),this,SLOT(recupInteraction(Interaction, string)));
    disconnect(&fiche,SIGNAL(SuppressionInteraction(Interaction)),this,SLOT(SupprimerInteraction(Interaction)));
    disconnect(&fiche,SIGNAL(EnvoiEnregistrement(string,string)),this,SLOT(recupEnregistrement(string,string)));

    BFiche = false;
    this->setEnabled(true);
}

// Foncteur qui me permet de comparer les contacts par nom et prénom
struct ComparateurContactAlphabet {
    bool operator()(Contact &a, Contact &b) const {
        // Comparaison par nom, puis par prénom en cas d'égalité
        return (a.getNom() == b.getNom()) ? (a.getPrenom() < b.getPrenom()) : (a.getNom() < b.getNom());
    }
};

// Fonction pour trier un vector<Contact> par ordre alphabétique
void ChercherContact::TrieAlphabetique() {
    std::sort(Liste.begin(), Liste.end(), ComparateurContactAlphabet());
    ListeCourante = Liste;
    MiseAJourTable(Liste);
}

// Foncteur personnalisé pour comparer les contacts par date de création
struct ComparateurContactDate {
    bool operator()(Contact &a, Contact &b) const {
        // Comparaison par nom, puis par prénom en cas d'égalité
        return a.getDate() < b.getDate();
    }
};

// Fonction pour trier un vector<Contact> par ordre alphabétique
void ChercherContact::TrieDate() {
    std::sort(Liste.begin(), Liste.end(), ComparateurContactDate());
    ListeCourante = Liste;
    MiseAJourTable(Liste);
}
