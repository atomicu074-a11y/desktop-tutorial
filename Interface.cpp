#include "Interface.h"
#include <ftxui/component/screen_interactive.hpp>
#include <iomanip>
#include <sstream>

using namespace ftxui;
using namespace std;

// Constructeur : Initialisation du client par défaut
Ui::Ui() : client_("invité") {
    // Couleurs de l'interface 
    rose_ = Color::RGB(245, 194, 231);
    mauve_ = Color::RGB(203, 166, 247);
    bleu_doux_ = Color::RGB(137, 180, 250);
    vert_clair_ = Color::RGB(166, 227, 161);
    fond_ = Color::RGB(30, 30, 46);
    surface_ = Color::RGB(49, 50, 68);
    alerte_ = Color::RGB(243, 139, 168);
    accent_ = Color::RGB(250, 179, 135);
}

// Fonction auxiliaire pour l'affichage de l'historique
Element Ui::render_historique() {
    Elements lignes;
    lignes.push_back(text(" Historique des transactions") | bold | color(mauve_));
    lignes.push_back(separator());
    lignes.push_back(text(" [Consultez le fichier journal.txt pour le détail] ") | dim);
    lignes.push_back(text(" ---------------------------------------------- ") | dim);
    return window(text(" Historique "), vbox(move(lignes))) | bgcolor(surface_);
}

// Fonction auxiliaire pour l'affichage du catalogue produits
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

// Fonction auxiliaire pour l'affichage du résumé financier du panier
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

