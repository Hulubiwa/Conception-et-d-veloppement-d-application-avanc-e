#ifndef CONTACT_H
#define CONTACT_H

#include <chrono>
#include <iostream>
#include "Todo.h"
#include "Interaction.h"
#include <QList>
#include "Modification.h"

#include "date/date.h"
#include "date/chrono_io.h"

using namespace date;
using namespace std::chrono;
using std::chrono::seconds;
using namespace std;
using seconds_point = std::chrono::time_point<std::chrono::system_clock, seconds>;


/**
 * @class Contact
 * @brief La classe Contact nous sert à gérer les informations de la fiche d'un contact ainsi que ses interactions.
 */
class Contact
{
private:
    string numeroID; /**< Identifiant du contact */
    string nom; /**< nom du contact */
    string prenom; /**< prénom du contact */
    string entreprise; /**< nom de l'entreprise du contact */
    string mail; /**< adresse e-mail du contact */
    string telephone; /**< numéro de téléphone du contact */
    string photo; /**< chemin vers la photo du contact */
    seconds_point dateCreation; /**< date de création du contact */
    vector<Modification> Modifications; /**< liste des modifications associées au contact */
    vector<Interaction> Interactions; /**< liste des interactions associées au contact */

public:
    /**
     * @brief Constructeur de la classe Contact avec la date courante
     */
    Contact(string NumeroID, string Nom, string Prenom, string Entreprise, string Mail, string Telephone,string Photo);

    /**
     * @brief Constructeur de la classe Contact avec une date donnée
     */
    Contact(string NumeroID, string Nom, string Prenom, string Entreprise, string Mail, string Telephone,string Photo, string Date);

    /**
     * @brief Constructeur de la classe Contact par défaut
     */
    Contact();

    /**
     * @brief Destructeur de la classe Contact
     */
    ~Contact();

    /**
     * @brief Ajoute une interaction au contact
     * @param I Intéraction à ajouter
     */
    void ajouterInteraction(Interaction I);

    /**
     * @brief Supprime une interaction au contact
     * @param I Intéraction à supprimer.
     */
    void supprimerInteraction(Interaction I);

    /**
     * @brief Renvoie la Date et l'heure en fonction du nombre de seconde écoulées depuis cette le 01/01/1970 à la date précise
     * @return La Date et l'Heure
     */
    string DateHeureToString();

    // Surcharge de l'opérateur ==
    bool operator==(const Contact &other) const;


    // get / set

    string getNom() const;
    string getPrenom() const;
    string getEntreprise() const;
    string getMail() const;
    string getTelephone() const;
    string getPhoto() const;
    string getDate() const;
    string getID() const;
    vector<Modification> getModifications();
    vector<Interaction> getInteractions();
    void setNom(string n);
    void setPrenom(string p);
    void setEntreprise(string e);
    void setMail(string m);
    void setTelephone(string t);
    void setPhoto(string p);
    void setDate(seconds_point d);
    void setModifications(vector<Modification> liste);
    void setInteractions(vector<Interaction> liste);
    void ajoutModification(Modification modification);
};

#endif
