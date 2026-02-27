# TP1 - Exercice 7

Structure modulaire C++ avec séparation headers/sources.

## Fichiers
- `include/` : Déclarations (.hpp) avec include guards
- `src/` : Implémentations (.cpp) et main

## Documentation des Fonctions

### double moyenne(int a, int b)
Calcule la moyenne arithmétique de deux entiers.
Retourne (a + b) / 2.0 en double (division flottante).

### bool division_securisee(int a, int b, double& r)
Effectue une division sécurisée. Si b == 0, retourne false.
Sinon, stocke le résultat dans r (par référence) et retourne true.

### bool lire_entier(int& x)
Lit un entier depuis std::cin. En cas d'erreur, vide le buffer
et retourne false. Sinon, stocke la valeur dans x et retourne true.

### void afficher_resultat(const char* msg, double val)
Affiche un message suivi d'une valeur numérique.
Format : msg + val + saut de ligne.

## ✅ Checklist
- [x] Code compiles with `-Wall -Wextra -pedantic`
- [ ] Every line of code is commented
- [ ] Theoretical questions answered above
- [ ] Data files (if any) included in `data/`
