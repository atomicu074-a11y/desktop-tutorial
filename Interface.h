#ifndef INTERFACE_H
#define INTERFACE_H

#include "Magasin.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

class Ui {
public:
    Ui();
    void lancer();

private:
    // Méthodes de rendu
    ftxui::Element render_produits();
    ftxui::Element render_panier();
    void action_valider_commande();

    // Données du magasin
    Magasin magasin_;
    Client client_;
    std::string message_;

    ftxui::Color rose_;
    ftxui::Color mauve_;
    ftxui::Color bleu_doux_;
    ftxui::Color vert_clair_;
    ftxui::Color fond_;
    ftxui::Color surface_;
    ftxui::Color alerte_;
    ftxui::Color accent_; 
};

#endif
