#include "GestionBD.h"
#include <QtSql/QSqlError>
#include <QDir>
#include <QFile>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <random>
#include <iostream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

using namespace std;

GestionBD::GestionBD(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    creationBD();
    connexionBD();
}

GestionBD::~GestionBD(){}

void GestionBD::deconnexionBD(){
    db.close();
}

void GestionBD::connexionBD(){

    db.setDatabaseName("../BaseDeDonnees/BD.sqlite");

    if (!db.open())
    {
        qDebug() << "Pas de connexion BDD !";
    }
    else
    {
        qDebug() << "Connexion BDD ok";
    }
}

void GestionBD::creationBD(){

    // Spécifiez l'emplacement et le nom du fichier de base de données
    QString nomFichierBase = "../BaseDeDonnees/BD.sqlite";

    // Vérifiez si le fichier de base de données existe déjà
    if (QFile::exists(nomFichierBase))
    {
        qDebug() << "La base de données existe déjà.";
    }
    else
    {
        // Configurez la base de données et créez le fichier de base de données
        db.setDatabaseName(nomFichierBase);

        if (db.open())
        {
            // Vous pouvez exécuter d'autres opérations sur la base de données ici
            qDebug() << "Base de données créée avec succès.";
            QSqlQuery query, query1, query2, query3,query4;
            query.prepare("CREATE TABLE Contact(NumeroID TEXT PRIMARY KEY, Nom TEXT, Prenom TEXT, Entreprise TEXT, Mail TEXT, Telephone TEXT, Photo TEXT, Date TEXT)");

            if(!query.exec()){
                qDebug() << "Impossible d'exécuter la requête !";
                qDebug() << "Erreur : " << query.lastError().text();
            }

            query1.prepare("CREATE TABLE Interactions(InteractionID TEXT PRIMARY KEY, ContenuInteraction TEXT, DateInteraction TEXT)");


            if(!query1.exec()){
                qDebug() << "Impossible d'exécuter la requête !";
                qDebug() << "Erreur : " << query.lastError().text();
            }

            query2.prepare("CREATE TABLE Todos(TodoID TEXT PRIMARY KEY, ContenuTodo TEXT, DateTodo TEXT)");

            if(!query2.exec()){
                qDebug() << "Impossible d'exécuter la requête !";
                qDebug() << "Erreur : " << query.lastError().text();
            }

            query3.prepare("CREATE TABLE AssociationTodoContactInteraction(NumeroID TEXT, InteractionID TEXT, TodoID TEXT,PRIMARY KEY (TodoID, InteractionID, NumeroID), FOREIGN KEY(NumeroID) REFERENCES Contact(NumeroID), FOREIGN KEY(InteractionID) REFERENCES Interactions(InteractionID), FOREIGN KEY(TodoID) REFERENCES Todos(TodoID))");

            if(!query3.exec()){
                qDebug() << "Impossible d'exécuter la requête !";
                qDebug() << "Erreur : " << query.lastError().text();
            }

            query4.prepare("CREATE TABLE HistoriqueModifications(ContenuModification TEXT, DateModification TEXT, NumeroID TEXT, PRIMARY KEY (ContenuModification , DateModification , NumeroID), FOREIGN KEY(NumeroID) REFERENCES Contact(NumeroID))");

            if(!query4.exec()){
                qDebug() << "Impossible d'exécuter la requête !";
                qDebug() << "Erreur : " << query.lastError().text();
            }
            // Fermez la base de données
            db.close();
        }
        else
        {
            qDebug() << "Erreur lors de la création de la base de données : " << db.lastError().text();
        }
    }
}

