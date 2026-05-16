#include "Interface.h"
#include <ftxui/component/screen_interactive.hpp>
#include <iomanip>
#include <sstream>

using namespace ftxui;
using namespace std;

// constructeur
Ui::Ui() : client_("invité") {

   // couleurs utilisées dans l’interface 
    rose_ = Color::RGB(245, 194, 231);
    mauve_ = Color::RGB(203, 166, 247);
    bleu_doux_ = Color::RGB(137, 180, 250);
    vert_clair_ = Color::RGB(166, 227, 161);
    fond_ = Color::RGB(30, 30, 46);
    surface_ = Color::RGB(49, 50, 68);
    alerte_ = Color::RGB(243, 139, 168);
    accent_ = Color::RGB(250, 179, 135);
}

//fonction pour afficher l'historique 
Element Ui::render_historique() {
    Elements lignes;
    lignes.push_back(text(" Historique des transactions") | bold | color(mauve_));
    lignes.push_back(separator());
    lignes.push_back(text(" [Consultez le fichier journal.txt pour le détail] ") | dim);
    lignes.push_back(text(" ---------------------------------------------- ") | dim);
    return window(text(" Historique "), vbox(move(lignes))) | bgcolor(surface_);
}



void Ui::lancer() {

    // lancement de l’interface
    auto screen = ScreenInteractive::FitComponent();
    
    // menu principal 
    vector<string> menu_entries = {"💖 Client", "📦 Produits", "🛒 Panier", "📜 Historique"};
    int selected_tab = 0;
    auto menu_principal = Menu(&menu_entries, &selected_tab);

    vector<string> produits_liste;
    int produit_selectionne = 0;

       // quantité choisie
    string saisie_qte = "1";
    auto menu_panier = Menu(&produits_liste, &produit_selectionne);
    auto input_qte = Input(&saisie_qte, "Qté");

    // vider le panier
    auto btn_vider = Button("Vider Panier", [&] {
        client_.panier().vider();
        message_ = "🗑 Panier vidé avec succès";
    });

   // ajout d'un produit
    auto btn_ajouter = Button("Ajouter au Panier", [&] {
        try {
            int qte = stoi(saisie_qte);
            int id_prod = magasin_.produits()[produit_selectionne].id();
            if(magasin_.ajouter_au_panier(client_.panier(), id_prod, qte, message_)) {
                saisie_qte = "1"; 
            }
        } catch (...) { message_ = "⚠ Quantité invalide"; }
    });

    // validation de la commande
    auto btn_valider = Button("Valider Commande", [&] { action_valider_commande(); }
);

    // informations du client
    auto vue_client = Renderer([&] {
        return window(text(" Informations Client "),
                      vbox({
                          text("Connecté en tant que : " + client_.nom()) | color(bleu_doux_),
                          separator(),
                          text("Statut : Prêt à commander") | dim
                      })) | bgcolor(surface_);
    });
    
     // affichage des produits 
    auto vue_produits = Renderer([&] { return render_produits(); });

    auto conteneur_panier = Container::Vertical({menu_panier, input_qte, btn_ajouter, btn_vider, btn_valider});

 // partie panier  
auto vue_panier = Renderer(conteneur_panier, [&] {
        produits_liste.clear();

         // mise à jour de la liste des produits
        for (auto& p : magasin_.produits()) {
            produits_liste.push_back(p.nom() + " (" + to_string(p.stock()) + " en stock)");
        }
        
        return vbox({
            window(text(" Sélectionner un produit "), menu_panier->Render() | vscroll_indicator | frame | size(HEIGHT, EQUAL, 6)),
            hbox({text(" Quantité : "), input_qte->Render() | border}),
            hbox({
                btn_ajouter->Render() | border, 
                btn_vider->Render() | border, 
                filler(), 
                btn_valider->Render() | border
            }),
            separator(),

            // résumé du panier
            render_panier()
        }) | bgcolor(surface_);
    });

    auto vue_historique = Renderer([&] { return render_historique(); });

    // navigation entre les pages
    auto main_container = Container::Tab(Components{vue_client, vue_produits, vue_panier, vue_historique}, &selected_tab);

     // affichage principal
    auto main_renderer = Renderer(Container::Horizontal(Components{menu_principal, main_container}), [&] {
        return vbox(Elements{
            text(" boutique en ligne ") | bold | center | color(rose_),
            hbox(Elements{

                  // menu de navigation 
                window(text("navigation") | color(bleu_doux_), menu_principal->Render()) | size(WIDTH, EQUAL, 25),

                // contenu principal
                window(text(menu_entries[selected_tab]) | color(mauve_), main_container->Render() | flex)
            }) | flex,

            // messages affichés à l'utilisateur 
            window(text("messages") | color(vert_clair_),
                   text(message_.empty() ? "pret" : message_) 
                   | color(message_.find("⚠") != std::string::npos ? alerte_ : vert_clair_)
            ) | size(HEIGHT, EQUAL, 5),
            text("realise par MDL") | center | dim
        }) | bgcolor(fond_) | border;
    });

      // boucle principale
    screen.Loop(main_renderer);
}


  // boucle principale 