// Lancement principal de l'application
void Ui::lancer() {
    auto screen = ScreenInteractive::TerminalOutput();

    //Composition de menu
    int selected_tab = 0;
    vector<string> menu_entries = {"💖 Client", "📦 Produits", "🛒 Panier", "📜 Historique", "🛠 Admin"};
    auto menu_principal = Menu(&menu_entries, &selected_tab);

    // L'onglet Client
    string saisie_nom_client = "";
    auto input_nom_client = Input(&saisie_nom_client, "Entrez votre nom ici...");

    // L'onglet Panier
    vector<string> produits_liste;
    int produit_selectionne = 0;
    auto menu_panier = Menu(&produits_liste, &produit_selectionne);

    string saisie_qte = "1";
    auto input_qte = Input(&saisie_qte, "Qté");

    // L'onglet Admin
    string nouv_nom = "";
    string nouv_prix = "0.0";
    string nouv_stock = "0";

    auto input_nom = Input(&nouv_nom, "Nom du produit");
    auto input_prix = Input(&nouv_prix, "Prix");
    auto input_stock = Input(&nouv_stock, "Stock");

    //boutons d'action
    auto btn_vider = Button("Vider Panier", [&] {
        client_.panier().vider();
        message_ = "🗑 Panier vidé avec succès";
    });

    auto btn_ajouter = Button("Ajouter au Panier", [&] {
        try {
            int qte = stoi(saisie_qte);
            
            if (qte <= 0) {
                message_ = "⚠ La quantité doit être supérieure à 0 et positive  !";
                return;
            }

            if (magasin_.produits().empty() || produit_selectionne >= magasin_.produits().size()) {
                message_ = "⚠ Aucun produit sélectionné";
                return;
            }

            int id_prod = magasin_.produits()[produit_selectionne].id();
            if (magasin_.ajouter_au_panier(client_.panier(), id_prod, qte, message_)) {
                saisie_qte = "1"; 
            }
        } catch (...) { 
            message_ = "⚠ Erreur : veuillez saisir un nombre entier valide"; 
        }
    });

    auto btn_valider = Button("Valider Commande", [&] {
        action_valider_commande();
    });

    auto btn_ajouter_admin = Button("Ajouter au Catalogue", [&] {
        try {
            double p = stod(nouv_prix);
            int s = stoi(nouv_stock);
            if (!nouv_nom.empty() && p > 0 && s >= 0) {
                ProduitElectronique prod(magasin_.produits().size() + 1, nouv_nom, "informatique", p, s);
                verifier_et_ajouter(prod);
                
                // Réinitialisation du formulaire
                nouv_nom = ""; 
                nouv_prix = "0.0"; 
                nouv_stock = "0";
            } else {
                message_ = "⚠ Données invalides (Le prix doit être supérieur à 0)";
            }
        } catch (...) {
            message_ = "⚠ Erreur de saisie dans les champs numériques";
        }
        screen.RequestAnimationFrame(); 
    });

    // Rendu de l'onglet Client
    auto vue_client = Renderer(input_nom_client, [&] {
        if (!saisie_nom_client.empty()) {
            client_.set_nom(saisie_nom_client);
        } else {
            client_.set_nom("invité");
        }
        return window(text(" Informations Client ") | bold | color(accent_),
            vbox({
                text(" Connecté en tant que : " + client_.nom()) | color(bleu_doux_),
                separator(),
                hbox(text(" Votre Nom : ") | size(WIDTH, EQUAL, 12), input_nom_client->Render() | border),
                separator(),
                text(" Statut : Prêt à commander ") | dim | italic
            })
        ) | bgcolor(surface_);
    });

    // Rendu de l'onglet Produits
    auto vue_produits = Renderer([&] { return render_produits(); });

    // Rendu de l'onglet Panier
    auto conteneur_panier = Container::Vertical({menu_panier, input_qte, btn_ajouter, btn_vider, btn_valider});
    auto vue_panier = Renderer(conteneur_panier, [&] {
        produits_liste.clear();
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
            render_panier()
        }) | bgcolor(surface_);
    });

    // Rendu de l'onglet Historique
    auto vue_historique = Renderer([&] { return render_historique(); });

    // Rendu de l'onglet Administration
    auto conteneur_admin = Container::Vertical({input_nom, input_prix, input_stock, btn_ajouter_admin});
    auto vue_admin = Renderer(conteneur_admin, [&] {
        return window(text(" [Zone Sécurisée] Ajouter un Produit au Catalogue ") | bold | color(accent_), 
            vbox({
                hbox(text(" Nom : ") | size(WIDTH, EQUAL, 10), input_nom->Render() | border),
                hbox(text(" Prix : ") | size(WIDTH, EQUAL, 10), input_prix->Render() | border),
                hbox(text(" Stock : ") | size(WIDTH, EQUAL, 10), input_stock->Render() | border),
                separator(),
                vbox({ btn_ajouter_admin->Render() | center }), 
                separator(),
                text(" Note : Cette action requiert le rôle 'Administrateur'. ") | dim | italic
            })
        ) | bgcolor(surface_);
    });

    // AGENCEMENT GLOBAL ET STRUCTURE DE LA BOUCLE 
    auto main_container = Container::Tab({vue_client, vue_produits, vue_panier, vue_historique, vue_admin}, &selected_tab);

    auto main_renderer = Renderer(Container::Horizontal({menu_principal, main_container}), [&] {
        return vbox({
            text(" boutique en ligne ") | bold | center | color(rose_),
            hbox({
                window(text("navigation") | color(bleu_doux_), menu_principal->Render()) | size(WIDTH, EQUAL, 25),
                window(text(menu_entries[selected_tab]) | color(mauve_), main_container->Render() | flex)
            }) | flex,
            window(text("messages") | color(vert_clair_),
                   text(message_.empty() ? "pret" : message_) 
                   | color(message_.find("⚠") != string::npos ? alerte_ : vert_clair_)
            ) | size(HEIGHT, EQUAL, 5),
            text("realise par MDL") | center | dim
        }) | bgcolor(fond_) | border;
    });

    screen.Loop(main_renderer);
}

// Validation comptable et sauvegarde de la facture
void Ui::action_valider_commande() {
    if (client_.panier().vide()) {
        message_ = "⚠ Le panier est vide !";
        return;
    }

    DetailPrix d = magasin_.calculer_prix(client_.panier());
    stringstream ss;

    ss << "========================================\n";
    ss << " COMMANDE CLIENT : " << client_.nom() << "\n";
    ss << "========================================\n";
    ss << "Détail des articles :\n";

    for (const auto& ligne : client_.panier().lignes()) {
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

    magasin_.sauvegarder_journal(ss.str());
    client_.panier().vider();
    message_ = "📦 Commande validée et enregistrée !";
}

// Fonction métier d'ajout de produit
void Ui::verifier_et_ajouter(const ProduitElectronique& nouveau_produit) { 
    magasin_.produits().push_back(nouveau_produit);
    message_ = "💾 Nouveau produit ajouté au catalogue avec succès !";
}






