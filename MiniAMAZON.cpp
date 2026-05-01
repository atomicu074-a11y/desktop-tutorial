// bravo l'équipe 
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>

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

// Fonction utilitaire pour sécuriser les entiers
int saisirEntier(string message) {
    int n;
    while (true) {
        cout << message;
        if (cin >> n) return n;
        else {
            cout << "   [ERREUR] Saisie invalide ! Veuillez entrer un nombre." << endl;
            cin.clear();
            cin.ignore(1000, '\n'); 
        }
    }
}

Produit* creerNouveauProduit() {
    string nom;
    double prix;
    int stock, type;

    cout << "\n--- ESPACE VENDEUR ---" << endl;
    cout << "1. Electronique | 2. Alimentaire | 3. Vetement : ";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    //  Vérification que PRIX est un nombre
    while (true) {
        cout << "Prix : ";
        if (cin >> prix) {
            break; 
        } else {
            cout << "   [ERREUR] Le prix doit etre un nombre (ex: 10.5). Reessayez." << endl;
            cin.clear();
            cin.ignore(1000, '\n'); 
        }
    }

    // Vérification que STOCK est un entier
    while (true) {
        cout << "Stock : ";
        if (cin >> stock) {
            break; 
        } else {
            cout << "   [ERREUR] Le stock doit etre un chiffre entier. Reessayez." << endl;
            cin.clear(); 
            cin.ignore(1000, '\n');
        }
    }

    // Création de l'objet selon le type
    if (type == 1) return new Electronique(nom, prix, stock);
    if (type == 2) return new Alimentaire(nom, prix, stock);
    if (type == 3) {
        return new Vetement(nom, prix, stock);
    }
    
    return nullptr;
}
void compterItemsParCategorie(const vector<Produit*>& catalogue) {
    int nbElec = 0, nbAlim = 0, nbVet = 0;

    for (Produit* p : catalogue) {
        if (p->getType() == "Electronique") nbElec++;
        else if (p->getType() == "Alimentaire") nbAlim++;
        else if (p->getType() == "Vetement") nbVet++;
    }

    cout << "\n--- STATISTIQUES CATALOGUE ---" << endl;
    cout << "Electronique : " << nbElec << endl;
    cout << "Alimentaire  : " << nbAlim << endl;
    cout << "Vetements    : " << nbVet << endl;
    cout << "TOTAL        : " << catalogue.size() << " produits differents." << endl;
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
  
    
