#ifndef CREERINTERACTION_H
#define CREERINTERACTION_H

#include <QWidget>
#include "Interaction.h"
#include "Todo.h"

namespace Ui {
class CreerInteraction;
}

/**
 * @class CreerInteraction
 * @brief La classe CreerInteraction permet de créer et de gérer un formulaire de création d'interaction.
 */
class CreerInteraction : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe CreerInteraction.
     * @param parent Le widget parent (par défaut : nullptr).
     */
    explicit CreerInteraction(QWidget *parent = nullptr);

    /**
     * @brief Initialise le formulaire pour la création d'interaction.
     * @param liste La liste des Todos.
     * @param contenu Le contenu associé à l'interaction.
     */
    void CreationInteraction(vector<string> liste, vector<string> contenu);

    /**
     * @brief Destructeur de la classe CreerInteraction.
     */
    ~CreerInteraction();

signals:
    /**
     * @brief Signal émis pour fermer le formulaire.
     * @param type Identifiant de la QWidget.
     */
    void fermer(string type);

    /**
     * @brief Signal émis pour créer une interaction.
     */
    void creerInteraction();

    /**
     * @brief Signal émis pour envoyer l'interaction à créer.
     * @param interaction L'interaction à créer.
     */
    void EnvoiInteraction(Interaction interaction);

private:
    Ui::CreerInteraction *ui;

public slots:
    /**
     * @brief Slot qui Gère le clic sur le bouton Valider.
     */
    void PBValider_clicked();

    /**
     * @brief Slot qui Lance la création de l'intéraction.
     */
    void Creation();

    /**
     * @brief Slot qui Annule la création d'interaction.
     */
    void Annuler();

protected:
    /**
     * @brief Méthode surchargée qui détecte l'événement de fermeture de la fenêtre.
     * @param event L'événement de fermeture.
     */
    void closeEvent(QCloseEvent *event) override;
};

#endif // CREERINTERACTION_H
