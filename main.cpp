#include <iostream>
#include <string>
#include <vector>
#include "Produit.h"      
#include "Panier.h"    

using namespace std;  
int main() {
    vector<Produit*> catalogue;
    catalogue.push_back(new Electronique("Smartphone X", 800.0, 2));
    catalogue.push_back(new Alimentaire("Pommes Bio", 3.0, 10));
    catalogue.push_back(new Vetement("T-Shirt ", 25.0, 5));

    Panier monPanier;
    vector<string> historique;
    int choix = -1;

    while (choix != 0) {
         cout << "\n=========== MENU E-COMMERCE ===========" << endl;
        cout << "1. Voir catalogue\n2. Ajouter au panier\n3. Voir facture\n4. Recommandations\n5. ESPACE VENDEUR\n6. Historique\n0. Quitter" << endl;
        cout << "Votre choix : ";
        while (!(cin >> choix)) {
        cout << "[ERREUR] Saisie invalide !" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');//propleme cant hna :)
        cout << "\n--- Choisissez un nombre entre 0 et 6 ---\n1. Voir catalogue\n2. Ajouter au panier\n3. Voir facture\n4. Recommandations\n5. ESPACE VENDEUR\n6. Historique\n0. Quitter" << endl;
        }
        if (choix == 1) {
        cout << "\n--- CATALOGUE COMPLET ---" << endl;
            if (catalogue.empty()) {
            cout << "Le catalogue est vide." << endl;
            } else {
                for (size_t i = 0; i < catalogue.size(); ++i) {
                    cout << "[" << i + 1 << "] "; 
                    catalogue[i]->afficher(); 
                    cout << endl;
                }
            }
        }
        else if (choix == 2) {
        int id;
        cout << "Entrez le numero du produit a ajouter : ";
    
    // Protection contre les caractères non-numériques
        if (!(cin >> id)) {
        cout << "   [ERREUR] Veuillez entrer un chiffre !" << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        } 
    // Vérification si l'ID existe dans le vecteur
        else if (id >= 1 && (size_t)id <= catalogue.size()) {
        monPanier.ajouterProduit(catalogue[id - 1]);
        } 
        else {
            cout << "   [ERREUR] Ce numero n'existe pas dans le catalogue." << endl;
        }
    } 
        else if (choix == 3) monPanier.afficherFacture();
        else if (choix == 4) {
            cout << "\n--- RECOMMANDATIONS ---" << endl;
            for (Produit* p : catalogue) {
                cout << "Score [" << p->getNom() << "] : " << p->calculerSimilarite(monPanier) << " %" << endl;
            }
        }
        else if (choix == 5) {
        Produit* nouveau = creerNouveauProduit();
        if (nouveau) catalogue.push_back(nouveau);
        }
        else if (choix == 6) {
            for (const string& s : historique) cout << s << endl;
        }
        else if (choix == 0 && monPanier.getNbArticles() > 0) {
            historique.push_back("Commande de " + to_string(monPanier.calculerMontantTotal()) + " DH");
            sauvegarderCatalogue(catalogue);
        }
       
        
    }for(Produit* p : catalogue) delete p;
    return 0;
}
  
    







































































