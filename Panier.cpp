#include "Panier.h"

void Panier::ajouterProduit(Produit* p) {
    if (p->getStock() > 0) {
        articles.push_back(p);
        p->retirerStock(1);
        cout << "-> Ajoute: " << p->getNom() << endl;
    } else {
        cout << "-> Erreur: Stock épuisé !" << endl;
    }
}