void GestionBD::ajoutContact(Contact c){

    QSqlQuery query;

    string ID = genererID("Contact");

    query.prepare("INSERT INTO Contact (NumeroID, Nom, Prenom, Entreprise, Mail, Telephone, Photo, Date) VALUES (:ni, :n, :p, :e, :m, :t, :ph, :d)");
    query.bindValue(":ni", QString::fromStdString(ID));
    query.bindValue(":n", QString::fromStdString(c.getNom()));
    query.bindValue(":p", QString::fromStdString(c.getPrenom()));
    query.bindValue(":e", QString::fromStdString(c.getEntreprise()));
    query.bindValue(":m", QString::fromStdString(c.getMail()));
    query.bindValue(":t", QString::fromStdString(c.getTelephone()));
    query.bindValue(":ph", QString::fromStdString(c.getPhoto()));
    query.bindValue(":d", QString::fromStdString(c.getDate()));

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête ajoutContact !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Ajout du Contact réussi !";
    }
}

void GestionBD::modificationContact(Contact c){


    QSqlQuery query;

    query.prepare("UPDATE Contact SET Nom = :n , Prenom = :p , Entreprise = :e , Mail = :m , Telephone = :t , Photo = :ph WHERE NumeroID = :ni");
    query.bindValue(":ni", QString::fromStdString(c.getID()));
    query.bindValue(":n", QString::fromStdString(c.getNom()));
    query.bindValue(":p", QString::fromStdString(c.getPrenom()));
    query.bindValue(":e", QString::fromStdString(c.getEntreprise()));
    query.bindValue(":m", QString::fromStdString(c.getMail()));
    query.bindValue(":t", QString::fromStdString(c.getTelephone()));
    query.bindValue(":ph", QString::fromStdString(c.getPhoto()));

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête modificationContatc !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Modification du Contact réussi !";

        for(Interaction I : c.getInteractions()){
            modificationInteraction(I);
            for(Todo t : I.getTodos()){
                if(t.getID() == "Nouveau"){
                    string tID = genererID("Todo");
                    t.setID(tID);
                    ajoutTodo(t);
                    ajoutAssociation(c.getID(), I.getID(),tID);
                }
                else if(t.getContenu() == "Null"){
                    SupprimerAssociation(c.getID(), I.getID(),t.getID());
                    SupprimerTodo(t);
                }
                else{
                    modificationTodo(t);
                }
            }
        }

        emit MiseAJour();
    }
}

void GestionBD::SupprimerAssociation(string contactid, string interactionid, string todoid){
    QSqlQuery query;

    query.prepare("DELETE FROM AssociationTodoContactInteraction WHERE NumeroID = :ni AND InteractionID = :ii AND TodoID = :ti");
    query.bindValue(":ni", QString::fromStdString(contactid));
    query.bindValue(":ii", QString::fromStdString(interactionid));
    query.bindValue(":ti", QString::fromStdString(todoid));

    if (!query.exec()) {
        qDebug() << "Impossible d'exécuter la requête suppressionContact !";
        qDebug() << "Erreur : " << query.lastError().text();
    } else {
        qDebug() << "Suppression de l'Association réussie !";
    }
}

void GestionBD::SupprimerTodo(Todo todo){
    QSqlQuery query;

    query.prepare("DELETE FROM Todos WHERE TodoID = :ni ");
    query.bindValue(":ni", QString::fromStdString(todo.getID()));

    if (!query.exec()) {
        qDebug() << "Impossible d'exécuter la requête suppressionContact !";
        qDebug() << "Erreur : " << query.lastError().text();
    } else {
        qDebug() << "Suppression du Todo réussi !";
    }
}

void GestionBD::modificationInteraction(Interaction I){

    QSqlQuery query;

    vector <string> lcontenu = I.getContenu();
    string contenu;
    for(string c : lcontenu){
        contenu+= c+"\n";
    }

    query.prepare("UPDATE Interactions SET ContenuInteraction = :c , DateInteraction = :d WHERE InteractionID = :ii");
    query.bindValue(":ii", QString::fromStdString(I.getID()));
    query.bindValue(":c", QString::fromStdString(contenu));
    query.bindValue(":d", QString::fromStdString(I.getDate()));

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête modificationInteraction !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Modification de l'Intéraction réussi !";
    }


}

