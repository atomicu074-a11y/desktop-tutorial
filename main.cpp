// ==========================================
//  PROGRAMME PRINCIPAL
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









































































