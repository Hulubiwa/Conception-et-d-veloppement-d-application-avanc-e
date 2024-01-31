#ifndef FICHEINTERACTION_H
#define FICHEINTERACTION_H

#include <QWidget>
#include "Interaction.h"
#include <QModelIndex>

namespace Ui {
class FicheInteraction;
}

/**
 * @class FicheInteraction
 * @brief La classe FicheInteraction gère l'affichage et la manipulation des interactions dans l'application.
 */
class FicheInteraction : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe FicheInteraction.
     * @param parent Le widget parent (par défaut : nullptr).
     */
    explicit FicheInteraction(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe FicheInteraction.
     */
    ~FicheInteraction();

    /**
     * @brief Affiche les informations de l'interaction.
     */
    void Affichage();

    /**
     * @brief Crée une nouvelle interaction.
     * @param ltodos La liste des todos existants.
     * @param ltodosnew La liste des nouveaux todos.
     * @param contenu Le contenu de l'interaction.
     */
    void CreationInteraction(vector<string> ltodos, vector<string> ltodosnew, vector<string> contenu);

signals:
    /**
     * @brief Signal émis pour envoyer une interaction modifiée.
     * @param I L'interaction modifiée à envoyer.
     */
    void EnvoiInteractionModifiee(Interaction I);

    /**
     * @brief Signal émis pour supprimer une interaction.
     * @param I L'interaction à supprimer.
     */
    void SuppressionInteraction(Interaction I);

    /**
     * @brief Signal émis pour fermer la fiche de l'interaction.
     */
    void fermer();

    /**
     * @brief Signal émis pour signaler une modification.
     */
    void Modification();

private:
    Ui::FicheInteraction *ui;
    Interaction interaction; /**< Intéraction Courante*/
    vector<Todo> listeTodos; /**< Liste des Todos de l'intéraction*/

public slots:
    /**
     * @brief Slot qui Gère le clic sur le bouton "Fermer".
     */
    void PBFermer_clicked();

    /**
     * @brief Slot qui Lance la création de l'intéraction.
     */
    void Creation();

    /**
     * @brief Gère le clic sur le bouton "Modifier".
     */
    void PBModifier_clicked();

    /**
     * @brief Récupère une interaction.
     * @param I L'interaction à récupérer.
     */
    void recupInteraction(Interaction I);

    /**
     * @brief Gère le clic sur le bouton "Supprimer".
     */
    void PBSupprimer_clicked();

protected:
    /**
     * @brief Méthode surchargée qui détecte l'événement de fermeture de la fenêtre.
     * @param event L'événement de fermeture.
     */
    void closeEvent(QCloseEvent *event) override;
};

#endif // FICHEINTERACTION_H
