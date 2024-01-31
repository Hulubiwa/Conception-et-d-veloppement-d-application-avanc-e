#include "chercherinteraction.h"
#include "ui_chercherinteraction.h"
#include <QMessageBox>

ChercherInteraction::ChercherInteraction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChercherInteraction)
{
    ui->setupUi(this);
}

ChercherInteraction::~ChercherInteraction()
{
    delete ui;
}

void ChercherInteraction::initListes(vector<Contact> liste){
    ui->Table->clear();
    ListeContacts.clear();
    ListeInteractions.clear();
    ListeTodos.clear();
    ListeContacts = liste;

    //Connexions des boutons
    connect(ui->PBInteractions,SIGNAL(clicked(bool)),this,SLOT(PBInteractions()));
    connect(ui->PBTodos,SIGNAL(clicked(bool)),this,SLOT(PBTodos()));
    connect(ui->Table,SIGNAL(clicked(QModelIndex)),SLOT(CliqueTable(QModelIndex)));
    connect(ui->PBFermer,SIGNAL(clicked(bool)),this,SLOT(PBFermer()));
    connect(ui->PBLancer,SIGNAL(clicked(bool)),this,SLOT(PBRechercher()));
    connect(ui->PBDate,SIGNAL(clicked(bool)),this,SLOT(PBDate()));
    connect(ui->PBListe,SIGNAL(clicked(bool)),this,SLOT(PBListe()));

    //initialisation des listes d'intéractions et de todos
    for(Contact contact : ListeContacts){
        vector<Interaction> linteractcion = contact.getInteractions();
        for(Interaction interaction : linteractcion){
            ListeInteractions.push_back(interaction);
            vector<Todo> ltodos = interaction.getTodos();
            for(Todo todo : ltodos){
                ListeTodos.push_back(todo);
            }
        }
    }
    MiseAJour("Interactions");
}

void ChercherInteraction::MiseAJourListes(){
    ListeInteractions.clear();
    ListeTodos.clear();

    for(Contact contact : ListeContacts){
        vector<Interaction> linteractcion = contact.getInteractions();
        for(Interaction interaction : linteractcion){
            ListeInteractions.push_back(interaction);
            vector<Todo> ltodos = interaction.getTodos();
            for(Todo todo : ltodos){
                ListeTodos.push_back(todo);
            }
        }
    }
    MiseAJour(typeListe);
}

Contact ChercherInteraction::getContactByID(string Id){

    /// On récupère le Contact de la liste des contacts
    for(int i=0; i<ListeContacts.size();i++){
        Contact c = ListeContacts.at(i);
        if(Id == c.getID()){
            return c;
        }

    }

    return Contact();
}

Interaction ChercherInteraction::getInteractionByID(string Id){
    for(Interaction I : ListeInteractions){
        if(Id == I.getID()){
            return I;
        }
    }

    ///On retourne une Intéraction vide si on ne trouve pas l'intéraction recherchée
    vector<string> contenu;
    contenu.push_back("vide");
    Interaction vide(contenu);
    vide.setID("Vide");
    return vide;
}

void ChercherInteraction::MiseAJour(string type){

    if(type == "Interactions"){
        ui->Table->setColumnCount(3);
        ui->Table->setHorizontalHeaderLabels({"Nom","Prénom", "Date de création"});

        //Supprime toutes les lignes de la table (Réinitialise la table)
        for (int i=0; i<ui->Table->rowCount();i++){
            ui->Table->removeRow(i);
            i--;
        }

        for(int i=0; i<ListeInteractions.size();i++){
            Interaction I = ListeInteractions.at(i);
            Contact c = getContactByID(I.getIDContact());

            int ligne = ui->Table->rowCount();
            ui->Table->insertRow(ligne);

            QString Nom = QString::fromStdString(c.getNom());
            QString Prenom = QString::fromStdString(c.getPrenom());
            QString Date = QString::fromStdString(I.DateHeureToString());

            QTableWidgetItem* itemNom = new QTableWidgetItem(Nom);
            ui->Table->setItem(ligne,0,itemNom);

            QTableWidgetItem* itemPrenom = new QTableWidgetItem(Prenom);
            ui->Table->setItem(ligne,1,itemPrenom);

            QTableWidgetItem* itemDate = new QTableWidgetItem(Date);
            ui->Table->setItem(ligne,2,itemDate);
        }
        typeListe = "Interactions";
    }


    if(type == "Todos"){
        ui->Table->setColumnCount(3);
        ui->Table->setHorizontalHeaderLabels({"Nom","Prénom", "Date"});

        //Supprime toutes les lignes de la table (Réinitialise la table)
        for (int i=0; i<ui->Table->rowCount();i++){
            ui->Table->removeRow(i);
            i--;
        }

        for(int i=0; i<ListeTodos.size();i++){
            Todo t = ListeTodos.at(i);
            Contact c = getContactByID(t.getIDContact());

            int ligne = ui->Table->rowCount();
            ui->Table->insertRow(ligne);

            QString Nom = QString::fromStdString(c.getNom());
            QString Prenom = QString::fromStdString(c.getPrenom());
            QString Date = QString::fromStdString(t.DateHeureToString());

            QTableWidgetItem* itemNom = new QTableWidgetItem(Nom);
            ui->Table->setItem(ligne,0,itemNom);

            QTableWidgetItem* itemPrenom = new QTableWidgetItem(Prenom);
            ui->Table->setItem(ligne,1,itemPrenom);

            QTableWidgetItem* itemDate = new QTableWidgetItem(Date);
            ui->Table->setItem(ligne,2,itemDate);
        }
        typeListe = "Todos";
    }

    ui->Table->setColumnWidth(0,150);
    ui->Table->setColumnWidth(1,150);
    ui->Table->setColumnWidth(2,150);
}

