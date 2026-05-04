#include "Produit.h"

void Produit::retirerStock(int quantite) {
    stock -= quantite;
    if (stock <= 0) {
        cout << "    [ALERTE] Rupture de stock pour [" << nom << "] !" << endl;
    }
}

void Produit::afficher() const {
    cout << nom << " | Prix Base: " << prixBase << " DH | Stock: " << stock;
}
