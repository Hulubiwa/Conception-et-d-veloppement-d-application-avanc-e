#include "Modification.h"
#include<iostream>

Modification::~Modification(){}

Modification::Modification(string type, string contactID):typeModification(type),ContactID(contactID){
    Date = floor<seconds>(system_clock::now());
    if(type == "Suppression")
        Contenu = "Suppression du contact "+ contactID;
    else
        Contenu = "Modification du contact "+ contactID;
}

string Modification::getDate()const{
    return std::to_string(Date.time_since_epoch().count());
}

string Modification::DateHeureToString(){

    long long secondes = Date.time_since_epoch().count();
    //Correction du décalage de 1h
    secondes += 3600;

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

string Modification::getContenu()const{ return Contenu;}

string Modification::getType()const{ return typeModification;}

string Modification::getContactID()const{ return ContactID;}

void Modification::setDate(string date){
    // Convertion du nombre de secondes depuis la date du 01 Janvier 1970 en date de type "Jour Mois Année"
    int nombreSecondes = std::stoi(date);
    Date = sys_days{1970_y/January/1} + seconds{nombreSecondes};
}