void GestionBD::modificationTodo(Todo todo){
    QSqlQuery query;

    query.prepare("UPDATE Todos SET ContenuTodo = :c , DateTodo = :d WHERE TodoID = :ii");
    query.bindValue(":ii", QString::fromStdString(todo.getID()));
    query.bindValue(":c", QString::fromStdString(todo.getContenu()));
    query.bindValue(":d", QString::fromStdString(todo.getDate()));

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête modificationTodo !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Modification du Todo réussi !";
    }
}

void GestionBD::ajoutInteractionTodo(Interaction I, string contactId){

    string InteractionId = genererID("Interaction");
    I.setID(InteractionId);
    vector<Todo> ltodos = I.getTodos();
    ajoutInteraction(I);

    for(Todo t : ltodos){
        string tID = genererID("Todo");
        t.setID(tID);
        ajoutTodo(t);
        ajoutAssociation(contactId, InteractionId,tID);
    }

    //On ajout un lien entre le contact et l'intéraction
    ajoutAssociation(contactId, InteractionId, "Null");
    emit MiseAJour();

}

void GestionBD::ajoutAssociation(string ContactID, string InteractionID, string TodoID){
    QSqlQuery query;

    query.prepare("INSERT INTO AssociationTodoContactInteraction(NumeroID, InteractionID, TodoID) VALUES (:ci, :ii, :ti)");
    query.bindValue(":ci", QString::fromStdString(ContactID));
    query.bindValue(":ii", QString::fromStdString(InteractionID));
    query.bindValue(":ti", QString::fromStdString(TodoID));
    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête ajoutAssociation !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Ajout de l'Association entre les Ids réussi !";
    }
}

void GestionBD::EnregistrerModifiaction(string type, string Id){
    Modification modification(type,Id);
    ajoutModification(modification);
    emit MiseAJour();
}

void GestionBD::ajoutInteraction(Interaction I){
    QSqlQuery query;

    vector<string> lcontenu = I.getContenu();
    string contenu;
    for(string c : lcontenu){
        contenu+= c+"\n";
    }

    query.prepare("INSERT INTO Interactions (InteractionID, ContenuInteraction, DateInteraction) VALUES (:ni, :c, :d)");
    query.bindValue(":ni", QString::fromStdString(I.getID()));
    query.bindValue(":c", QString::fromStdString(contenu));
    query.bindValue(":d", QString::fromStdString(I.getDate()));
    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête ajoutInteraction !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Ajout de l'Interaction réussi !";
    }
}

void GestionBD::ajoutTodo(Todo t){

    QSqlQuery query;

    query.prepare("INSERT INTO Todos (TodoID, ContenuTodo, DateTodo) VALUES (:ni, :c, :d)");
    query.bindValue(":ni", QString::fromStdString(t.getID()));
    query.bindValue(":c", QString::fromStdString(t.getContenu()));
    query.bindValue(":d", QString::fromStdString(t.getDate()));
    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête ajoutTodo !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Ajout du Todo réussi !";
    }
}

void GestionBD::suppressionContact(Contact c) {
    QSqlQuery query;

    query.prepare("DELETE FROM Contact WHERE NumeroID = :ni ");
    query.bindValue(":ni", QString::fromStdString(c.getID()));

    if (!query.exec()) {
        qDebug() << "Impossible d'exécuter la requête suppressionContact !";
        qDebug() << "Erreur : " << query.lastError().text();
    } else {
        qDebug() << "Suppression du Contact réussi !";
        for(Interaction I : c.getInteractions()){
            SupprimerInteraction(I);
        }
        emit MiseAJour();
    }
}

