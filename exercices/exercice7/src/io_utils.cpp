#include "io_utils.hpp"
#include <iostream>
#include <limits>

bool lire_entier(int& x) {
    std::cin >> x;
    
    if (std::cin.fail()) {
        // Clear error state
        std::cin.clear();
        // Discard invalid input
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    
    // Clear any remaining characters (like newline)
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

void afficher_resultat(const char* message, double valeur) {
    std::cout << message << valeur << std::endl;
}