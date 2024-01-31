#include "ficheinteraction.h"
#include "ui_ficheinteraction.h"
#include <QRegularExpressionMatch>
#include <QTextBlock>

FicheInteraction::FicheInteraction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FicheInteraction)
{
    ui->setupUi(this);
}

FicheInteraction::~FicheInteraction()
{
    delete ui;
}

void FicheInteraction::PBModifier_clicked(){
    emit Modification();
    close();
}

void FicheInteraction::PBSupprimer_clicked(){
    emit SuppressionInteraction(interaction);
    close();
}

void FicheInteraction::recupInteraction(Interaction I){
    interaction = I;

    //Connexion des flux des boutons
    connect(ui->PBModifier,SIGNAL(clicked()),this,SLOT(PBModifier_clicked()));
    connect(ui->PBFermer,SIGNAL(clicked()),this,SLOT(PBFermer_clicked()));
    connect(ui->PBSupprimer,SIGNAL(clicked()),this,SLOT(PBSupprimer_clicked()));
    connect(this,SIGNAL(Modification()),this,SLOT(Creation()));

    Affichage();
}

void FicheInteraction::Affichage(){
    vector<string> contenu = interaction.getContenu();
    vector<Todo> ltodos  = interaction.getTodos();

    ui->Texte->clear();

    for(string c : contenu){
        ui->Texte->append(QString::fromStdString(c));
    }

    //Stockage des données des todos dans une liste
    listeTodos = ltodos;

    for(int i=0;i<ltodos.size();i++){
        string info;
        info = "@todo"+std::to_string(i)+" "+ltodos.at(i).getContenu()+" @date "+ltodos.at(i).DateHeureToString();
        ui->Texte->append(QString::fromStdString(info));
    }
}

void FicheInteraction::Creation() {

    QTextDocument *qdoc = ui->Texte->document();
    vector<string> ltodosmodif;
    vector<string> ltodosnew;
    QTextCursor c_contenu(qdoc);
    QTextDocument::FindFlag fcs = (QTextDocument::FindFlag)0;
    vector<string> contenu;
    QRegularExpression qreg_contenu("^(.*)$");

    int nbBlock = qdoc->blockCount();

    for(int i=0;i<nbBlock;i++){
        c_contenu = qdoc->find(qreg_contenu, c_contenu.position(), fcs);
        if (!c_contenu.isNull()) {
            QRegularExpressionMatch match = qreg_contenu.match(c_contenu.block().text());
            if (match.hasMatch()) {
                string contenuText = match.captured(1).toStdString();
                contenu.push_back(contenuText);
            }
            c_contenu.movePosition(QTextCursor::NextBlock);
        }
    }


    for (int i = 0; i < contenu.size(); i++) {
        string c = contenu.at(i);
        QRegularExpression qreg_todo("^(@todo\\d+\\s.*)"); // Modification d'un todo déjà existant
        QRegularExpression newqreg_todo("^@todo\\s(.*)");   //Nouveau Todo

        QRegularExpressionMatch match = qreg_todo.match(QString::fromStdString(c));
        QRegularExpressionMatch match2 = newqreg_todo.match(QString::fromStdString(c));

        if (match.hasMatch()) {
            string todo = match.captured(1).toStdString();
            ltodosmodif.push_back(todo);
            contenu.erase(contenu.begin()+i);
            i--;
        }

        if (match2.hasMatch()) {
            string todo = match2.captured(1).toStdString();
            ltodosnew.push_back(todo);
            contenu.erase(contenu.begin()+i);
            i--;
        }
    }

    string mot = "@todo";
    for (std::string &t : ltodosmodif) {
        size_t pos = 0;
        while ((pos = t.find(mot, pos)) != string::npos) { //Si find ne trouve pas de correspondance, alors il renvoie npos
            // Supprime le mot trouvé, ainsi que l'espace suivant s'il y en a un
            t.erase(pos, mot.length());
        }
    }

    CreationInteraction(ltodosmodif, ltodosnew, contenu);
}