Element Ui::render_produits() {
    Elements rows;
    for (const auto& p : magasin_.produits()) {
        stringstream ss;
        ss << fixed << setprecision(2) << p.prix() << " DH";
        rows.push_back(hbox({
            text("#" + to_string(p.id()) + " " + p.nom()),
            filler(),
            text(ss.str()) | color(vert_clair_),
            text(" | stock " + to_string(p.stock())) | color(p.stock() <= 3 ? alerte_ : bleu_doux_)
        }));
    }
    return window(text(" Catalogue "), vbox(move(rows))) | bgcolor(surface_);
}


// résumé de la facture
Element Ui::render_panier() {
    DetailPrix d = magasin_.calculer_prix(client_.panier());
    auto ligne_style = [](string label, double val, Color col) {
        stringstream ss;
        ss << fixed << setprecision(2) << val << " DH";
        return hbox({ text(label), filler(), text(ss.str()) }) | color(col);
    };

    return window(text(" Résumé Facture "), vbox({
        ligne_style("sous-total : ", d.sous_total, bleu_doux_),
        ligne_style("remise 10% : ", -d.remise, accent_),
        ligne_style("tva 20%    : ", d.tva, mauve_),
        separator(),
        ligne_style("total ttc  : ", d.total_ttc, vert_clair_) | bold
    }));
}


// validation de la commande
  void Ui::action_valider_commande() {
    // 1. Vérifier si le panier est vide
    if (client_.panier().vide()) {
        message_ = "⚠ Le panier est vide !";
        return;
    }

    DetailPrix d = magasin_.calculer_prix(client_.panier());
    stringstream ss;

    // 2. Construction d'un ticket de caisse détaillé
    ss << "========================================\n";
    ss << " COMMANDE CLIENT : " << client_.nom() << "\n";
    ss << "========================================\n";
    ss << "Détail des articles :\n";

    // On boucle sur les lignes du panier
    for (const auto& ligne : client_.panier().lignes()) {
        // cherche le produit dans le magasin pour avoir son nom et son prix unitaire
        const Produit* p = magasin_.trouver(ligne.produit_id);
        if (p) {
            ss << "- " << p->nom() 
               << " (x" << ligne.quantite << ")"
               << " : " << fixed << setprecision(2) << (p->prix() * ligne.quantite) << " DH\n";
        }
    }

    ss << "----------------------------------------\n";
    ss << "Sous-total : " << d.sous_total << " DH\n";
    if (d.remise > 0) {
        ss << "Remise 10% : -" << d.remise << " DH\n";
    }
    ss << "TVA 20%    : " << d.tva << " DH\n";
    ss << "TOTAL TTC  : " << d.total_ttc << " DH\n"; 
    ss << "========================================\n\n";

    // 3. Sauvegarde de la chaîne complète  dans le journal
    magasin_.sauvegarder_journal(ss.str());

    // 4. Vider le panier après la validation
    client_.panier().vider();
    message_ = "📦 Commande validée et enregistrée !";
}












