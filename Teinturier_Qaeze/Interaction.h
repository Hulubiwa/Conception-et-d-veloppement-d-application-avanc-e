#ifndef INTERACTION_H
#define INTERACTION_H

#include <chrono>
#include <iostream>
#include "Todo.h"
#include <list>
#include <vector>
#include "date/date.h"
#include "date/chrono_io.h"
#include <sstream>
#include <iomanip>

using namespace date;
using namespace std::chrono;
using namespace std;
using seconds_point = std::chrono::time_point<std::chrono::system_clock, seconds>;

/**
 * @class Interaction
 * @brief La classe Interaction nous permet de gérer une interaction d'un contact et les tâches à faire (Todo) associées
 */
class Interaction {
private:
    string ID; /**< Identifiant de l'intéraction */
    string IDContact; /**< Identifiant du contact possédant l'intéraction */
    vector<Todo> todos; /**< liste des tâches associées à l'interaction */
    vector<string> contenu; /**< contenu de l'interaction */
    seconds_point date; /**< date de l'interaction */
    list<seconds_point> datesModifications; /**< liste des dates des modifications de l'interaction */

public:

    Interaction() = default; // Constructeur par défaut

    /**
     * @brief Constructeur de la classe Interaction
     */
    Interaction(vector<string> c);

    /**
     * @brief Destructeur de la classe Interaction
     */
    ~Interaction();

    /**
     * @brief Ajoute un Todo à l'interaction
     */
    void ajoutTodo(Todo todo);

    /**
     * @brief Vérifie si deux interactions sont égales en comparant leur Identifiant
     */
    bool operator==(Interaction &I);

    /**
     * @brief Renvoie la Date et l'heure en fonction du nombre de seconde écoulées depuis cette le 01/01/1970 à la date précise
     * @return La Date et l'Heure
     */
    string DateHeureToString();


    //get
    vector<string> getContenu();
    string getDate() const;
    string getID()const;
    vector<Todo> getTodos();
    string getIDContact() const;
    void setIDContact(string id);
    void setID(string Id);
    void setDate(string d);
    void setTodos(vector<Todo> lt);
};

#endif
