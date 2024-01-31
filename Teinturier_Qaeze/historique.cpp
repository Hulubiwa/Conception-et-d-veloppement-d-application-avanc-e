#include "historique.h"
#include "ui_historique.h"

Historique::Historique(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Historique)
{
    ui->setupUi(this);
}

Historique::~Historique()
{
    delete ui;
}

void Historique::initTable(){
    ui->Table->setColumnCount(3);
    ui->Table->setHorizontalHeaderLabels({"Contact ID","Contenu","Date"});

    //Supprime toutes les lignes de la table (Réinitialise la table)
    for (int i=0; i<ui->Table->rowCount();i++){
        ui->Table->removeRow(i);
        i--;
    }

    for(int i=0; i<Liste.size();i++){
        Modification m = Liste.at(i);

        int ligne = ui->Table->rowCount();
        ui->Table->insertRow(ligne);

        QString Contenu = QString::fromStdString(m.getContenu());
        QString ID = QString::fromStdString(m.getContactID());
        QString Date = QString::fromStdString(m.DateHeureToString());

        QTableWidgetItem* itemID = new QTableWidgetItem(ID);
        ui->Table->setItem(ligne,0,itemID);

        QTableWidgetItem* itemContenu = new QTableWidgetItem(Contenu);
        ui->Table->setItem(ligne,1,itemContenu);

        QTableWidgetItem* itemDate = new QTableWidgetItem(Date);
        ui->Table->setItem(ligne,2,itemDate);
    }

    ui->Table->setColumnWidth(0,150);
    ui->Table->setColumnWidth(1,150);
    ui->Table->setColumnWidth(2,150);
}

void Historique::ListModifications(vector<Modification> listemodifications){
    Liste = listemodifications;
    initTable();
}

void Historique::closeEvent(QCloseEvent *event)
{
    emit fermer("Historique");
    QWidget::closeEvent(event);
}
