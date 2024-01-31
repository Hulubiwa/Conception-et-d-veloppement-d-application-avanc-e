#include "Todo.h"
#include "Contact.h"
#include<iostream>
#include "Interaction.h"
#include <iomanip>
#include <sstream>


using namespace std;

Contact::Contact(string NumeroID, string Nom, string Prenom, string Entreprise, string Mail, string Telephone,string Photo){
    numeroID = NumeroID;
    nom = Nom;
    prenom = Prenom;
    entreprise = Entreprise;
    mail = Mail;
    telephone = Telephone;
    photo = Photo;
    dateCreation = floor<seconds>(system_clock::now()) + hours(1); //Ajout d'une heure pour corriger le décalage
}

Contact::Contact(string NumeroID, string Nom, string Prenom, string Entreprise, string Mail, string Telephone,string Photo, string Date){
    numeroID = NumeroID;
    nom = Nom;
    prenom = Prenom;
    entreprise = Entreprise;
    mail = Mail;
    telephone = Telephone;
    photo = Photo;

    // Convertion du nombre de jour depuis la date du 01 Janvier 1970 en date de type "Jour Mois Année"
    int numberOfDays = std::stoi(Date);
    dateCreation = sys_days{1970_y/January/1} + seconds{numberOfDays};
}

Contact::Contact(){
    numeroID = "";
    nom = "";
    prenom = "";
    entreprise = "";
    mail = "";
    telephone = "";
    photo = "";
    dateCreation = floor<seconds>(system_clock::now()) + hours(1); //Ajout d'une heure pour corriger le décalage
}

void Contact::ajouterInteraction(Interaction I){
    Interactions.push_back(I);
}

void Contact::supprimerInteraction(Interaction I){
    for(int i=0; i<Interactions.size(); i++){
        Interaction interaction = Interactions.at(i);
        if(interaction == I){
            Interactions.erase(Interactions.begin()+i);
        }
    }
}

Contact::~Contact(){}

string Contact::getNom()const{ return nom;}

string Contact::getPrenom()const{ return prenom;}

string Contact::getEntreprise()const{ return entreprise;}

string Contact::getMail()const{ return mail;}

string Contact::getTelephone()const{ return telephone;}

string Contact::getDate()const{
    return std::to_string(dateCreation.time_since_epoch().count());
}

string Contact::getID()const{ return numeroID;}

string Contact::getPhoto()const{ return photo;}

vector<Modification> Contact::getModifications(){ return Modifications;}

vector<Interaction> Contact::getInteractions(){ return Interactions;}

void Contact::setNom(string n) { nom = n; }

void Contact::setPrenom(string p) { prenom = p; }

void Contact::setEntreprise(string e) { entreprise = e; }

void Contact::setMail(string m) { mail = m; }

void Contact::setTelephone(string t) { telephone = t; }

void Contact::setPhoto(string p) {photo = p; }

void Contact::setModifications(vector<Modification> liste){Modifications = liste;}
void Contact::setInteractions(vector<Interaction> liste){Interactions = liste;}

string Contact::DateHeureToString(){

    long long secondes = dateCreation.time_since_epoch().count();

    // Convertir les secondes en duration
    std::chrono::seconds s(secondes);

    // Créer un time_point à partir de la durée
    std::chrono::system_clock::time_point tp{s};

    // Convertir le time_point en date et heure
    auto dp = date::floor<date::days>(tp);
    auto time = date::make_time(tp - dp);
    auto ymd = date::year_month_day{dp};

    // Convertir en string
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << unsigned{ymd.day()} << "/"
        << std::setfill('0') << std::setw(2) << unsigned{ymd.month()} << "/"
        << ymd.year() << " "
        << std::setfill('0') << std::setw(2) << time.hours().count() << "h"
        << std::setfill('0') << std::setw(2) << time.minutes().count();


    return oss.str();
}

bool Contact::operator==(const Contact &other) const {
    // Comparaison des membres pour déterminer l'égalité
    return (numeroID == other.getID());
}

void Contact::ajoutModification(Modification modification){
    Modifications.push_back(modification);
}
