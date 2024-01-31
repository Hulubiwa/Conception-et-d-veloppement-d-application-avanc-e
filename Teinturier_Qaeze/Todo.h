

#ifndef TODO_H
#define TODO_H

#include <chrono>
#include <iostream>

#include "date/date.h"
#include "date/chrono_io.h"

using namespace date;
using namespace std::chrono;
using namespace std;
using seconds_point = std::chrono::time_point<std::chrono::system_clock, seconds>;

/**
 * @class Todo
 * @brief La classe Todo nous permet de gérer une tâche à faire dans une interaction
 */
class Todo {
private:
    seconds_point date; /**< date du Todo */
    string Contenu; /**< contenu du Todo */
    string ID; /**< Identifiant du Todo */
    string IDInteraction;   /**< Identifiant de l'intéraction */
    string IDContact;   /**< Identifiant du Contact */

public:
    /**
     * @brief Constructeur de la classe Todo
     * @param contenu Contenu du Todo
     * @param jour  Jour du Todo
     * @param mois  Mois du Todo
     * @param annee  Annee du Todo
     * @param heure  Heure du Todo
     * @param minute  Minute du Todo
     */
    Todo(string contenu, string jour, string mois, string annee, string heure, string minute);

    /**
     * @brief Constructeur de la classe Todo
     * @param contenu Contenu du Todo
     */
    Todo(string contenu);

    /**
     * @brief Destructeur de la classe Todo
     */
    ~Todo();

    /**
     * @brief Renvoie la Date et l'heure en fonction du nombre de seconde écoulées depuis cette le 01/01/1970 à la date précise
     * @return La Date et l'Heure
     */
    string DateHeureToString();

    /**
     * @brief Vérifie si deux Todo sont égales en comparant leur Id.
     */
    bool operator==(Todo &t);

    //get
    string getContenu()const ;
    string getDate() const;
    string getID()const;
    string getIDContact() const;
    string getIDInteraction() const;
    void setIDContact(string id);
    void setIDInteraction(string id);
    void setID(string id);
    void setDate(string d);
    void setContenu(string contenu);
};

#endif