void FicheInteraction::CreationInteraction(vector<string> ltodos, vector<string> ltodosnew, vector<string> contenu){
    string date;
    vector<string> Date;
    vector<Todo> TodosModifies;

    //Créer une Interaction
    Interaction newinteraction(contenu);
    newinteraction.setID(interaction.getID());
    newinteraction.setDate(interaction.getDate());

    /* Dans cette partie de cette fonction, on traite les todos qui ont été
     * modifiés ou supprimés
     * */
    for(string contenutodo : ltodos){
        Date.clear();
        QRegularExpression regex("@date\\s+(\\d{2}/\\d{2}/\\d{4}\\s+[0-2]?[0-9]h[0-5]?[0-9])");

        QRegularExpressionMatch match = regex.match(QString::fromStdString(contenutodo));

        if (match.hasMatch()) {
            date = match.captured(1).toStdString();
            QString qContenutodo = QString::fromStdString(contenutodo);
            qContenutodo.remove(regex);
            contenutodo = qContenutodo.toStdString();


            // On sépare la date et l'heure
            vector<string> DateH;
            stringstream dh(date);
            string partie;

            while(std::getline(dh,partie, ' ')){
                DateH.push_back(partie);
            }

            //Découpe la date en jour mois et année
            stringstream ss(DateH.at(0));

            while (std::getline(ss, partie, '/')) {
                Date.push_back(partie);
            }


            //Découpe l'horaire en heure et minute

            // Récupère l'horaire (censé être le dernier élément de DateH)
            string horaire = DateH.back();
            stringstream hm(horaire);

            while(std::getline(hm,partie,'h')){
                Date.push_back(partie);
            }

            //Dans cette partie de la fonction, les contenu des todos sont
            // de la forme "1 rdv @date 01/01/2012"

            //On récupère le chiffre
            vector<string> mots;

            stringstream ss2(contenutodo);
            string partie2;

            while (std::getline(ss2, partie2, ' ')) {
                mots.push_back(partie2);
            }

            string index = mots[0];

            //Ce chiffre nous permet de répérer indice du todo dans la liste de todos
            int indice = stoi(index);

            QRegularExpression chiffre("^\\d+\\s");
            QString contenu = QString::fromStdString(contenutodo);
            contenu.replace(chiffre, "");
            contenutodo = contenu.toStdString();

            //On crée un nouveau todo modifié puis on lui assigne l'Id que l'on souhaite modifier
            Todo todo(contenutodo, Date.at(0), Date.at(1), Date.at(2), Date.at(3), Date.at(4));
            todo.setID(listeTodos.at(indice).getID());


            TodosModifies.push_back(todo);
        }
        else {

            //Dans cette partie de la fonction, les contenu des todos sont
            // de la forme "1 rdv ..."

            //On récupère le chiffre
            vector<string> mots;

            stringstream ss(contenutodo); //
            string partie;

            while (std::getline(ss, partie, ' ')) {
                mots.push_back(partie);
            }

            string index = mots[0];

            //Ce chiffre nous permet de répérer indice du todo dans la liste de todos
            int indice = stoi(index);

            //On créer un nouveau todo modifié puis on lui assigne l'Id que l'on souhaite modifié
            contenutodo = contenutodo.substr(1);
            Todo todo(contenutodo);
            todo.setID(listeTodos.at(indice).getID());

            TodosModifies.push_back(todo);
        }
    }

    //On vérifie si des todos ont été effacé, ce qui sera considéré comme une suppression
    for (int i = 0; i < listeTodos.size(); i++) {
        bool verif = true;
        for (Todo& tm : TodosModifies) {
            if (listeTodos[i].getID() == tm.getID()) {
                listeTodos[i].setContenu(tm.getContenu());
                listeTodos[i].setDate(tm.getDate());
                verif = false;
                break;
            }
        }

        if (verif) {
            //On ne retrouve pas le todo dans la liste des todos modifiés
            listeTodos[i].setContenu("Null");
        }
    }

    /* Dans cette seconde partie de la fonction, on traite les todos qui ont été
     * ajoutés
     * */
    for(string contenutodo : ltodosnew){
        QRegularExpression regex("@date\\s+(\\d{2}/\\d{2}/\\d{4}\\s+[0-2]?[0-9]h[0-5]?[0-9])");

        QRegularExpressionMatch match = regex.match(QString::fromStdString(contenutodo));

        if (match.hasMatch()) {
            date = match.captured(1).toStdString();
            QString qContenutodo = QString::fromStdString(contenutodo);
            qContenutodo.remove(regex);
            contenutodo = qContenutodo.toStdString();


            // On sépare la date et l'heure
            vector<string> DateH;
            stringstream dh(date);
            string partie;

            while(std::getline(dh,partie, ' ')){
                DateH.push_back(partie);
            }

            //Découpe la date en jour mois et année
            stringstream ss(DateH.at(0));

            while (std::getline(ss, partie, '/')) {
                Date.push_back(partie);
            }


            //Découpe l'horaire en heure et minute

            // Récupère l'horaire (censé être le dernier élément de DateH)
            string horaire = DateH.back();
            stringstream hm(horaire);

            while(std::getline(hm,partie,'h')){
                Date.push_back(partie);
            }

            //On créer un nouveau todo modifié puis on lui assigne l'Id que l'on souhaite modifié
            Todo todo(contenutodo, Date.at(0), Date.at(1), Date.at(2), Date.at(3), Date.at(4));
            todo.setID("Nouveau");
            todo.setIDContact(interaction.getIDContact());
            todo.setIDInteraction(interaction.getID());
            listeTodos.push_back(todo);
        }
        else {

            //On créer un nouveau todo modifié puis on lui assigne l'Id que l'on souhaite modifié
            Todo todo(contenutodo);
            todo.setID("Nouveau");
            todo.setIDContact(interaction.getIDContact());
            todo.setIDInteraction(interaction.getID());
            listeTodos.push_back(todo);
        }
    }


    //Récupérationc de la liste de todos et envoi de la nouvelle Intéraction
    newinteraction.setTodos(listeTodos);
    newinteraction.setIDContact(interaction.getIDContact());
    emit EnvoiInteractionModifiee(newinteraction);
    close();
}

void FicheInteraction::PBFermer_clicked(){
    close();
}

void FicheInteraction::closeEvent(QCloseEvent *event)
{
    emit fermer();
    ui->Texte->clear();

    //Déconnexion des flux des boutons
    disconnect(ui->PBModifier,SIGNAL(clicked()),this,SLOT(PBModifier_clicked()));
    disconnect(ui->PBFermer,SIGNAL(clicked()),this,SLOT(PBFermer_clicked()));
    disconnect(ui->PBSupprimer,SIGNAL(clicked()),this,SLOT(PBSupprimer_clicked()));
    disconnect(this,SIGNAL(Modification()),this,SLOT(Creation()));

    QWidget::closeEvent(event);
}