void GestionBD::SupprimerInteraction(Interaction I){
    QSqlQuery query;

    query.prepare("DELETE FROM Interactions WHERE InteractionID = :ii ");
    query.bindValue(":ii", QString::fromStdString(I.getID()));

    if (!query.exec()) {
        qDebug() << "Impossible d'exécuter la requête SupprimerInteraction !";
        qDebug() << "Erreur : " << query.lastError().text();
    } else {
        qDebug() << "Suppression de l'interaction réussi !";

        vector<Todo> ltodos = I.getTodos();

        for(Todo t : ltodos){
            SupprimerTodo(t);
        }

        //On supprime les liens
        QSqlQuery query1;

        query1.prepare("DELETE FROM AssociationTodoContactInteraction WHERE InteractionID = :ii");
        query1.bindValue(":ii", QString::fromStdString(I.getID()));
        if (!query1.exec()) {
            qDebug() << "Impossible d'exécuter la requête SupprimerInteraction Association !";
            qDebug() << "Erreur : " << query1.lastError().text();
        } else {
            qDebug() << "Suppression de l'Association réussie !";
        }
    }
}

void GestionBD::ajoutModification(Modification m){
    QSqlQuery query;

    query.prepare("INSERT INTO HistoriqueModifications(ContenuModification, DateModification, NumeroID) VALUES ( :c , :d , :i)");
    query.bindValue(":c", QString::fromStdString(m.getContenu()));
    query.bindValue(":d", QString::fromStdString(m.getDate()));
    query.bindValue(":i", QString::fromStdString(m.getContactID()));

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête ajoutModification !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Ajout de la modification à l'historique réussi !";
    }
}

vector<Contact> GestionBD::getContacts(){
    vector<Contact> contactList;

    QSqlQuery query;
    query.prepare("SELECT * FROM Contact");

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête getContacts !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Récupération de la Liste des contacts réussie !";

        // Parcourir les résultats de la requête
        while (query.next()) {
            string numeroID = query.value("NumeroID").toString().toStdString();
            string nom = query.value("Nom").toString().toStdString();
            string prenom = query.value("Prenom").toString().toStdString();
            string entreprise = query.value("Entreprise").toString().toStdString();
            string mail = query.value("Mail").toString().toStdString();
            string telephone = query.value("Telephone").toString().toStdString();
            string photo = query.value("Photo").toString().toStdString();
            string date = query.value("Date").toString().toStdString();

            // Créer un objet Contact et l'ajouter à la liste
            Contact contact(numeroID, nom, prenom, entreprise, mail, telephone, photo, date);

            //On récupère la liste des modifications du contact
            vector<Modification> liste = getModificationsOfContact(contact);
            vector<Interaction> interactions = getInteractionsOfContact(contact);
            contact.setModifications(liste);
            contact.setInteractions(interactions);

            contactList.push_back(contact);
        }
    }

    return contactList;
}

vector<Modification> GestionBD::getModifications(){
    vector<Modification> Modifications;

    QSqlQuery query;
    query.prepare("SELECT * FROM HistoriqueModifications");

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête getModifications !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Récupération de la Liste des Modifications réussie !";

        // Parcourir les résultats de la requête
        while (query.next()) {

            string contenu = query.value("ContenuModification").toString().toStdString();
            string date = query.value("DateModification").toString().toStdString();
            string contactID = query.value("NumeroID").toString().toStdString();

            std::istringstream iss(contenu);
            std::string type;
            iss >> type;


            Modification Modif(type,contactID);
            Modif.setDate(date);
            Modifications.push_back(Modif);
        }
    }

    return Modifications;
}

vector<Interaction> GestionBD::getInteractions(){
    vector<Interaction> interactions;

    QSqlQuery query;
    query.prepare("SELECT * FROM Interactions");

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête getInteractions !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Récupération de la Liste des Interactions réussie !";

        // Parcourir les résultats de la requête
        while (query.next()) {

            string Id = query.value("InteractionID").toString().toStdString();
            string Contenu = query.value("ContenuInteraction").toString().toStdString();
            string Date = query.value("DateInteraction").toString().toStdString();

            vector<string> listeContenu;

            stringstream ss(Contenu); //
            string partie;

            char s = '\n';

            while (std::getline(ss, partie, s)) {
                string mot = "\n";
                size_t pos = 0;
                while ((pos = partie.find(mot, pos)) != string::npos) { //Si find ne trouve pas de correspondance, alors il renvoie npos
                    // Supprime le mot trouvé, ainsi que l'espace suivant s'il y en a un
                    partie.erase(pos, mot.length());
                }

                listeContenu.push_back(partie);
            }


            Interaction interaction(listeContenu);
            interaction.setDate(Date);
            interaction.setID(Id);

            interactions.push_back(interaction);
        }
    }

    return interactions;
}

