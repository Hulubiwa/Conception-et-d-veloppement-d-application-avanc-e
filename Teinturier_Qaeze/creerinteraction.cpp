#include "creerinteraction.h"
#include "ui_creerinteraction.h"
#include<QTextBlock>
#include <QRegularExpression>
#include <iostream>
#include <string>

using namespace std;

CreerInteraction::CreerInteraction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreerInteraction)
{
    ui->setupUi(this);

    connect(ui->PBValider,SIGNAL(clicked()),this,SLOT(PBValider_clicked()));
    connect(ui->PBAnnuler,SIGNAL(clicked()),this,SLOT(Annuler()));
    connect(this,SIGNAL(creerInteraction()),this,SLOT(Creation()));
}

CreerInteraction::~CreerInteraction()
{
    delete ui;
}

void CreerInteraction::PBValider_clicked(){
    emit creerInteraction();
    close();
}

void CreerInteraction::Creation() {
    QTextDocument *qdoc = ui->Texte->document();
    QTextCursor c_contenu(qdoc);
    QTextDocument::FindFlag fcs = (QTextDocument::FindFlag)0;
    vector<string> ltodos;
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


    for (int i = 0; i < contenu.size(); ++i) {
        string c = contenu.at(i);
        QRegularExpression qreg_todo("^@todo (.*)");

        QRegularExpressionMatch match = qreg_todo.match(QString::fromStdString(c));

        if (match.hasMatch()) {
            string todo = match.captured(1).toStdString();
            ltodos.push_back(todo);
            contenu.erase(contenu.begin()+i);
            i--;
        }
    }

    ui->Texte->clear();
    std::string mot = "@todo ";
    for (std::string &t : ltodos) {
        size_t pos = 0;
        while ((pos = t.find(mot, pos)) != string::npos) { //Si find ne trouve pas de correspondance, alors il renvoie npos
            // Supprime le mot trouvé, ainsi que l'espace suivant s'il y en a un
            t.erase(pos, mot.length());
        }
    }

    CreationInteraction(ltodos, contenu);
}

void CreerInteraction::CreationInteraction(vector<string> ltodos, vector<string> contenu){
    string date;
    vector<string> Date;

    //Créer une Interaction
    Interaction interaction(contenu);


    //On parcours la liste de todos
    for(string myString : ltodos){
        QRegularExpression regex("@date\\s+(\\d{2}/\\d{2}/\\d{4}\\s+[0-2]?[0-9]h[0-5]?[0-9])");


        QRegularExpressionMatch match = regex.match(QString::fromStdString(myString));

        if (match.hasMatch()) {
            date = match.captured(1).toStdString();
            QString qmyString = QString::fromStdString(myString);
            qmyString.remove(regex);
            myString = qmyString.toStdString();

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

            //On créé un todo et on l'ajoute à l'intéraction
            Todo todo(myString, Date.at(0), Date.at(1), Date.at(2), Date.at(3), Date.at(4));
            interaction.ajoutTodo(todo);


        }
        else {
            Todo todo(myString);
            interaction.ajoutTodo(todo);
        }
    }
    emit EnvoiInteraction(interaction);

}

void CreerInteraction::Annuler(){
    close();
}

void CreerInteraction::closeEvent(QCloseEvent *event)
{
    emit fermer("creerinteraction");
    QWidget::closeEvent(event);
}
