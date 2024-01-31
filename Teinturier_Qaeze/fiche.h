#ifndef FICHE_H
#define FICHE_H

#include <QWidget>
#include "Contact.h"
#include "modifiercontact.h"
#include "Modification.h"
#include "listemodifications.h"
#include "creerinteraction.h"
#include "listeinteractioncontact.h"

namespace Ui {
class Fiche;
}

/**
 * @class Fiche
 * @brief La classe Fiche représente une fiche de contact dans l'application.
 */
class Fiche : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe Fiche.
     * @param parent Le widget parent (par défaut : nullptr).
     */
    explicit Fiche(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe Fiche.
     */
    ~Fiche();

    /**
     * @brief Met à jour les données de la fiche.
     */
    void MiseAjourDonnees();

signals:
    /**
     * @brief Signal émis pour envoyer les modifications apportées à un contact.
     * @param c Le contact modifié à envoyer.
     */
    void EnvoiModificationsContact(Contact c);

    /**
     * @brief Signal émis pour envoyer l'enregistrement des modifications.
     * @param type Le type de modification.
     * @param Id L'identifiant du contact associé à la modification.
     */
    void EnvoiEnregistrement(string type, string Id);

    /**
     * @brief Signal émis pour envoyer un contact.
     * @param c Le contact à envoyer.
     */
    void EnvoiContact(Contact c);

    /**
     * @brief Signal émis pour envoyer une liste de modifications.
     * @param liste La liste des modifications à envoyer.
     */
    void EnvoiModification(vector<Modification> liste);

    /**
     * @brief Signal émis pour envoyer une interaction associée à un contact.
     * @param I L'interaction à envoyer.
     * @param contactId L'identifiant du contact associé à l'interaction.
     */
    void EnvoiInteraction(Interaction I, string contactId);

    /**
     * @brief Signal émis pour envoyer une liste d'interactions.
     * @param I La liste d'interactions à envoyer.
     */
    void EnvoiInteractions(vector<Interaction> I);

    /**
     * @brief Signal émis pour envoyer une interaction modifiée.
     * @param I L'interaction modifiée à envoyer.
     */
    void EnvoiInteractionModifiee(Interaction I);

    /**
     * @brief Signal émis pour fermer la fiche.
     */
    void fermer();

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


private:
    Ui::Fiche *ui;
    Contact contact; /**< Contact courant*/
    ModifierContact ModifContact; /**< QWidget pour modifier le contact*/
    ListeModifications Modifications; /**< QWidget pour lister les modifications du contact*/
    CreerInteraction creerinteraction; /**< QWidget pour créer une intéraction pour le contact*/
    ListeInteractionContact listeInteractions; /**< QWidget pour lister les intéractions du contact*/

public slots:
    /**
     * @brief Slot qui Supprime une interaction.
     * @param I L'interaction à supprimer.
     */
    void SupprimerInteraction(Interaction I);

    /**
     * @brief Slot qui Récupère les modifications d'un contact.
     * @param c Le contact modifié.
     */
    void recupModificationContact(Contact c);

    /**
     * @brief Slot qui Initialise les données d'un contact.
     * @param C Le contact à initialiser.
     */
    void initContact(Contact C);

    /**
     * @brief Slot qui Gère le clic sur le bouton "Fermer".
     */
    void on_PBFermer_clicked();

    /**
     * @brief Slot qui Gère le clic sur le bouton "Modifier".
     */
    void on_PBModifier_clicked();

    /**
     * @brief Slot qui Gère le clic sur le bouton "Modifications".
     */
    void PBModification_clicked();

    /**
     * @brief Slot qui récupère le signal envoyé par une QWidget lorsqu'elle se ferme.
     * @param type Identifiant de la QWidget.
     */
    void fermerQWidget(string type);

    /**
     * @brief Slot qui Supprime un contact.
     */
    void supprimerContact();

    /**
     * @brief Slot qui Gère le clic sur le bouton d'interaction.
     */
    void PBInteraction_clicked();

    /**
     * @brief Slot qui Gère le clic sur le bouton d'interactions.
     */
    void PBInteractions_clicked();

    /**
     * @brief Slot qui Récupère une interaction.
     * @param I L'interaction à récupérer.
     */
    void recupInteraction(Interaction I);

    /**
     * @brief Slot qui Récupère une interaction modifiée.
     * @param I L'interaction modifiée à récupérer.
     */
    void recupInteractionModifiee(Interaction I);

    /**
     * @brief Slot qui Enregistre une modification.
     * @param type Le type de modification.
     * @param Id L'identifiant de la modification.
     */
    void EnregistrementModification(string type, string Id);


protected:
    /**
     * @brief Méthode surchargée qui détecte l'événement de fermeture de la fenêtre.
     * @param event L'événement de fermeture.
     */
    void closeEvent(QCloseEvent *event) override;
};

#endif // FICHE_H
