# 🛒 Mini E-Commerce en C++

## 📌 Description

Ce projet est une application e-commerce développée en C++ moderne.
L’application permet de gérer un catalogue de produits, un panier d’achat,
les commandes des clients ainsi qu’un historique des achats.

Le projet utilise la programmation orientée objet (POO),
la bibliothèque STL et une interface utilisateur terminal moderne avec FTXUI.

---

# 🚀 Fonctionnalités

✅ Gestion du catalogue produits  
✅ Gestion du stock  
✅ Ajout au panier  
✅ Calcul automatique des prix  
✅ Calcul TVA et remises  
✅ Validation des commandes  
✅ Historique des commandes  
✅ Interface utilisateur interactive  
✅ Gestion des erreurs utilisateur  

---

# 🧱 Architecture du Projet

```text
.
├── CMakeLists.txt
├── main.cpp
├── Interface.cpp
├── Interface.h
├── Magasin.cpp
├── Magasin.h
├── README.md
└── .gitignore
```

---

# 🛠️ Technologies utilisées

- C++17
- STL
- Programmation Orientée Objet
- FTXUI
- CMake
- Git / GitHub

---

# ⚙️ Compilation
Le projet se compile avec CMake et C++17.

Depuis la racine du projet :

```bash
cmake -S . -B build
cmake --build build -j
```

---

# ▶️ Exécution

```bash
./build/boutique_app
```
#
---

## ℹ️ Notes d'utilisation (français)

- **Client — saisir son nom :** Ouvrez l'onglet "Client", saisissez votre nom dans le champ "Votre nom" puis cliquez sur "Confirmer". Le nom apparaît sur le ticket de commande.
- **Ajouter au panier :** Dans l'onglet "Panier" sélectionnez un produit, indiquez une quantité strictement positive (> 0) et cliquez sur "Ajouter au Panier". Les quantités nulles ou négatives sont rejetées.
- **Stock faible :** Les produits avec moins de 5 unités affichent l'indicateur "stock faible" dans le catalogue et la liste du panier.
- **Historique intégré :** L'onglet "Historique" affiche les commandes (du plus récent au plus ancien). Chaque entrée contient la date et l'heure (format YYYY-MM-DD HH:MM:SS). Le journal est aussi sauvegardé dans `journal.txt`.
- **Administration :** Allez dans l'onglet "Admin". Saisissez le mot de passe `admin123` puis cliquez sur "Se connecter". Après authentification le formulaire d'ajout de produit apparaît (Nom / Prix / Stock). Cliquez "Se déconnecter" pour quitter la session admin.
- **Remarques sur l'architecture :** Le projet utilise des concepts POO : héritage (`Produit` et ses dérivés), polymorphisme (méthodes `virtual`/`override`, notamment `get_description_complete()`), et surcharge (ex. `operator<<` pour `Panier`).

Si vous voulez que je :
- masque le champ mot de passe (afficher des astérisques),
- limite le nombre d'entrées d'historique affichées,
- ou externalise le mot de passe dans un fichier de configuration,
dites-moi lequel et je l'implémente.

# 📖 Utilisation du programme
## 📷 Figure 1 — Menu principal

<img width="497" height="249" alt="image" src="https://github.com/user-attachments/assets/48b85a17-40ad-4173-90d7-3dcbd8e2734e" />


Cette interface représente le menu principal de l’application e-commerce.
La barre de navigation située à gauche permet d’accéder aux différentes sections du programme :
- Client
- Produits
- Panier
- Historique

L’utilisateur peut naviguer facilement entre les différentes fonctionnalités grâce à l’interface interactive développée avec FTXUI.
---


## 📦 Figure 2 — Catalogue des produits

<img width="584" height="384" alt="image" src="https://github.com/user-attachments/assets/493f8862-4479-496d-ae24-e18efd955bd8" />


Cette interface affiche le catalogue des produits disponibles dans le magasin.

Chaque produit possède :
- un identifiant,
- un nom,
- un prix,
- une quantité en stock.

Le système signale automatiquement les produits ayant un stock faible afin d’améliorer la gestion des articles.

---
## 🛒 Figure 3 — Gestion du panier

<img width="651" height="521" alt="image" src="https://github.com/user-attachments/assets/22e7ddfe-f549-45bf-8532-e459f0e0a87a" />


Cette interface permet à l’utilisateur de gérer son panier d’achat.

L’utilisateur peut :
- sélectionner un produit,
- saisir une quantité,
- ajouter un produit au panier,
- vider le panier,
- valider une commande.

Le programme calcule automatiquement :
- le sous-total,
- les remises,
- la TVA,
- le total TTC.
---
## 🔐Figure 4 — Section Administration

<img width="619" height="394" alt="image" src="https://github.com/user-attachments/assets/a80730f6-baf0-46b2-811a-1632426f1014" />


Elle permet d’ajouter dynamiquement de nouveaux produits au catalogue grâce à un formulaire interactif développé avec FTXUI.

L’administrateur peut saisir :
- le nom du produit,
- le prix,
- la quantité disponible en stock.

Cette fonctionnalité améliore la gestion du catalogue et introduit une séparation entre les fonctionnalités client et administrateur.

---

## 📜 Figure 5 — Historique des commandes

<img width="710" height="283" alt="Capture d&#39;écran 2026-05-13 010715" src="https://github.com/user-attachments/assets/00cf1d1b-d0f6-41e3-bddf-e5235779ea24" />


Cette interface affiche l’historique des commandes effectuées par le client.

Après la validation d’une commande, les informations suivantes sont enregistrées :
- l’identifiant de la commande,
- la date et l’heure,
- les produits achetés,
- les quantités,
- le montant total TTC.
  
Vous trouverez tout cela dans le fichier Journal de votre fichier builld.

Cette fonctionnalité permet au client de consulter les anciennes commandes et améliore le suivi des achats réalisés dans l’application.



# 📚 Concepts POO utilisés

- Encapsulation
- Abstraction
- Composition
- Modularité
- Gestion des classes

---


un Rapport sur le projet disponible sur le page  suivant :https://benevolent-squirrel-63ea8d.netlify.app/


---

# 🎨 Interface

L’interface utilisateur est réalisée avec la bibliothèque FTXUI afin de proposer une expérience moderne directement dans le terminal.

---

# 👨‍💻 Auteur

Projet réalisé par MDL.
Maryem, Doaa & Laila






































































