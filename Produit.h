#ifndef PRODUIT_H
#define PRODUIT_H
#include <iostream>
#include <string>
using namespace std;

class Panier;
// ==========================================
// CLASSE PRODUIT 
// ==========================================
class Produit {
protected:
    string nom;
    double prixBase;
    int stock;
public:
    Produit(string n, double p, int s);
    virtual ~Produit();
    string getNom() const;
    int getStock() const;
    void retirerStock(int quantite);
    virtual double calculerPrixFinal() const = 0;
    virtual double calculerSimilarite(const Panier& p) const = 0;
    virtual void afficher() const;
};
class Electronique : public Produit {
public:
    Electronique(string n, double p, int s);
    double calculerPrixFinal() const override;
    double calculerSimilarite(const Panier& p) const override;
};
class Alimentaire : public Produit {
public:
    Alimentaire(string n, double p, int s);
    double calculerPrixFinal() const override;
    double calculerSimilarite(const Panier& p) const override;
};
class Vetement : public Produit {
public:
    Vetement(string n, double p, int s);
    double calculerPrixFinal() const override;
    double calculerSimilarite(const Panier& p) const override;
};
#endif















































































































