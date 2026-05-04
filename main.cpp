#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "Produit.h"   
#include "Panier.h"  

using namespace std;

// FONCTIONS UTILITAIRES                          
int saisirEntier(string message) {
    int n;
    while (true) {
        cout << message;
        if (cin >> n) return n;
        else {
            cout << "   [ERREUR] Saisie invalide ! Veuillez entrer un nombre." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

Produit* creerNouveauProduit() {
    string nom;
    double prix;
    int stock, type;

    cout << "\n--- ESPACE VENDEUR ---" << endl;
    type = saisirEntier("1. Electronique | 2. Alimentaire | 3. Vetement : ");
    
    cin.ignore(); // Nettoie le buffer avant le getline
    cout << "Nom : "; getline(cin, nom);
    
    // Validation du prix
    while (true) {
        cout << "Prix : ";
        if (cin >> prix) break;
        else {
            cout << "   [ERREUR] Entrez un nombre pour le prix." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    stock = saisirEntier("Stock : ");

    if (type == 1) return new Electronique(nom, prix, stock);
    if (type == 2) return new Alimentaire(nom, prix, stock);
    if (type == 3) return new Vetement(nom, prix, stock);
    
    return nullptr;
}

// PROGRAMME PRINCIPAL 

int main() {
    vector<Produit*> catalogue;
    catalogue.push_back(new Electronique("Smartphone X", 800.0, 2));
    catalogue.push_back(new Alimentaire("Pommes Bio", 3.0, 10));
    catalogue.push_back(new Vetement("T-Shirt C++", 25.0, 5));

    Panier monPanier;
    vector<string> historique;
    int choix = -1;

    while (choix != 0) {
        cout << "\n=========== MENU E-COMMERCE ===========" << endl;
        cout << "1. Voir catalogue\n2. Ajouter au panier\n3. Voir facture\n4. Recommandations\n5. ESPACE VENDEUR\n0. Quitter" << endl;
        
        choix = saisirEntier("Votre choix : ");

        if (choix == 1) {
            cout << "\n--- CATALOGUE COMPLET ---" << endl;
            for (size_t i = 0; i < catalogue.size(); ++i) {
                cout << "[" << i + 1 << "] ";
                catalogue[i]->afficher();
                cout << endl;
            }
        }
        else if (choix == 2) {
            int id = saisirEntier("Numero du produit : ");
            if (id >= 1 && (size_t)id <= catalogue.size()) {
                monPanier.ajouterProduit(catalogue[id - 1]);
            } else {
                cout << "   [ERREUR] ID invalide." << endl;
            }
        }
        else if (choix == 3) {
            monPanier.afficherFacture();
        }
        else if (choix == 5) {
            Produit* n = creerNouveauProduit();
            if (n) catalogue.push_back(n);
        }
        }
    for (Produit* p : catalogue) delete p;
    
    return 0;
}










