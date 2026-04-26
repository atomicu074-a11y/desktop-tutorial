

// bravo l'équipe 
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Panier;

// classe de base
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
        cout << nom << " | Prix Base: " << prixBase << " DH | Stock: " << stock;
    }
    virtual string getType() const = 0;
};

//classe de items
class Electronique : public Produit {
public:
    Electronique(string n, double p, int s) : Produit(n, p, s) {}
    double calculerPrixFinal() const override { return prixBase * 1.20; }
    double calculerSimilarite(const Panier& p) const override { return 85.0; }
    string getType() const override { return "Electronique"; }
};

class Alimentaire : public Produit {
public:
    Alimentaire(string n, double p, int s) : Produit(n, p, s) {}
    double calculerPrixFinal() const override { return prixBase * 1.055; }
    double calculerSimilarite(const Panier& p) const override { return 30.0; }
    string getType() const override { return "Alimentaire"; }
    
};

class Vetement : public Produit {
public:
    Vetement(string n, double p, int s) : Produit(n, p, s) {}
    double calculerPrixFinal() const override { return prixBase * 1.10; }
    double calculerSimilarite(const Panier& p) const override { return 60.0; }
    string getType() const override { return "Vetement"; }
};

// gestion de panier
class Panier {
private:
    vector<Produit*> articles; 

public:
    void ajouterProduit(Produit* p) {
        if (p->getStock() > 0) {
            articles.push_back(p);
            p->retirerStock(1);
            cout << "-> Ajoute: " << p->getNom() << endl;
        } else {
            cout << "-> Erreur: Stock épuisé !" << endl;
        }
    }

    double calculerMontantTotal() const {
        double total = 0;
        for (Produit* p : articles) {
            total += p->calculerPrixFinal();
        }
        if (total > 500) total *= 0.95; 
        return total;
    }

    int getNbArticles() const { return articles.size(); }

    void afficherFacture() const {
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
};

//des fonctions cree un produit
Produit* creerNouveauProduit() {
    string nom; double prix; int stock, type;
    cout << "\n--- ESPACE VENDEUR ---" << endl;
    cout << "1. Electronique | 2. Alimentaire | 3. Vetement : ";
    cin >> type;
    cout << "Nom : "; cin.ignore(); getline(cin, nom);
    cout << "Prix : "; cin >> prix;
    cout << "Stock : "; cin >> stock;

    if (type == 1) return new Electronique(nom, prix, stock);
    if (type == 2) return new Alimentaire(nom, prix, stock);
    if (type == 3) return new Vetement(nom, prix, stock);
    return nullptr;
}

void sauvegarderCatalogue(const vector<Produit*>& catalogue) {
    ofstream fichier("catalogue.txt");
    if (fichier.is_open()) {
        for (Produit* p : catalogue) {
            fichier << p->getNom() << " " << p->getStock() << endl;
        }
        fichier.close();
        cout << "   [OK] Catalogue enregistré !" << endl;
    }
}

// programme principale
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
        cout << "1. Voir catalogue\n2. Ajouter au panier\n3. Voir facture\n4. Recommandations\n5. ESPACE VENDEUR\n6. Historique\n0. Quitter" << endl;
        cout << "Votre choix : "; cin >> choix;

        if (choix == 1) {
            int cat;
            cout << "Choisir catégorie (1: Electronique, 2: Alimentaire, 3: Vetement) : ";
            cin >> cat;
            string cible = (cat == 1) ? "Electronique" : (cat == 2) ? "Alimentaire" : "Vetement";

    cout << "\n--- AFFICHAGE : " << cible << " ---" << endl;
    for (Produit* p : catalogue) {
        if (p->getType() == cible) {
            p->afficher();
            cout << endl;
        }
    }
        } 
        else if (choix == 2) {
            int id; cout << "ID (1-" << catalogue.size() << "): "; cin >> id;
            if (id >= 1 && (size_t)id <= catalogue.size()) monPanier.ajouterProduit(catalogue[id-1]);
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

    