vector<Interaction> GestionBD::getInteractionsOfContact(Contact c){

    vector<Interaction> interactions;

    QSqlQuery query;

    query.prepare("SELECT DISTINCT Interactions.InteractionID, Interactions.ContenuInteraction, Interactions.DateInteraction "
                  "FROM Interactions "
                  "JOIN AssociationTodoContactInteraction ON Interactions.InteractionID = AssociationTodoContactInteraction.InteractionID "
                  "WHERE AssociationTodoContactInteraction.NumeroID = :ni");
    query.bindValue(":ni",QString::fromStdString(c.getID()));

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête getInteractionsOfContact !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Récupération de la Liste des Intéractions réussie !";

        while(query.next()){
            string Id = query.value("InteractionID").toString().toStdString();
            string Contenu = query.value("ContenuInteraction").toString().toStdString();
            string Date = query.value("DateInteraction").toString().toStdString();

            vector<string> listeContenu;

            stringstream ss(Contenu); //
            string partie;

            char s = '\n';

            while (std::getline(ss, partie, s)) {
                string mot = "\n";
                    size_t pos = 0;
                while ((pos = partie.find(mot, pos)) != string::npos) { //Si find ne trouve pas de correspondance, alors il renvoie npos
                    // Supprime le mot trouvé, ainsi que l'espace suivant s'il y en a un
                    partie.erase(pos, mot.length());
                }

                listeContenu.push_back(partie);
            }

            for (std::string& partie : listeContenu) {
                char delimiter = '\n';

                size_t pos = partie.find(delimiter);
                while (pos != std::string::npos) {
                    partie.erase(pos, 1); // Supprime le caractère à la position 'pos'
                    pos = partie.find(delimiter, pos); // Recherche le prochain caractère '\n'
                }
            }

            Interaction interaction(listeContenu);
            interaction.setDate(Date);
            interaction.setID(Id);
            interaction.setIDContact(c.getID());

            vector<Todo> ltodos = getTodosOfInteraction(Id, c.getID());
            interaction.setTodos(ltodos);

            interactions.push_back(interaction);
        }
    }

    return interactions;
}

vector<Todo> GestionBD::getTodosOfInteraction(string InteractionID, string ContactID){

    QSqlQuery query;

    vector<Todo> ltodos;

    query.prepare("SELECT Todos.TodoID, Todos.ContenuTodo, Todos.DateTodo "
                "FROM Todos "
                "JOIN AssociationTodoContactInteraction ON Todos.TodoID = AssociationTodoContactInteraction.TodoID "
                "WHERE AssociationTodoContactInteraction.InteractionID = :ii");
    query.bindValue(":ii",QString::fromStdString(InteractionID));

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête getTodosOfInteraction !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Récupération de la Liste des Todos réussie !";

        while(query.next()){


            string Id = query.value("TodoID").toString().toStdString();
            string Contenu = query.value("ContenuTodo").toString().toStdString();
            string Date = query.value("DateTodo").toString().toStdString();

            Todo todo(Contenu);
            todo.setDate(Date);
            todo.setID(Id);
            todo.setIDContact(ContactID);
            todo.setIDInteraction(InteractionID);

            if(Id != "Null"){
                ltodos.push_back(todo);
            }

        }
    }

    return ltodos;
}

bool GestionBD::VerificationID(string ID, vector<string> listeIDs){
    for(string id : listeIDs){
        if(id == ID){
            return true;
        }
    }
    return false;

}

