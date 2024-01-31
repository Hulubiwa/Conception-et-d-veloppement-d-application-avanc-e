#include "creercontact.h"
#include "ui_creercontact.h"
#include "Contact.h"
#include <QFile>
#include <QMessageBox>

using namespace std;

CreerContact::CreerContact(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreerContact)
{
    ui->setupUi(this);

    connect(ui->PBValider, SIGNAL(clicked()), this, SLOT(verifierContact()));
    connect(ui->PBAnnuler,SIGNAL(clicked()),this,SLOT(Annuler()));
}


CreerContact::~CreerContact()
{
    delete ui;
}

void CreerContact::verifierContact(){

    if(!ui->nOMLineEdit->text().isEmpty()){
        QString nom = ui->nOMLineEdit->text();
        string Nom = nom.toStdString();
        if(!ui->entrepriseLineEdit->text().isEmpty()){
            QString entreprise = ui->entrepriseLineEdit->text();
            string Entreprise = entreprise.toStdString();
            if(!ui->cheminVersLaPhotoLineEdit->text().isEmpty()){
                QString photo = ui->cheminVersLaPhotoLineEdit->text();
                string Photo = photo.toStdString();
                if(!ui->mailLineEdit->text().isEmpty()){
                    QString mail = ui->mailLineEdit->text();
                    string Mail = mail.toStdString();
                    if(!ui->prNomLineEdit->text().isEmpty()){
                        QString prenom = ui->prNomLineEdit->text();
                        string Prenom = prenom.toStdString();
                        if(!ui->tLPhoneLineEdit->text().isEmpty()){
                            QString telephone = ui->tLPhoneLineEdit->text();
                            string Telephone = telephone.toStdString();

                            if(VerifPhoto(Photo)){
                                Contact c ("None",Nom,Prenom,Entreprise,Mail,Telephone,Photo);
                                emit validerContact(c);
                                this->close();
                            }
                            else if(Photo == "Aucune"){
                                Contact c ("None",Nom,Prenom,Entreprise,Mail,Telephone,"NoImage.jpg");
                                emit validerContact(c);
                                this->close();
                            }
                            else{
                                QMessageBox::critical(this, "Erreur Photo ", "Le chemin de la photo n'est pas correcte. S'il n'y a pas de photo pour le contact, entrez 'Aucune' !");
                            }
                        }
                    }
                }
            }
        }
    }
}

bool CreerContact::VerifPhoto(string image){
    QString img = "../image/"+QString::fromStdString(image);
    QFile file(img);

    bool png = img.endsWith(".png",Qt::CaseInsensitive);
    bool jpg = img.endsWith(".jpg",Qt::CaseInsensitive);

    if(file.exists() && (png || jpg)){
        return true;
    }

    return false;
}

void CreerContact::Initialisation(){
    ui->entrepriseLineEdit->setText("");
    ui->cheminVersLaPhotoLineEdit->setText("");
    ui->mailLineEdit->setText("");
    ui->nOMLineEdit->setText("");
    ui->prNomLineEdit->setText("");
    ui->tLPhoneLineEdit->setText("");
}

void CreerContact::Annuler(){
    Initialisation();
    close();
}

void CreerContact::closeEvent(QCloseEvent *event)
{
    emit fermer("creercontact");
    QWidget::closeEvent(event);
}


