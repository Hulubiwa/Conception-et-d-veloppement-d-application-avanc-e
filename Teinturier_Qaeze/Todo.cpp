#include "Todo.h"
#include<iostream>
#include <iomanip>

using namespace std;

Todo::Todo(string contenu, string jour, string mois, string annee, string heure, string minute){

    // Convertir les composants de date et d'heure en entiers
    int jourInt = std::stoi(jour);
    int moisInt = std::stoi(mois);
    int anneeInt = std::stoi(annee);
    int heureInt = std::stoi(heure);
    int minuteInt = std::stoi(minute);

    Contenu = contenu;
    // Créer la date et l'heure
    date = date::sys_days{date::year_month_day{date::year(anneeInt), date::month(moisInt), date::day(jourInt)}} +
           std::chrono::hours{heureInt} + std::chrono::minutes{minuteInt};
}

Todo::Todo(string contenu){
    Contenu = contenu;

    date = floor<seconds>(system_clock::now()) + hours(1); //Ajout d'une heure pour corriger le décalage
}

Todo::~Todo(){}

string Todo::getContenu()const{
    return Contenu;
}

string Todo::getDate()const{
    return std::to_string(date.time_since_epoch().count());
}

string Todo::getID()const{ return ID;}

string Todo::getIDContact()const{ return IDContact;}

string Todo::getIDInteraction()const{ return IDInteraction;}

void Todo::setID(string id){
    ID = id;
}

void Todo::setIDContact(string id){
    IDContact = id;
}

void Todo::setIDInteraction(string id){
    IDInteraction = id;
}

void Todo::setDate(string d){
    // Convertion du nombre de secondes depuis la date du 01 Janvier 1970 en date de type "Jour Mois Année"
    int nombreSecondes = std::stoi(d);
    date = sys_days{1970_y/January/1} + seconds{nombreSecondes};
}

void Todo::setContenu(string contenu){
    Contenu = contenu;
}

string Todo::DateHeureToString(){

    long long secondes = date.time_since_epoch().count();

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

bool Todo::operator==(Todo& t){
    return getID() == t.getID();
}
