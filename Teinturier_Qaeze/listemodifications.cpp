#include "listemodifications.h"
#include "ui_listemodifications.h"

ListeModifications::ListeModifications(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListeModifications)
{
    ui->setupUi(this);
    connect(ui->PBFermer, SIGNAL(clicked()),this, SLOT(Fermer()));
}

ListeModifications::~ListeModifications()
{
    delete ui;
}

void ListeModifications::recupListeModifications(vector<Modification> liste){
    MiseAJourTable(liste);
}

void ListeModifications::Fermer()
{
    close();
}

void ListeModifications::MiseAJourTable(vector<Modification> liste){

    ui->Table->setColumnCount(3);
    ui->Table->setHorizontalHeaderLabels({"Contenu","Date","ID Contact"});

    for (int i=0; i<ui->Table->rowCount();i++){
        ui->Table->removeRow(i);
        i--;
    }

    for(int i=0; i<liste.size();i++){
        Modification c = liste.at(i);

        int ligne = ui->Table->rowCount();
        ui->Table->insertRow(ligne);

        QString Contenu = QString::fromStdString(c.getContenu());
        QString Date = QString::fromStdString(c.DateHeureToString());
        QString ID = QString::fromStdString(c.getContactID());

        QTableWidgetItem* itemNom = new QTableWidgetItem(Contenu);
        ui->Table->setItem(ligne,0,itemNom);

        QTableWidgetItem* itemPrenom = new QTableWidgetItem(Date);
        ui->Table->setItem(ligne,1,itemPrenom);

        QTableWidgetItem* itemDate = new QTableWidgetItem(ID);
        ui->Table->setItem(ligne,2,itemDate);
    }

    ui->Table->setColumnWidth(0,150);
    ui->Table->setColumnWidth(1,150);
    ui->Table->setColumnWidth(2,150);
}

void ListeModifications::closeEvent(QCloseEvent *event)
{
    emit fermer("listemodifications");
    QWidget::closeEvent(event);
}
