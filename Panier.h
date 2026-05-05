#ifndef PANIER_H
#define PANIER_H

#include "Produit.h"
#include <vector>

class Panier {
private:
    vector<Produit*> articles;
public:
    void ajouterProduit(Produit* p);
    double calculerMontantTotal() const;
int getNbArticles() const;
    void afficherFacture() const;
};

#endif
