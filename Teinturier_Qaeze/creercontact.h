#ifndef CREERCONTACT_H
#define CREERCONTACT_H

#include <QWidget>
#include <Contact.h>

namespace Ui {
class CreerContact;
}

/**
 * @class CreerContact
 * @brief La classe CreerContact permet de créer et de gérer un formulaire de création de contact.
 */
class CreerContact : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe CreerContact.
     * @param parent Le widget parent (par défaut : nullptr).
     */
    explicit CreerContact(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe CreerContact.
     */
    ~CreerContact();

    /**
     * @brief Initialise les éléments du formulaire.
     */
    void Initialisation();

    /**
     * @brief Vérifie le chemin d'accès à la photo du contact.
     * @param image Le chemin d'accès à l'image.
     * @return Vrai si le chemin d'accès à l'image est valide, sinon faux.
     */
    bool VerifPhoto(string image);

signals:
    /**
     * @brief Signal émis pour valider un nouveau contact.
     * @param c Le contact à créer.
     */
    void validerContact(Contact c);

    /**
     * @brief Signal émis pour fermer CreerContact.
     * @param type L'identifiant de la QWidget.
     */
    void fermer(string type);

private:
    Ui::CreerContact *ui;

public slots:
    /**
     * @brief Slot qui vérifie les informations du contact avant validation.
     */
    void verifierContact();

    /**
     * @brief Slot qui annule la création du contact.
     */
    void Annuler();

protected:
    /**
     * @brief Méthode surchargée qui détecte l'événement de fermeture de la fenêtre.
     * @param event L'événement de fermeture.
     */
    void closeEvent(QCloseEvent *event) override;
};

#endif // CREERCONTACT_H
