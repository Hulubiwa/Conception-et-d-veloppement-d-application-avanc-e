#ifndef MODIFICATION_H
#define MODIFICATION_H

#include <chrono>
#include <iostream>
#include "date/date.h"
#include "date/chrono_io.h"
#include <iomanip>
#include <sstream>

using namespace date;
using namespace std::chrono;
using namespace std;
using seconds_point = std::chrono::time_point<std::chrono::system_clock, seconds>;


/**
 * @class Modification
 * @brief La classe Modification nous sert à enregistrer une modification ou une suppression d'une fiche contact (avec la date à laquelle l'événement a été réalisé)
 */
class Modification {
private:
    string typeModification; /**< type de modification effectuée */
    string Contenu; /**< Contenu de la modification */
    string ContactID; /**< Id du contact */
    seconds_point Date; /**< date de la modification */

public:
    /**
     * @brief Constructeur de la classe Modification.
     */
    Modification(string type, string contactID);

    /**
     * @brief Destructeur de la classe Modification.
     */
    ~Modification();

    /**
     * @brief Renvoie la Date et l'heure en fonction du nombre de seconde écoulées depuis cette le 01/01/1970 à la date précise
     * @return La Date et l'Heure
     */
    string DateHeureToString();

    string getContenu()const;
    string getType()const;
    string getDate()const;
    string getContactID()const;
    void setDate(string date);
};

#endif