string GestionBD::genererID(string type){

    vector<string> listeIds;

    if(type == "Contact"){
        listeIds = getIDsContact();
    }
    else if (type == "Interaction"){
        listeIds = getIDsInteraction();
    }
    else{
        listeIds = getIDsTodo();
    }

    // Créer un moteur aléatoire et une distribution uniforme
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(100000, 999999);

    // Générer un nombre aléatoire entre 100.000 et 999.999
    int nombreAleatoire = distribution(gen);
    std::ostringstream oss;
    oss << std::setw(6) << std::setfill('0') << nombreAleatoire;
    string newID = oss.str();

    //Vérification de l'unicité de l'ID
    while(VerificationID(newID, listeIds)){
        nombreAleatoire = distribution(gen);
        oss << std::setw(6) << std::setfill('0') << nombreAleatoire;
        newID = oss.str();
    }

    return newID;

}

vector<string> GestionBD::getIDsContact(){
    vector<string> IDsliste;

    QSqlQuery query;
    query.prepare("SELECT NumeroID FROM Contact");

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête getIDsContact !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Récupération de la Liste des IDs réussie !";

        // Parcourir les résultats de la requête
        while (query.next()) {
            string numeroID = query.value("NumeroID").toString().toStdString();
            IDsliste.push_back(numeroID);
        }
    }
    return IDsliste;
}

vector<string> GestionBD::getIDsInteraction(){
    vector<string> IDsliste;

    QSqlQuery query;
    query.prepare("SELECT InteractionID FROM Interactions");

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête getIDsInteraction !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Récupération de la Liste des IDs réussie !";

        // Parcourir les résultats de la requête
        while (query.next()) {
            string numeroID = query.value("InteractionID").toString().toStdString();
            IDsliste.push_back(numeroID);
        }
    }
    return IDsliste;
}

vector<string> GestionBD::getIDsTodo(){
    vector<string> IDsliste;

    QSqlQuery query;
    query.prepare("SELECT TodoID FROM Todos");

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête getIDsTodo !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Récupération de la Liste des IDs réussie !";

        // Parcourir les résultats de la requête
        while (query.next()) {
            string numeroID = query.value("TodoID").toString().toStdString();
            IDsliste.push_back(numeroID);
        }
    }
    return IDsliste;
}

vector<Modification> GestionBD::getModificationsOfContact(Contact c){
    vector<Modification> ModifListe;


    QSqlQuery query;
    query.prepare("SELECT * FROM HistoriqueModifications WHERE NumeroID = " + QString::fromStdString(c.getID()));


    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête getModificationsOfContact !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else
    {
        qDebug() << "Recuperation de la Liste des Modifications de " << QString::fromStdString(c.getNom()) << " " << QString::fromStdString(c.getPrenom()) <<" reussie !";

        // Parcourir les résultats de la requête
        while (query.next()) {
            string contenu = query.value("ContenuModification").toString().toStdString();
            string date = query.value("DateModification").toString().toStdString();
            string contactID = query.value("NumeroID").toString().toStdString();

            std::istringstream iss(contenu);
            std::string type;
            iss >> type;


            Modification Modif(type,contactID);
            ModifListe.push_back(Modif);
        }
    }
    return ModifListe;
}

void GestionBD::MiseAJourListeContacts(){
    vector<Contact> ListeC = getContacts();
    vector<Modification> ListeM = getModifications();

    emit EnvoiListes(ListeC, ListeM);
}

