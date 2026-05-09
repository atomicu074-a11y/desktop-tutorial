#include "Magasin.h"
#include <fstream>
#include <iomanip>

Produit::Produit(int id, std::string nom, std::string cat, double p, int s) 
    : id_(id), nom_(nom), categorie_(cat), prix_(p), stock_(s) {}

Magasin::Magasin() {
    produits_.push_back(Produit(1, "ordinateur portable", "informatique", 899.99, 5));
    // ... Ajoutez vos autres produits ici ...
}

void Magasin::sauvegarder_journal(const std::string& log) {
    std::ofstream fichier("journal.txt", std::ios::app);
    if (fichier.is_open()) {
        fichier << log << std::endl;
        fichier.close();
    }
}
// Implémentez les autres méthodes (trouver, calculer_prix...) ici
