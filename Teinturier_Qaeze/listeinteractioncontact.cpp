#include "listeinteractioncontact.h"
#include "ui_listeinteractioncontact.h"
#include <iostream>
#include <QStringListModel>

using namespace std;

ListeInteractionContact::ListeInteractionContact(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListeInteractionContact)
{
    ui->setupUi(this);

    //Connexion des flux des boutons
    connect(ui->PBFermer,SIGNAL(clicked()),this, SLOT(PBFermer_clicked()));
    connect(ui->Liste,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(selectInteraction(QModelIndex)));

    //Connexion du flux pour la mise à jour de la liste
    connect(this,SIGNAL(MiseAJourListe(vector<Interaction>)),this,SLOT(initInteraction(vector<Interaction>)));
}

ListeInteractionContact::~ListeInteractionContact()
{
    delete ui;
}

void ListeInteractionContact::initInteraction(vector<Interaction> l) {
    liste = l;
    QStringList listeItem;

    for (Interaction I : liste) {
        std::string item = "Interaction : " + I.getID() + " " + I.DateHeureToString();
        if (I.getTodos().size() == 0)
            item += " Ne possède pas de todo !";
        else {
            std::string nbTodos = std::to_string(I.getTodos().size());
            item += " Possède " + nbTodos + " todo !";
        }

        listeItem.append(QString::fromStdString(item));
    }

    QStringListModel *model = new QStringListModel();
    model->setStringList(listeItem);

    ui->Liste->setModel(model);
}


void ListeInteractionContact::selectInteraction(QModelIndex index){
    // Obtention du modèle associé à la QListView
    QAbstractItemModel *model = ui->Liste->model();

    Interaction interaction = liste.at(0);

    // Vérification si l'index est valide et si le modèle existe
    if (model && index.isValid()) {
        // Récupération des données à partir de l'index sélectionné
        QVariant donnee = model->data(index, Qt::DisplayRole);
        if (donnee.isValid()) {
            QString donnees = donnee.toString();
            QStringList info = donnees.split(" ");
            QString ID = info.at(2); // On récupère l'id de l'intéraction.

            for(Interaction I : liste){
                if(I.getID() == ID.toStdString())
                    interaction = I;  // On récupère l'intéraction
            }
        }
    }

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

void ListeInteractionContact::SupprimerInteraction(Interaction I){
    emit SuppressionInteraction(I);
}

void ListeInteractionContact::recupInteractionModifiee(Interaction I){
    emit EnvoiInteractionModifiee(I);
}

void ListeInteractionContact::PBFermer_clicked(){
    close();
}

void ListeInteractionContact::fermerQWidget(){

    //Connexion des flux avec la QWidget fiche
    disconnect(&fiche,SIGNAL(fermer()),this,SLOT(fermerQWidget()));
    disconnect(this,SIGNAL(EnvoiInteraction(Interaction)),&fiche,SLOT(recupInteraction(Interaction)));
    disconnect(&fiche,SIGNAL(EnvoiInteractionModifiee(Interaction)),this,SLOT(recupInteractionModifiee(Interaction)));
    disconnect(&fiche,SIGNAL(SuppressionInteraction(Interaction)),this,SLOT(SupprimerInteraction(Interaction)));

    this->setEnabled(true);
}

void ListeInteractionContact::closeEvent(QCloseEvent *event)
{
    emit fermer("listeinteractioncontact");
    QWidget::closeEvent(event);
}