void GestionBD::exportJSON(){
    QSqlQuery query, query1, query2, query3,query4;

    query.prepare("SELECT * FROM Contact");

    if (!query.exec())
    {
        qDebug() << "Impossible d'exécuter la requête exportJSON pour la Table Contact !!";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else{
        QJsonArray jsonArray;

        while (query.next()) {
            QJsonObject jsonObject;
            jsonObject["NumeroID"] = query.value("NumeroID").toString();
            jsonObject["Nom"] = query.value("Nom").toString();
            jsonObject["Prenom"] = query.value("Prenom").toString();
            jsonObject["Entreprise"] = query.value("Entreprise").toString();
            jsonObject["Mail"] = query.value("Mail").toString();
            jsonObject["Telephone"] = query.value("Telephone").toString();
            jsonObject["Photo"] = query.value("Photo").toString();
            jsonObject["Date"] = query.value("Date").toString();

            jsonArray.append(jsonObject);
        }

        QJsonDocument jsonDocument(jsonArray);

        QFile file("../BaseDeDonnees/Contact.json");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(jsonDocument.toJson());
            qDebug()<< "Export en JSON réussi pour la Table Contact !";
            file.close();
        }
    }


    query1.prepare("SELECT * FROM Interactions");

    if (!query1.exec())
    {
        qDebug() << "Impossible d'exécuter la requête exportJSON pour la Table Interactions !!";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else{
        QJsonArray jsonArray;

        while (query1.next()) {
            QJsonObject jsonObject;
            jsonObject["InteractionID"] = query1.value("InteractionID").toString();
            jsonObject["ContenuInteraction"] = query1.value("ContenuInteraction").toString();
            jsonObject["DateInteraction"] = query1.value("DateInteraction").toString();

            jsonArray.append(jsonObject);
        }

        QJsonDocument jsonDocument(jsonArray);

        QFile file("../BaseDeDonnees/Interactions.json");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(jsonDocument.toJson());
            qDebug()<< "Export en JSON réussi pour la Table Interactions !";
            file.close();
        }
    }

    query2.prepare("SELECT * FROM Todos");

    if (!query2.exec())
    {
        qDebug() << "Impossible d'exécuter la requête exportJSON pour la Table Todos !!";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else{
        QJsonArray jsonArray;

        while (query2.next()) {
            QJsonObject jsonObject;
            jsonObject["TodoID"] = query2.value("TodoID").toString();
            jsonObject["ContenuTodo"] = query2.value("ContenuTodo").toString();
            jsonObject["DateTodo"] = query2.value("DateTodo").toString();

            jsonArray.append(jsonObject);
        }

        QJsonDocument jsonDocument(jsonArray);

        QFile file("../BaseDeDonnees/Todos.json");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(jsonDocument.toJson());
            qDebug()<< "Export en JSON réussi pour la Table Todos !";
            file.close();
        }
    }

    query3.prepare("SELECT * FROM AssociationTodoContactInteraction");

    if (!query3.exec())
    {
        qDebug() << "Impossible d'exécuter la requête exportJSON pour la Table AssociationTodoContactInteraction !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else{
        QJsonArray jsonArray;

        while (query3.next()) {
            QJsonObject jsonObject;
            jsonObject["NumeroID"] = query3.value("NumeroID").toString();
            jsonObject["InteractionID"] = query3.value("InteractionID").toString();
            jsonObject["TodoID"] = query3.value("TodoID").toString();

            jsonArray.append(jsonObject);
        }

        QJsonDocument jsonDocument(jsonArray);

        QFile file("../BaseDeDonnees/AssociationTodoContactInteraction.json");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(jsonDocument.toJson());
            qDebug()<< "Export en JSON réussi pour la Table AssociationTodoContactInteraction !";
            file.close();
        }
    }

    query4.prepare("SELECT * FROM HistoriqueModifications");

    if (!query4.exec())
    {
        qDebug() << "Impossible d'exécuter la requête exportJSON pour la Table HistoriqueModifications !";
        qDebug() << "Erreur : " << query.lastError().text();
    }
    else{
        QJsonArray jsonArray;

        while (query4.next()) {
            QJsonObject jsonObject;
            jsonObject["NumeroID"] = query4.value("NumeroID").toString();
            jsonObject["ContenuModification"] = query4.value("ContenuModification").toString();
            jsonObject["DateModification"] = query4.value("DateModification").toString();

            jsonArray.append(jsonObject);
        }

        QJsonDocument jsonDocument(jsonArray);

        QFile file("../BaseDeDonnees/HistoriqueModifications.json");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(jsonDocument.toJson());
            qDebug()<< "Export en JSON réussi pour la Table HistoriqueModifications !";
            file.close();
        }
    }
}
