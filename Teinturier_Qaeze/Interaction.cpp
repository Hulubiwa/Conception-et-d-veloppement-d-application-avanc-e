
#include "Interaction.h"
#include "Todo.h"


Interaction::Interaction(vector<string> c){
    contenu = c;
    ID = "";
    date = floor<seconds>(system_clock::now()) + hours(1); //Ajout d'une heure pour corriger le décalage
}

Interaction::~Interaction(){}

vector<string> Interaction::getContenu(){
    return contenu;
}

string Interaction::getDate()const{
    return std::to_string(date.time_since_epoch().count());
}

string Interaction::getID()const{
    return ID;
}

string Interaction::getIDContact() const{
    return IDContact;
}

void Interaction::setIDContact(string id){
    IDContact = id;
}

void Interaction::ajoutTodo(Todo todo){
    todos.push_back(todo);
}

vector<Todo> Interaction::getTodos(){
    return todos;
}

void Interaction::setID(string Id){
    ID = Id;
}

void Interaction::setDate(string d){
    // Convertion du nombre de secondes depuis la date du 01 Janvier 1970 en date de type "Jour Mois Année"
    int nombreSecondes = std::stoi(d);
    date = sys_days{1970_y/January/1} + seconds{nombreSecondes};
}

void Interaction::setTodos(vector<Todo> lt){
    todos = lt;
}


bool Interaction::operator==(Interaction& I){
    return getID() == I.getID();
}

string Interaction::DateHeureToString(){

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
