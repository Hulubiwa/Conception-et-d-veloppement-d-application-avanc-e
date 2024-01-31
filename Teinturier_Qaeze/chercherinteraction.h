#ifndef CHERCHERINTERACTION_H
#define CHERCHERINTERACTION_H

#include <QWidget>
#include <iostream>
#include "Contact.h"
#include "Interaction.h"
#include "ficheinteraction.h"

/**
 * @class ChercherInteraction
 * @brief La classe ChercherInteraction nous sert à lister la totalité des intéractions
 */

namespace Ui {
class ChercherInteraction;
}

class ChercherInteraction : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe ChercherInteraction.
     * @param parent Le widget parent (par défaut : nullptr).
     */
    explicit ChercherInteraction(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe ChercherInteraction.
     */
    ~ChercherInteraction();

    /**
     * @brief Met à jour la liste courante.
     * @param type Précise le type de la liste "Todo" ou "Interaction".
     */
    void MiseAJour(string type);

    /**
     * @brief Récupère un contact en fonction de son Id.
     * @param Id Identifiant du contact recherché.
     * @return Le contact correspondant à l'Id
     */
    Contact getContactByID(string Id);

    /**
     * @brief Récupère une interaction en fonction de son identifiant.
     * @param Id L'identifiant de l'interaction à récupérer.
     * @return L'interaction correspondant à l'Id
     */
    Interaction getInteractionByID(string Id);

    /**
     * @brief Met à jour les listes internes.
     */
    void MiseAJourListes();


signals:
    /**
     * @brief Signal émis pour fermer la fenêtre courante.
     * @param type Permet d'identifier le la fenêtre qui envoie la le signal.
     */
    void fermer(string type);

    /**
     * @brief Signal émis pour envoyer une interaction.
     * @param I L'interaction à envoyer.
     */
    void EnvoiInteraction(Interaction I);

    /**
     * @brief Signal émis pour envoyer un contact.
     * @param c Le contact à envoyer.
     */
    void EnvoiContact(Contact c);

    /**
     * @brief Signal émis pour supprimer une interaction.
     * @param I L'interaction à supprimer.
     */
    void SuppressionInteraction(Interaction I);

private:
    Ui::ChercherInteraction *ui;
    vector<Interaction> ListeInteractions; /**< Liste des Intéraction */
    vector<Todo> ListeTodos;   /**< Liste des Todos */
    vector <Contact> ListeContacts; /**< Liste des contacts */
    string typeListe; /**< Type de la liste courante */
    FicheInteraction fiche; /**< Fiche de l'intéraction sélectionnée */

public slots :

    /**
     * @brief Slot qui initialise les listes avec une nouvelle liste de contacts.
     * @param liste La liste de contacts pour initialiser.
     */
    void initListes(vector<Contact> liste);

    /**
     * @brief Slot qui gère le clic sur le bouton "Liste des Intéractions".
     */
    void PBInteractions();

    /**
     * @brief Slot qui gère le clic sur le bouton "Liste de Todos".
     */
    void PBTodos();

    /**
     * @brief Slot qui gère le clic sur le bouton "Fermer".
     */
    void PBFermer();

    /**
     * @brief Slot qui gère le clic sur le bouton "Lancer Recherche".
     */
    void PBRechercher();

    /**
     * @brief Slot qui gère le clic sur le bouton "Chercher".
     */
    void PBDate();

    /**
     * @brief Slot qui gère le clic sur le bouton "Liste Complète".
     */
    void PBListe();

    /**
     * @brief Slot qui récupère une interaction modifiée.
     * @param I L'interaction modifiée à récupérer.
     */
    void recupInteractionModifiee(Interaction I);

    /**
     * @brief Slot qui récupère interaction à supprimer.
     * @param I L'interaction à supprimer.
     */
    void SupprimerInteraction(Interaction I);

    /**
     * @brief Slot qui récupère le signal envoyé par la QWidget "FicheInteraction" lorsqu'elle se ferme.
     */
    void fermerQWidget();

    /**
     * @brief Gère le clic sur la table.
     * @param index L'index de la table cliquée.
     */
    void CliqueTable(const QModelIndex &index);



protected:
    /**
     * @brief Méthode surchargée qui détecte l'événement de fermeture de la fenêtre.
     * @param event L'événement de fermeture.
     */
    void closeEvent(QCloseEvent *event) override;
};

#endif // CHERCHERINTERACTION_H
