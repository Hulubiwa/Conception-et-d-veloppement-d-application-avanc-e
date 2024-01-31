#ifndef CHERCHERCONTACT_H
#define CHERCHERCONTACT_H

#include <QWidget>
#include <Contact.h>
#include <fiche.h>

/**
 * @class ChercherContact
 * @brief La classe ChercherContact nous sert à lister les contacts
 */
namespace Ui {
class ChercherContact;
}

class ChercherContact : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe ChercherContact
     * @param parent Le widget parent (par défaut : nullptr).
     */
    explicit ChercherContact(QWidget *parent = nullptr);
    /**
     * @brief Destructeur de la classe ChercherContact
     */
    ~ChercherContact();

    /**
     * @brief Met à jour la liste de contact en fonction de la liste courante
     */
    void MiseAJourTable(vector<Contact> liste);

signals:
    /**
     * @brief Signal émis pour envoyer le contact lors de l'ouverture de sa Fiche.
     * @param c Le contact sélectionné sur la liste.
     */
    void EnvoiContact(Contact c);

    /**
     * @brief Signal émis pour envoyer une modification de contact à la base de données.
     * @param c Le contact modifié à envoyer.
     */
    void EnvoiModificationContact(Contact c);

    /**
     * @brief Signal émis pour envoyer une  nouvelle intéraction ou une intéraction modifiée.
     * @param I L'interaction à envoyer.
     * @param contactId L'identifiant du contact associé à cette interaction.
     */
    void EnvoiInteraction(Interaction I, string contactId);

    /**
     * @brief Signal émis pour supprimer un contact.
     * @param c Le contact à supprimer.
     */
    void SupprimerContact(Contact c);

    /**
     * @brief Signal émis pour supprimer une interaction.
     * @param I L'interaction à supprimer.
     */
    void SuppressionInteraction(Interaction I);

    /**
     * @brief Signal émis pour fermer la fenêtre courante.
     * @param type Permet d'identifier le la fenêtre qui envoie la le signal.
     */
    void fermer(string type);

    /**
     * @brief Signal émis pour enregistrer une modification.
     * @param type Le type de modification effectuée.
     * @param ID L'identifiant du contact associé à la modification.
     */
    void EnregistrementModification(string type, string ID);

private:
    Ui::ChercherContact *ui;
    Contact contactFiche; /**< Contact dont la fiche est ouverte*/
    bool BFiche; /**< Vrai si une fiche est ouverte */
    vector <Contact> Liste; /**< Liste des contacts */
    vector <Contact> ListeCourante; /**< Liste courante de contact */
    Fiche fiche; /**< Fiche du contact */

public slots:
    /**
     * @brief Slot qui récupère l'intéraction à supprimer.
     * @param I L'intéraction à supprimer.
     */
    void SupprimerInteraction(Interaction I);

    /**
     * @brief Slot qui récupère la liste des contacts.
     * @param listeContact La liste des contacts.
     */
    void recupListeContact(vector<Contact> listeContacts);

    /**
     * @brief Slot qui récupère le contact modifié.
     * @param c Contact modifié.
     */
    void recupModificationContact(Contact c);

    /**
     * @brief Slot qui récupère le contact à supprimer.
     * @param c Contact à supprimer.
     */
    void recupSuppressionContact(Contact c);

    /**
     * @brief Slot qui récupère l'intéraction modifiée / à supprimer / nouvellement ajoutée.
     * @param I Intéraction modifiée / à supprimer / nouvellement ajoutée.
     * @param contactId Identifiant du contact
     */
    void recupInteraction(Interaction I, string contactId);

    /**
     * @brief Slot qui récupère la modification d'un contact.
     * @param type Type de modification.
     * @param Id Identifiant du contact.
     */
    void recupEnregistrement(string type, string Id);

    /**
     * @brief Slot qui récupère le signal de clique lors de la sélection du contact.
     * @param index Index du contact sélectionné.
     */
    void ContactCliqueTable(const QModelIndex &index);

    /**
     * @brief Slot qui récupère le signal envoyé par la QWidget "Fiche" lorsqu'elle se ferme.
     */
    void fermerQWidget();

    /**
     * @brief Slot qui trie la liste par ordre alphabétique.
     */
    void TrieAlphabetique();

    /**
     * @brief Slot qui trie la liste par ordre de Date.
     */
    void TrieDate();

    /**
     * @brief Slot qui lance une recherche en fonction du Nom du contact ou de son entreprise.
     */
    void CliqueLancerRecherche();

    /**
     * @brief Slot qui réinitialise la liste.
     */
    void ListeComplete();

protected:
    /**
     * @brief Méthode surchargée qui détecte l'événement de fermeture de la fenêtre.
     * @param event L'événement de fermeture.
     */
    void closeEvent(QCloseEvent *event) override;
};

#endif // CHERCHERCONTACT_H
