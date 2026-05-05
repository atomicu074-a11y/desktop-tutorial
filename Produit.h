
#ifndef PRODUIT_H
#define PRODUIT_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Panier; 

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
    void retirerStock(int quantite);
    virtual double calculerPrixFinal() const = 0;
    virtual double calculerSimilarite(const Panier& p) const = 0;
    virtual void afficher() const;
    virtual string getType() const = 0;
};

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


#endif



























































































