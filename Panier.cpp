#include "Panier.h"
#include <iostream>
using namespace std;

Panier::Panier(int cap) : nbArticles(0), capaciteMax(cap) {
    articles = new Produit*[capaciteMax];
}

Panier::~Panier() { delete [] articles; }

int Panier::getNbArticles() const { return nbArticles; }
Produit* Panier::getArticle(int i) const { return articles[i]; }

void Panier::ajouterProduit(Produit* p) {
    if (nbArticles < capaciteMax && p->getStock() > 0) {
        articles[nbArticles++] = p;
        p->retirerStock(1);
        cout << "-> Ajoute: " << p->getNom() << endl;
    } else {
        cout << "-> Erreur: Stock épuisé ou Panier plein !" << endl;
    }
}

double Panier::calculerMontantTotal() const {
    double total = 0;
    for (int i = 0; i < nbArticles; i++) {
        total += articles[i]->calculerPrixFinal();
    }
    if (total > 500) total *= 0.95;
    return total;
}

void Panier::afficherFacture() const {
    cout << "\n--- VOTRE FACTURE ---" << endl;
    if (nbArticles == 0) {
        cout << "Panier vide." << endl;
        return;
    }
    for (int i = 0; i < nbArticles; i++) {
        cout << "- " << articles[i]->getNom() << " : "
             << articles[i]->calculerPrixFinal() << " DH" << endl;
    }
    cout << "TOTAL (TTC + Remises): "
         << calculerMontantTotal() << " DH" << endl;
}
