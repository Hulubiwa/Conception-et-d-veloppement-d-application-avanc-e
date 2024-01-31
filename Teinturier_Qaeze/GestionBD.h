#ifndef GESTIONBD_H
#define GESTIONBD_H

#include <Contact.h>
#include <QtSql/QSqlDatabase>
#include <Modification.h>
#include <QObject>

/**
 * @class GestionBD
 * @brief La classe GestionBD gère la manipulation de la base de donnée en SQL puis l'échange des données avec l'application.
 */
class GestionBD: public QObject
{
    Q_OBJECT

private:
    QSqlDatabase db; /**< Objet de base de données*/

public:

    /**
     * @brief Constructeur de la classe GestionBD
     * @param parent Le widget parent (par défaut : nullptr).
     */
    GestionBD(QObject *parent = nullptr);

    /**
     * @brief Destructeur de la classe GestionBD.
     */
    ~GestionBD();

    /**
     * @brief Connecte la base de données
     */
    void connexionBD();

    /**
     * @brief Si la base de données n'existe pas, alors on en créer une nouvelle.
     */
    void creationBD();

    /**
     * @brief Renouvèle les données d'une Intéraction suite à des modifications
     * @param I Intéraction dont les données doivent être renouvelées
     */
    void modificationInteraction(Interaction I);

    /**
     * @brief Ajout un lien entre un contact, une de ses Interaction et un des Todo de l'Intération
     * @param ContactID Id du Contact
     * @param InteractionID Id de L'Intéraction
     * @param TodoID Id du Todo
     */
    void ajoutAssociation(string ContactID, string InteractionID, string TodoID);

    /**
     * @brief Ajout d'un Todo
     * @param t Todo à ajouter
     */
    void ajoutTodo(Todo t);

    /**
     * @brief Ajout d'un Intéraction
     * @param I Intéraction à ajouter
     */
    void ajoutInteraction(Interaction I);

    /**
     * @brief Ajout d'une modification
     * @param m La modification à ajouter
     */
    void ajoutModification(Modification m);

    /**
     * @brief Suppression d'un lien entre un contact, une de ses Interaction et un des Todo de l'Intération
     * @param contactid Id du contact
     * @param interactionid Id de l'intéraction
     * @param todoid Id du Todo
     */
    void SupprimerAssociation(string contactid, string interactionid, string todoid);

    /**
     * @brief Suppression d'un Todo
     * @param todo Todo à supprimer
     */
    void SupprimerTodo(Todo todo);

    /**
     * @brief Modification d'un Todo
     * @param todo Todo à modifier
     */
    void modificationTodo(Todo todo);

    /**
     * @brief Récupère la liste des identifiants de tous les contacts
     * @return Liste des Ids
     */
    vector<string> getIDsContact();

    /**
     * @brief Récupère la liste des identifiants de toutes les Intéractions
     * @return Liste des Ids
     */
    vector<string> getIDsInteraction();

    /**
     * @brief Récupère la liste des identifiants de tous les Todos
     * @return Liste des Ids
     */
    vector<string> getIDsTodo();

    /**
     * @brief Verification de l'unicité d'un Identifiant
     * @param ID Identifiant à vérifier
     * @param listeIDs Liste des Identifiants
     * @return Vrai si l'unicité est vérifiée sinon false
     */
    bool VerificationID(string ID, vector<string> listeIDs);

    /**
     * @brief Génère un Id pour un contact ou une intéraction ou un Todo
     * @param type Type de l'objet de l'id (contact, intéraction ou todo)
     * @return un Id unique
     */
    string genererID(string type);

    /**
     * @brief Récupère la liste des contacts
     * @return Liste de contacts
     */
    vector<Contact> getContacts();

    /**
     * @brief Récupère la liste des intéractions
     * @return Liste d'intéractions
     */
    vector<Interaction> getInteractions();

    /**
     * @brief Renvoie les modifications associées à un contact spécifique.
     * @param c Le contact dont les modifications sont recherchées.
     * @return Une liste contenant les modifications associées au contact.
     */
    vector<Modification> getModificationsOfContact(Contact c);

    /**
     * @brief Renvoie les interactions associées à un contact spécifique.
     * @param c Le contact dont les interactions sont recherchées.
     * @return Une liste contenant les interactions associées au contact.
     */
    vector<Interaction> getInteractionsOfContact(Contact c);

    /**
     * @brief Renvoie les todos associés à une interaction spécifique d'un contact spécifique.
     * @param InteractionID L'identifiant de l'interaction.
     * @param ContactID L'identifiant du contact.
     * @return Une liste contenant les todos associés à l'interaction pour ce contact.
     */
    vector<Todo> getTodosOfInteraction(string InteractionID, string ContactID);

    /**
     * @brief Récupère l'ensemble des modifications stockés dans la base de données
     * @return  Liste des modifications
     */
    vector<Modification> getModifications();


signals:
    /**
     * @brief Signal émis pour lancer une mise à jour
     */
    void MiseAJour();

    /**
     * @brief Signal émis pour envoyer la liste des contacts et celle des modifications mises à jour.
     * @param ListeC Liste des contact
     * @param ListeM Liste des modifications
     */
    void EnvoiListes(vector<Contact> ListeC, vector<Modification> ListeM);

public slots:
    /**
     * @brief Deconnecte la base de données
     */
    void deconnexionBD();

    /**
     * @brief Ajout d'un Intéraction
     * @param I Intéraction à ajouter
     * @param contactId Id du contact
     */
    void ajoutInteractionTodo(Interaction I, string contactId);

    /**
     * @brief Renouvèle les données d'un contact suite à des modifications
     * @param c Contact dont les données doivent être renouvelées
     */
    void modificationContact(Contact c);

    /**
     * @brief Récupère le signal de la fenêtre principale qu'elle envoi quand elle à besoin de mettre à jour la liste de contact
     */
    void MiseAJourListeContacts();

    /**
     * @brief Enregistrement d'une modifiaction
     * @param type Type de la modification (Suppression ou Modification)
     * @param Id Identifiant du Contact
     */
    void EnregistrerModifiaction(string type, string Id);

    /**
     * @brief Permet de faire l'exportation en JSON de la base de données
     */
    void exportJSON();

    /**
     * @brief Suppression d'un contact
     * @param c Contact à supprimer
     */
    void suppressionContact(Contact c);

    /**
     * @brief Ajout d'un contact
     * @param c Contact à ajouter
     */
    void ajoutContact(Contact c);

    /**
     * @brief Suppression d'une Intéraction
     * @param I Intéraction à supprimer
     */
    void SupprimerInteraction(Interaction I);

};

#endif // GESTIONBD_H
