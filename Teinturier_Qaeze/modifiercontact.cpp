#include "modifiercontact.h"
#include "ui_modifiercontact.h"
#include <QMessageBox>
#include <QFile>

ModifierContact::ModifierContact(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifierContact)
{
    ui->setupUi(this);
}

ModifierContact::~ModifierContact()
{
    delete ui;
}

void ModifierContact::recupContact(Contact c){
    contact = c;

    connect(ui->PBAnnuler,SIGNAL(clicked()),this,SLOT(PBAnnuler_clicked()));
    connect(ui->PBValider,SIGNAL(clicked()),this,SLOT(PBValider_clicked()));

    //Initialisation des données du contact
    ui->LineNom->setText(QString::fromStdString(c.getNom()));
    ui->LinePrenom->setText(QString::fromStdString(c.getPrenom()));
    ui->LineEntreprise->setText(QString::fromStdString(c.getEntreprise()));
    ui->LineMail->setText(QString::fromStdString(c.getMail()));
    ui->LineTelephone->setText(QString::fromStdString(c.getTelephone()));
    ui->LinePhoto->setText(QString::fromStdString(c.getPhoto()));
}

void ModifierContact::PBValider_clicked()
{
    //Récupération des modifications
    contact.setNom(ui->LineNom->text().toStdString());
    contact.setPrenom(ui->LinePrenom->text().toStdString());
    contact.setEntreprise(ui->LineEntreprise->text().toStdString());
    contact.setMail(ui->LineMail->text().toStdString());
    contact.setTelephone(ui->LineTelephone->text().toStdString());   

    QString img = ui->LinePhoto->text();

    if(VerifPhoto("../image/"+img.toStdString())){
        contact.setPhoto(img.toStdString());

        //Envoie des modifications
        emit EnvoiModifications(contact);
        emit EnvoiEnregistrement("Modification",contact.getID());

        close();
    }
    else if(img == "Aucune"){
        contact.setPhoto("NoImage.jpg");

        //Envoie des modifications
        emit EnvoiModifications(contact);
        emit EnvoiEnregistrement("Modification",contact.getID());

        close();
    }
    else{
        QMessageBox::critical(this, "Erreur Photo ", "Le chemin de la photo n'est pas correcte. S'il n'y a pas de photo pour le contact, entrez 'Aucune' !");
    }

}

bool ModifierContact::VerifPhoto(string image){
    QString img = QString::fromStdString(image);
    QFile file(img);

    bool png = img.endsWith(".png",Qt::CaseInsensitive);
    bool jpg = img.endsWith(".jpg",Qt::CaseInsensitive);

    if(file.exists() && (png || jpg)){
        return true;
    }

    return false;
}

void ModifierContact::PBAnnuler_clicked()
{
    close();
}

void ModifierContact::closeEvent(QCloseEvent *event)
{
    //Déconnexion
    disconnect(ui->PBAnnuler,SIGNAL(clicked()),this,SLOT(PBAnnuler_clicked()));
    disconnect(ui->PBValider,SIGNAL(clicked()),this,SLOT(PBValider_clicked()));
    emit fermer("modifiercontact");
    QWidget::closeEvent(event);
}

