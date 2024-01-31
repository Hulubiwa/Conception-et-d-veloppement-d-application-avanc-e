#ifndef HISTORIQUE_H
#define HISTORIQUE_H

#include <QWidget>
#include "Modification.h"

namespace Ui {
class Historique;
}

/**
 * @class Historique
 * @brief La classe Historique affiche la liste de toutes les modifications de l'ensemble des contacts
 */
class Historique : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de Historique
     * @param parent Le widget parent (par défaut : nullptr).
     */
    explicit Historique(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe Historique
     */
    ~Historique();

    /**
     * @brief Initialise la Table des modifications
     */
    void initTable();

signals:
    /**
     * @brief Signal émis lorsque l'historique se ferme.
     */
    void fermer(string type);

private:
    Ui::Historique *ui;
    vector<Modification> Liste; /**< Liste des modifications */

public slots:
    /**
     * @brief Slot qui récupère la liste des modifications
     * @param listemodifications Liste des modifications
     */
    void ListModifications(vector<Modification> listemodifications);

protected:
    /**
     * @brief Méthode surchargée qui détecte l'événement de fermeture de la fenêtre.
     * @param event L'événement de fermeture.
     */
    void closeEvent(QCloseEvent *event) override;
};

#endif // HISTORIQUE_H
