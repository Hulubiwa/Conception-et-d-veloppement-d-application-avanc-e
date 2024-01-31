#ifndef MODIFIERCONTACT_H
#define MODIFIERCONTACT_H

#include <QWidget>
#include <Contact.h>

namespace Ui {
class ModifierContact;
}

/**
 * @class ModifierContact
 * @brief La classe ModifierContact gère la modification des informations d'un contact.
 */
class ModifierContact : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe ModifierContact.
     * @param parent Le widget parent (par défaut : nullptr).
     */
    explicit ModifierContact(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe ModifierContact.
     */
    ~ModifierContact();

    /**
     * @brief Vérifie la validité du chemin de la photo.
     * @param image Le chemin de l'image à vérifier.
     * @return true si l'image est valide, sinon false.
     */
    bool VerifPhoto(string image);

signals:
    /**
     * @brief Signal émis pour envoyer les modifications du contact.
     * @param nouveauContact Le contact avec les modifications.
     */
    void EnvoiModifications(Contact nouveauContact);

    /**
     * @brief Signal émis pour envoyer l'enregistrement.
     * @param type Le type de l'enregistrement.
     * @param Id L'identifiant de l'enregistrement.
     */
    void EnvoiEnregistrement(string type, string Id);

    /**
     * @brief Signal émis pour fermer la fenêtre courante.
     * @param type Permet d'identifier le la fenêtre qui envoie la le signal.
     */
    void fermer(string type);

private:
    Ui::ModifierContact *ui;
    Contact contact; /**< Contact dont on souhaite modifier les données */

public slots:
    /**
     * @brief Gère le clic sur le bouton "Valider".
     */
    void PBValider_clicked();

    /**
     * @brief Récupère les informations du contact à modifier.
     * @param c Le contact à modifier.
     */
    void recupContact(Contact c);

    /**
     * @brief Gère le clic sur le bouton "Annuler".
     */
    void PBAnnuler_clicked();

protected:
    /**
     * @brief Méthode surchargée qui détecte l'événement de fermeture de la fenêtre.
     * @param event L'événement de fermeture.
     */
    void closeEvent(QCloseEvent *event) override;
};

#endif // MODIFIERCONTACT_H
