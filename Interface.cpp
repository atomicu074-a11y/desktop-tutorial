#include "Interface.h"
#include <ftxui/component/screen_interactive.hpp>
#include <iomanip>
#include <sstream>

using namespace ftxui;
using namespace std;

Ui::Ui() : client_("invité") {
    // Palette Catppuccin
    rose_ = Color::RGB(245, 194, 231);
    mauve_ = Color::RGB(203, 166, 247);
    bleu_doux_ = Color::RGB(137, 180, 250);
    vert_clair_ = Color::RGB(166, 227, 161);
    fond_ = Color::RGB(30, 30, 46);
    surface_ = Color::RGB(49, 50, 68);
    alerte_ = Color::RGB(243, 139, 168);
}

void Ui::lancer() {
    auto screen = ScreenInteractive::FitComponent();

    // 1. Navigation Principale
    vector<string> menu_entries = {"💖 Client", "📦 Produits", "🛒 Panier", "📜 Historique"};
    int selected_tab = 0;
    auto menu_principal = Menu(&menu_entries, &selected_tab);

    // 2. Logique du Panier (Sélection et Inputs)
    vector<string> produits_liste;
    int produit_selectionne = 0;
    string saisie_qte = "1";
    auto menu_panier = Menu(&produits_liste, &produit_selectionne);
    auto input_qte = Input(&saisie_qte, "Qté");

    // 3. Boutons d'action
    auto btn_ajouter = Button("Ajouter au Panier", [&] {
        try {
            int qte = stoi(saisie_qte);
            int id_prod = magasin_.produits()[produit_selectionne].id();
            if(magasin_.ajouter_au_panier(client_.panier(), id_prod, qte, message_)) {
                saisie_qte = "1"; // Reset qte après succès
            }
        } catch (...) { message_ = "⚠ Quantité invalide"; }
    });

    auto btn_valider = Button("Valider Commande", [&] { action_valider_commande(); });

    // 4. Définition des Vues (Tabs)
    auto vue_client = Renderer([&] {
        return window(text(" Informations Client ") | bold | color(rose_),
                      vbox({
                          text("Connecté en tant que : " + client_.nom()) | color(bleu_doux_),
                          separator(),
                          text("Modifiez votre nom dans le code ou via Input") | dim
                      })) | bgcolor(surface_);
    });

    auto vue_produits = Renderer([&] { return render_produits(); });

    auto conteneur_panier = Container::Vertical({menu_panier, input_qte, btn_ajouter, btn_valider});
    auto vue_panier = Renderer(conteneur_panier, [&] {
        // Mise à jour dynamique de la liste des produits
        produits_liste.clear();
        for (auto& p : magasin_.produits()) {
            produits_liste.push_back(p.nom() + " (" + to_string(p.stock()) + " en stock)");
        }
        
        return vbox({
            window(text(" Sélection "), menu_panier->Render() | vscroll_indicator | frame | size(HEIGHT, EQUAL, 6)),
            hbox({text(" Quantité : "), input_qte->Render() | border}),
            hbox({btn_ajouter->Render(), filler(), btn_valider->Render()}),
            separator(),
            render_panier() // Affiche les totaux et le contenu actuel
        }) | bgcolor(surface_);
    });

    // 5. Assemblage final
    auto main_container = Container::Tab({vue_client, vue_produits, vue_panier, vue_client}, &selected_tab);
    auto main_renderer = Renderer(Container::Horizontal({menu_principal, main_container}), [&] {
        return vbox({
            text(" MI-AMAZON TUI ") | bold | center | color(mauve_),
            separator(),
            hbox({
                menu_principal->Render() | size(WIDTH, EQUAL, 20) | border,
                main_container->Render() | flex | border
            }) | flex,
            window(text(" Statut "), text(message_.empty() ? "Prêt" : message_) | color(vert_clair_))
        }) | bgcolor(fond_);
    });

    screen.Loop(main_renderer);
}

Element Ui::render_produits() {
    Elements rows;
    for (const auto& p : magasin_.produits()) {
        bool low = p.stock() <= 3;
        rows.push_back(hbox({
            text(p.nom()) | flex,
            separator(),
            text(to_string(p.prix()) + " DH") | color(vert_clair_),
            separator(),
            text(to_string(p.stock())) | color(low ? alerte_ : bleu_doux_)
        }));
    }
    return window(text(" Catalogue "), vbox(move(rows))) | bgcolor(surface_);
}

Element Ui::render_panier() {
    DetailPrix d = magasin_.calculer_prix(client_.panier());
    return window(text(" Résumé Facture "), vbox({
        text("Sous-total : " + to_string(d.sous_total) + " DH"),
        text("Remise     : -" + to_string(d.remise) + " DH") | color(alerte_),
        separator(),
        text("TOTAL TTC  : " + to_string(d.total_ttc) + " DH") | bold | color(vert_clair_)
    }));
}

void Ui::action_valider_commande() {
    if (client_.panier().vide()) {
        message_ = "⚠ Le panier est vide !";
        return;
    }
    DetailPrix d = magasin_.calculer_prix(client_.panier());
    
    // Logique de sauvegarde journalisée
    stringstream ss;
    ss << "Commande client [" << client_.nom() << "] - Total: " << d.total_ttc << " DH";
    magasin_.sauvegarder_journal(ss.str());

    client_.panier().vider();
    message_ = "📦 Commande validée et enregistrée !";
}


















