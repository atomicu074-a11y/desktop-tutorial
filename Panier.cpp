#include "Panier.h"
#include <iostream>

using namespace std;
void Panier::ajouterProduit(Produit* p) {
    if (p->getStock() > 0) {
        articles.push_back(p);
        p->retirerStock(1);
        cout << "-> Ajoute: " << p->getNom() << endl;
    } else {
        cout << "-> Erreur: Stock épuisé !" << endl;
    }
}
double Panier::calculerMontantTotal() const {
    double total = 0;
    for (Produit* p : articles) {
        total += p->calculerPrixFinal();
    }
    if (total > 500) total *= 0.95; 
    return total;
}
int Panier::getNbArticles() const { 
    return articles.size(); 
}
void Panier::afficherFacture() const {
    cout << "\n--- VOTRE FACTURE ---" << endl;
    if (articles.empty()) {
        cout << "Panier vide." << endl;
        return;
    }
    for (Produit* p : articles) {
        cout << "- " << p->getNom() << " : " << p->calculerPrixFinal() << " DH" << endl;
    }
    cout << "TOTAL (TTC + Remises): " << calculerMontantTotal() << " DH" << endl;
}
