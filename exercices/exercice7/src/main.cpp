#include <iostream>
#include "math_utils.hpp"
#include "io_utils.hpp"

int main()
{
    int a, b;
    double res;
    
    // Ask user for input
    std::cout << "Entrez deux entiers :" << std::endl;
    
    // Safe reading of two integers
    // Each call must validate the input
    std::cout << "a = ";
    while (!lire_entier(a)) {
        std::cout << "Erreur de saisie. Reessayez : a = ";
    }
    
    std::cout << "b = ";
    while (!lire_entier(b)) {
        std::cout << "Erreur de saisie. Reessayez : b = ";
    }
    
    // Call average calculation function
    // Observe return type and implicit conversions
    double moy = moyenne(a, b);
    afficher_resultat("Moyenne = ", moy);
    
    // Call safe division
    if (division_securisee(a, b, res))
    {
        // Display result if division is valid
        afficher_resultat("Division = ", res);
    }
    else
    {
        // Logical error case handled properly
        std::cout << "Erreur : division par zero impossible" << std::endl;
    }
    
    // End of program
    return 0;
}