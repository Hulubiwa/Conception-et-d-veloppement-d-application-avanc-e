#ifndef LISTEINTERACTIONCONTACT_H
#define LISTEINTERACTIONCONTACT_H

#include <QWidget>
#include "Interaction.h"
#include "ficheinteraction.h"

namespace Ui {
class ListeInteractionContact;
}

/**
 * @class ListeInteractionContact
 * @brief La classe ListeInteractionContact gère l'affichage et la manipulation des interactions associées à un contact.
 */
class ListeInteractionContact : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe ListeInteractionContact.
     * @param parent Le widget parent (par défaut : nullptr).
     */
    explicit ListeInteractionContact(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe ListeInteractionContact.
     */
    ~ListeInteractionContact();

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
     * @brief Signal émis pour envoyer une interaction modifiée.
     * @param I L'interaction modifiée à envoyer.
     */
    void EnvoiInteractionModifiee(Interaction I);

    /**
     * @brief Signal émis pour mettre à jour la liste d'interactions.
     * @param lst La nouvelle liste d'interactions.
     */
    void MiseAJourListe(vector<Interaction> lst);

    /**
     * @brief Signal émis pour supprimer une interaction.
     * @param I L'interaction à supprimer.
     */
    void SuppressionInteraction(Interaction I);


private:
    Ui::ListeInteractionContact *ui;
    vector<Interaction> liste; /**< Liste des Intéractions*/
    FicheInteraction fiche; /**< QWidget pour Afficher les données d'une intéraction */

public slots:
    /**
     * @brief Slot qui Sélectionne une interaction dans la liste.
     * @param index L'index de l'interaction sélectionnée.
     */
    void selectInteraction(QModelIndex index);

    /**
     * @brief Slot qui Initialise la liste des interactions.
     * @param liste La nouvelle liste d'interactions.
     */
    void initInteraction(vector<Interaction> liste);

    /**
     * @brief Slot qui Récupère une interaction modifiée.
     * @param I L'interaction modifiée récupérée.
     */
    void recupInteractionModifiee(Interaction I);

    /**
     * @brief Slot qui Ferme le QWidget FicheInteraction.
     */
    void fermerQWidget();

    /**
     * @brief Slot qui Gère le clic sur le bouton "Fermer".
     */
    void PBFermer_clicked();

    /**
     * @brief Supprime une interaction.
     * @param I L'interaction à supprimer.
     */
    void SupprimerInteraction(Interaction I);

protected:
    /**
     * @brief Méthode surchargée qui détecte l'événement de fermeture de la fenêtre.
     * @param event L'événement de fermeture.
     */
    void closeEvent(QCloseEvent *event) override;

};

#endif // LISTEINTERACTIONCONTACT_H
