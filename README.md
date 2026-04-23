#include <iostream>
#include <string>
#include <vector>
#include <fstream>  
using namespace std;
class Panier; 
// ==========================================
// 1. CLASSE ABSTRAITE DE BASE : 
// ==========================================
class Produit {
protected: 
    string nom;
    double prixBase;
    int stock;

public:
    Produit(string n, double p, int s) : nom(n), prixBase(p), stock(s) {}
    virtual ~Produit() {} 

    string getNom() const { return nom; }
    int getStock() const { return stock; }
    void retirerStock(int quantite) {
        stock -= quantite;
        if (stock <= 0) {
            cout << "   [ALERTE] Rupture de stock pour [" << nom << "] !" << endl;
        }
    }
    virtual double calculerPrixFinal() const = 0; 
    virtual double calculerSimilarite(const Panier& p) const = 0; 
    virtual void afficher() const {
        cout << nom << " | Prix Base: " << prixBase << " EUR | Stock: " << stock;
    }
};
// ==========================================
// 2. CLASSES DÉRIVÉES (CATALOGUE) 
// ==========================================
class Electronique : public Produit {
public:
    Electronique(string n, double p, int s) : Produit(n, p, s) {}
    double calculerPrixFinal() const override { return prixBase * 1.20; } // TVA 20% 
    double calculerSimilarite(const Panier& p) const override { return 85.0; } // [cite: 34]
};

class Alimentaire : public Produit {
public:
    Alimentaire(string n, double p, int s) : Produit(n, p, s) {}
    double calculerPrixFinal() const override { return prixBase * 1.055; } // TVA 5.5% 
    double calculerSimilarite(const Panier& p) const override { return 30.0; } // [cite: 34]
};

class Vetement : public Produit {
public:
    Vetement(string n, double p, int s) : Produit(n, p, s) {}
    double calculerPrixFinal() const override { return prixBase * 1.10; } 
    double calculerSimilarite(const Panier& p) const override { return 60.0; } 
};

// ========================================
// 3. GESTION DU PANIER 
// ==========================================
class Panier {
private:
    Produit** articles;
    int nbArticles;
    int capaciteMax;

public:
    Panier(int cap) : nbArticles(0), capaciteMax(cap) {
        articles = new Produit*[capaciteMax]; 
    }

    ~Panier() { delete [] articles; }

    int getNbArticles() const { return nbArticles; }
    Produit* getArticle(int i) const { return articles[i]; }

    void ajouterProduit(Produit* p) {
        if (nbArticles < capaciteMax && p->getStock() > 0) {
            articles[nbArticles++] = p;
            p->retirerStock(1); 
            cout << "-> Ajoute: " << p->getNom() << endl;
        } else {
            cout << "-> Erreur: Stock épuisé ou Panier plein !" << endl;
        }
    }
    double calculerMontantTotal() const {
        double total = 0;
        for (int i = 0; i < nbArticles; i++) {
            total += articles[i]->calculerPrixFinal();
        }
        if (total > 500) total *= 0.95; 
        return total;
    }

    void afficherFacture() const {
        cout << "\n--- VOTRE FACTURE  ---" << endl;
        if (nbArticles == 0) {
            cout << "Panier vide." << endl;
            return;
        }
        for (int i = 0; i < nbArticles; i++) {
            cout << "- " << articles[i]->getNom() << " : " << articles[i]->calculerPrixFinal() << " DH" << endl;
        }
        cout << "TOTAL (TTC + Remises): " << calculerMontantTotal() << " DH" << endl;
    }
};

// ==========================================
// 4. PROGRAMME PRINCIPAL
// ==========================================
int main() {
    const int nbProduits = 3;
    Produit* catalogue[nbProduits]; 

    catalogue[0] = new Electronique("Smartphone X", 800.0, 2);
    catalogue[1] = new Alimentaire("Pommes Bio", 3.0, 10);
    catalogue[2] = new Vetement("T-Shirt C++", 25.0, 5);

    Panier monPanier(10);
    vector<string> historique;
    int choix = -1;
    void sauvegarderCatalogue(Produit* catalogue[], int taille) ;{
    ofstream fichier("catalogue.txt"); 
    if (fichier.is_open()) {
        for (int i = 0; i < nbProduits; i++) {
            fichier << catalogue[i]->getNom() << " " 
                    << " " << catalogue[i]->getStock() << endl;
        }
        fichier.close();
        cout << "   [OK] Catalogue enregistré avec succès !" << endl;
    }
}

    while (choix != 0) {
        cout << "\n=========== MENU E-COMMERCE  ===========" << endl;
        cout << "1. Voir le catalogue" << endl;
        cout << "2. Ajouter au panier" << endl;
        cout << "3. Voir facture et total" << endl;
        cout << "4. Recommandations (Challenge) " << endl;
        cout << "5. Voir l'historique des commandes " << endl;
        cout << "0. Valider et Quitter" << endl;
        cout << "Votre choix : ";
        cin >> choix;

        if (choix == 1) {
            for (int i = 0; i < nbProduits; i++) {
                cout << i + 1 << ". "; catalogue[i]->afficher(); cout << endl;
            }
        } 
        else if (choix == 2) {
            int id; cout << "ID (1-" << nbProduits << "): "; cin >> id;
            if (id >= 1 && id <= nbProduits) monPanier.ajouterProduit(catalogue[id-1]);
        } 
        else if (choix == 3) {
            monPanier.afficherFacture();
        } 
        else if (choix == 4) {
            cout << "\n--- RECOMMANDATIONS  ---" << endl;
            for (int i = 0; i < nbProduits; i++) {
                cout << "Score pour [" << catalogue[i]->getNom() << "] : " 
                     << catalogue[i]->calculerSimilarite(monPanier) << " %" << endl;
            }
        }
        else if (choix == 5) {
            cout << "\n--- HISTORIQUE  ---" << endl;
            for (const string& cmd : historique) cout << cmd << endl;
        }
        else if (choix == 0 && monPanier.getNbArticles() > 0) {
            historique.push_back("Commande de " + to_string(monPanier.calculerMontantTotal()) + " DH");
        }
    }

    for(int i = 0; i < nbProduits; i++) delete catalogue[i];
    return 0;
}
