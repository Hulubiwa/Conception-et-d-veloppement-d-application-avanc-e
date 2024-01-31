#ifndef LISTEMODIFICATIONS_H
#define LISTEMODIFICATIONS_H

#include <QWidget>
#include <Modification.h>

namespace Ui {
class ListeModifications;
}

/**
 * @class ListeModifications
 * @brief La classe ListeModifications gère l'affichage des modifications dans un tableau.
 */
class ListeModifications : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe ListeModifications.
     * @param parent Le widget parent (par défaut : nullptr).
     */
    explicit ListeModifications(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe ListeModifications.
     */
    ~ListeModifications();

    /**
     * @brief Met à jour le tableau avec une liste de modifications.
     * @param liste La liste de modifications à afficher dans le tableau.
     */
    void MiseAJourTable(vector<Modification> liste);

signals:
    /**
     * @brief Signal émis pour fermer la fenêtre.
     * @param type L.
     */
    void fermer(string type);

private:
    Ui::ListeModifications *ui;

public slots:
    /**
     * @brief Slot qui Gère le clic sur le bouton "Fermer".
     */
    void Fermer();

    /**
     * @brief Slot qui Récupère la liste de modifications.
     * @param liste La liste de modifications à récupérer.
     */
    void recupListeModifications(vector<Modification> liste);

protected:
    /**
     * @brief Méthode surchargée qui détecte l'événement de fermeture de la fenêtre.
     * @param event L'événement de fermeture.
     */
    void closeEvent(QCloseEvent *event) override;

};

#endif // LISTEMODIFICATIONS_H
