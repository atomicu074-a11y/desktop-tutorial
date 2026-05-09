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

    Magasin magasin_;
    Client client_;
    std::string message_;
    // Couleurs Catppuccin
    ftxui::Color rose_, mauve_, bleu_doux_, vert_clair_;
};

#endif
