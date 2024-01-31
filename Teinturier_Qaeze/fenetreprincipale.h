#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include <QMainWindow>
#include "creercontact.h"
#include <QObject>
#include <QDebug>
#include "Contact.h"
#include "GestionBD.h"
#include "cherchercontact.h"
#include "chercherinteraction.h"
#include "historique.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FenetrePrincipale; }
QT_END_NAMESPACE

/**
 * @brief La classe FenetrePrincipale représente la fenêtre principale de l'application.
 */
class FenetrePrincipale : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe FenetrePrincipale.
     * @param parent Le widget parent (par défaut : nullptr).
     */
    FenetrePrincipale(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe FenetrePrincipale.
     */
    ~FenetrePrincipale();

    /**
     * @brief Vérifie les informations d'un contact.
     * @param c Le contact à vérifier.
     * @return Vrai si le contact est valide, sinon faux.
     */
    bool VerificationContact(Contact c);

signals:
    /**
     * @brief Signal émis pour déconnecter la base de donnée.
     */
    void deconnexionBD();

    /**
     * @brief Envoi une Intéraction au gestionnaire de la base de données pour l'ajouter à cette dernière
     * @param I Intéraction à ajouter
     * @param contactId Id du contact de l'intéraction I
     */
    void AjoutInteractionTodo(Interaction I, string contactId);

    /**
     * @brief Envoi le contact modifié au gestionnaire de la base de donnée
     * @param c
     */
    void modificationContact(Contact c);

    /**
     * @brief Envoi la modification d'un contact au gestionnaire de la base de données
     * @param type Type de modification
     * @param IDContact Identifiant du contact modifié
     */
    void EnvoiEnregistrement(string type, string IDContact);

    /**
     * @brief Signal émis pour demander une mise à jour de la liste des contacts.
     */
    void MiseAJourListeContacts();

    /**
     * @brief Signal émis pour demander une mise à jour de la liste des modifications.
     */
    void MiseAJourListeModifications();

    /**
     * @brief Signal émis pour demander au gestionnaire de la base de donnée d'effectuer un export en JSON
     */
    void ExportJSON();

    /**
     * @brief Signal émis pour lancer la suppression d'un contact
     * @param c Contact à supprimer.
     */
    void suppressionContact(Contact c);

    /**
     * @brief Signal émis pour envoyer une Interaction à supprimer de la base de données
     * @param I Interaction à supprimer
     */
    void SupprimerInteraction(Interaction I);

    /**
     * @brief Signal émis pour ajouter un contact
     * @param c Contact à ajouter
     */
    void ajoutContact(Contact c);

    /**
     * @brief Signal émis pour envoyer une liste de contacts.
     * @param liste La liste de contacts à envoyer.
     */
    void envoiListeContact(vector<Contact> liste);

    /**
     * @brief Signal émis pour envoyer une liste d'interactions.
     * @param liste La liste d'interactions à envoyer.
     */
    void envoiListeInteraction(vector<Contact> liste);

    /**
     * @brief Signal émis pour envoyer un historique de modifications.
     * @param liste La liste de modifications à envoyer.
     */
    void envoiHistorique(vector<Modification> liste);

private:
    Ui::FenetrePrincipale *ui;
    CreerContact cc; /**< QWidget pour créer un contact */
    GestionBD gestionbd; /**< Gestionnaire de la base de données*/
    ChercherContact cherchContact;  /**< QWidget pour chercher un contact */
    ChercherInteraction cherchInteraction;  /**< QWidget pour chercher une Intéraction */
    Historique historique;  /**< QWidget pour lister la totalités des modifications */
    vector<Contact> ListeContacts; /**< Liste des contacts présent dans la base de données*/
    vector<Modification> ListeModifications; /**< Liste des modifications */
    QMenuBar * barreMenu;

public slots:
    /**
     * @brief Slot qui Lance la création d'un nouveau contact.
     */
    void lancerCreationContact();

    /**
     * @brief Slot qui Gère la fermeture d'un QWidget.
     * @param type Le type de fermeture.
     */
    void fermerQWidget(string type);

    /**
     * @brief Slot qui permet de faire l'export en JSON.
     */
    void exportJSON();

    /**
     * @brief Slot qui Crée un nouveau contact.
     * @param c Le contact à créer.
     */
    void creerContact(Contact c);

    /**
     * @brief Slot qui Lance la recherche d'interactions.
     */
    void lancerRechercheInteraction();

    /**
     * @brief Slot qui Lance la recherche de contacts.
     */
    void lancerRechercheContact();

    /**
     * @brief Slot qui Récupère les modifications d'un contact.
     * @param c Le contact modifié.
     */
    void recupModificationContact(Contact c);

    /**
     * @brief Slot qui Récupère la suppression d'un contact.
     * @param c Le contact supprimé.
     */
    void recupSuppression(Contact c);

    /**
     * @brief Slot qui Récupère une interaction associée à un contact et qui doit être ajoutée à la base de données.
     * @param I L'interaction à associer.
     * @param contactId L'identifiant du contact associé à l'interaction.
     */
    void recupInteraction(Interaction I, string contactId);

    /**
     * @brief Slot qui Enregistre une modification effectuée.
     * @param type Le type de modification.
     * @param IDContact L'identifiant du contact concerné.
     */
    void EnregistrementModification(string type, string IDContact);

    /**
     * @brief Slot qui Supprime une interaction.
     * @param I L'interaction à supprimer.
     */
    void SuppressionInteraction(Interaction I);

    /**
     * @brief Slot qui Ouvre l'historique des modifications.
     */
    void lancerHistorique();

    /**
     * @brief Slot qui lance une mise à jour des données.
     */
    void MiseAJour();

    /**
     * @brief Slot qui récupère la liste des contacts mise à jour et celle des modifications aussi mise à jour.
     * @param ListeC Liste des contacts
     */
    void recupListes(vector<Contact> ListeC, vector<Modification> ListeM);
};


#endif // FENETREPRINCIPALE_H