void ChercherInteraction::CliqueTable(const QModelIndex &index) {

    ///On ouvre la Fiche de l'intéraction sélectionnée

    if(typeListe == "Interactions"){
        Interaction interaction = ListeInteractions.at(index.row());

        fiche.setWindowModality(Qt::ApplicationModal);
        fiche.show();

        //Connexion des flux avec la QWidget fiche
        connect(&fiche,SIGNAL(fermer()),this,SLOT(fermerQWidget()));
        connect(this,SIGNAL(EnvoiInteraction(Interaction)),&fiche,SLOT(recupInteraction(Interaction)));
        connect(&fiche,SIGNAL(EnvoiInteractionModifiee(Interaction)),this,SLOT(recupInteractionModifiee(Interaction)));
        connect(&fiche,SIGNAL(SuppressionInteraction(Interaction)),this,SLOT(SupprimerInteraction(Interaction)));

        emit EnvoiInteraction(interaction);

        this->setEnabled(false);
    }

    if(typeListe == "Todos"){
        Todo todo = ListeTodos.at(index.row());
        Interaction interaction= getInteractionByID(todo.getIDInteraction());

        fiche.setWindowModality(Qt::ApplicationModal);
        fiche.show();

        //Connexion des flux avec la QWidget fiche
        connect(&fiche,SIGNAL(fermer()),this,SLOT(fermerQWidget()));
        connect(this,SIGNAL(EnvoiInteraction(Interaction)),&fiche,SLOT(recupInteraction(Interaction)));
        connect(&fiche,SIGNAL(EnvoiInteractionModifiee(Interaction)),this,SLOT(recupInteractionModifiee(Interaction)));
        connect(&fiche,SIGNAL(SuppressionInteraction(Interaction)),this,SLOT(SupprimerInteraction(Interaction)));

        emit EnvoiInteraction(interaction);

        this->setEnabled(false);
    }
}

void ChercherInteraction::recupInteractionModifiee(Interaction I){
    Contact c = getContactByID(I.getIDContact());
    vector<Interaction> liste = c.getInteractions();
    for(Interaction &interaction : liste){
        if(interaction == I){
            interaction = I;

        }
    }
    c.setInteractions(liste);
    emit EnvoiContact(c);
}

void ChercherInteraction::SupprimerInteraction(Interaction I){
    emit SuppressionInteraction(I);
}

void ChercherInteraction::PBInteractions(){
    MiseAJour("Interactions");
}

void ChercherInteraction::PBTodos(){
    MiseAJour("Todos");
}

void ChercherInteraction::PBFermer(){
    close();
}

void ChercherInteraction::PBRechercher() {
    string Nom = ui->Nom->text().toStdString();
    string Prenom = ui->Prenom->text().toStdString();
        vector<Contact> lcontact;

    for (Contact c : ListeContacts) {
        if ((Nom == "" || c.getNom() == Nom) && (Prenom == "" || c.getPrenom() == Prenom)) {
            lcontact.push_back(c);
        }
    }

    if (typeListe == "Interactions") {
        vector<Interaction> liste;

        for (Contact c : lcontact) {
            for (Interaction I : c.getInteractions()) {
                liste.push_back(I);
            }
        }

        ListeInteractions = liste;
        MiseAJour("Interactions");
    } else if (typeListe == "Todos") {
        vector<Todo> liste;

        for (Contact c : lcontact) {
            for (Interaction I : c.getInteractions()) {
                for (Todo t : I.getTodos()) {
                    liste.push_back(t);
                }
            }
        }

        ListeTodos = liste;
        MiseAJour("Todos");
    }

}

void ChercherInteraction::PBDate(){
    string Date1 = ui->Date_1->text().toStdString();
    string Date2 = ui->Date_2->text().toStdString();

    //On récupère les jours, mois et années des dates dans des listes
    vector<string> D1;
    vector<string> D2;

    stringstream ss(Date1);
    stringstream ss2(Date2);
    string partie;

    //On sépare La Date et heure ("jj/mm/aaaa" et "hh:mm")
    vector<string> Dh1;
    vector<string> Dh2;
    while (std::getline(ss, partie, ' ')) {
        Dh1.push_back(partie);
    }

    while (std::getline(ss2, partie, ' ')) {
        Dh2.push_back(partie);
    }

    //On découpe la Date pour séparer le jour le mois et l'année
    stringstream sd(Dh1.at(0));
    stringstream sd2(Dh2.at(0));

    while (std::getline(sd, partie, '/')) {
        D1.push_back(partie);
    }

    while (std::getline(sd2, partie, '/')) {
        D2.push_back(partie);
    }

    //On découpe l'Heure pour séparer les heures et les minutes
    stringstream sh(Dh1.at(1));
    stringstream sh2(Dh2.at(1));

    while (std::getline(sh, partie, ':')) {
        D1.push_back(partie);
    }

    while (std::getline(sh2, partie, ':')) {
        D2.push_back(partie);
    }

    seconds_point date1 = date::sys_days{date::year_month_day{date::year(stoi(D1.at(2))), date::month(stoi(D1.at(1))), date::day(stoi(D1.at(0)))}}+
           std::chrono::hours{stoi(D1.at(3))} + std::chrono::minutes{stoi(D1.at(4))};
    seconds_point date2 = date::sys_days{date::year_month_day{date::year(stoi(D2.at(2))), date::month(stoi(D2.at(1))), date::day(stoi(D2.at(0)))}}+
           std::chrono::hours{stoi(D2.at(3))} + std::chrono::minutes{stoi(D2.at(4))};

    //On récupère le nombre de jours écoulés depuis le 01/01/1970
    int nbjours1 = stoi(to_string(date1.time_since_epoch().count()));
    int nbjours2 = stoi(to_string(date2.time_since_epoch().count()));

    vector<Interaction> listeI;
    vector<Todo> listeT;
    if(nbjours1 < nbjours2){

        // On trie la liste en fonction des dates

        if(typeListe == "Interactions"){
            for(Contact c : ListeContacts){
                for(Interaction I : c.getInteractions()){
                    int dateI = stoi(I.getDate());
                    if(dateI<=nbjours2 && dateI>=nbjours1){
                        listeI.push_back(I);
                    }
                }
                ListeInteractions = listeI;
                MiseAJour("Interactions");
            }
        }

        if(typeListe == "Todos"){
            for(Contact c : ListeContacts){
                for(Interaction I : c.getInteractions()){
                    for(Todo t : I.getTodos()){
                        int datet = stoi(t.getDate());
                        if(datet<=nbjours2 && datet>=nbjours1){
                            listeT.push_back(t);
                        }
                    }
                    ListeTodos = listeT;
                    MiseAJour("Todos");
                }
            }
        }

    }
    else{
        //Envoie Message d'erreur si la première date est supérieure à la deuxième
        QMessageBox::critical(this, "Erreur de dates", "Les dates entrées ne sont pas cohérentes");
    }
}

void ChercherInteraction::PBListe(){
    MiseAJourListes();
}


void ChercherInteraction::fermerQWidget(){
    //Déconnexion des flux avec la QWidget fiche
    disconnect(&fiche,SIGNAL(fermer()),this,SLOT(fermerQWidget()));
    disconnect(this,SIGNAL(EnvoiInteraction(Interaction)),&fiche,SLOT(recupInteraction(Interaction)));
    disconnect(&fiche,SIGNAL(EnvoiInteractionModifiee(Interaction)),this,SLOT(recupInteractionModifiee(Interaction)));
    disconnect(&fiche,SIGNAL(SuppressionInteraction(Interaction)),this,SLOT(SupprimerInteraction(Interaction)));

    this->setEnabled(true);
}

void ChercherInteraction::closeEvent(QCloseEvent *event)
{
    ui->Table->clear();
    ListeContacts.clear();
    ListeInteractions.clear();
    ListeTodos.clear();
    emit fermer("chercherinteraction");


    //Déconnexions des boutons
    disconnect(ui->PBInteractions,SIGNAL(clicked(bool)),this,SLOT(PBInteractions()));
    disconnect(ui->PBTodos,SIGNAL(clicked(bool)),this,SLOT(PBTodos()));
    disconnect(ui->PBFermer,SIGNAL(clicked(bool)),this,SLOT(PBFermer()));
    disconnect(ui->PBLancer,SIGNAL(clicked(bool)),this,SLOT(PBRechercher()));
    disconnect(ui->PBDate,SIGNAL(clicked(bool)),this,SLOT(PBDate()));
    disconnect(ui->PBListe,SIGNAL(clicked(bool)),this,SLOT(PBListe()));
    disconnect(ui->Table,SIGNAL(clicked(QModelIndex)),this,SLOT(CliqueTable(QModelIndex)));

    QWidget::closeEvent(event);
}
