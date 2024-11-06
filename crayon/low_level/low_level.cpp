#include <cpr/cpr.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Ville {
 public:
  std::string nom;
  int code_postal;
  float prix_m_2;

  // Constructeur: Il prend un ID et recupere les données de la ville
  Ville(int id) {
    // Requête HTTP
    cpr::Response r =
        cpr::Get(cpr::Url{"http://localhost:8000/Ville/" + std::to_string(id)});

    // Parsing JSON
    json data = json::parse(r.text);
    nom = data["Nom"];
    code_postal = data["CP"];
    prix_m_2 = data["Prix/m^2"];
  }

  // Méthode d'affichage
  void afficher() const {
    std::cout << "Ville: " << nom << "\nCode Postal: " << code_postal
              << "\nPrix/m^2: " << prix_m_2 << "€" << std::endl;
  }
};

class Machine {
 public:
  std::string nom;
  int prix;
  long int NS;

  // Constructeur
  Machine(int id) {
    cpr::Response r = cpr::Get(
        cpr::Url{"http://localhost:8000/Machine/" + std::to_string(id)});
    json data = json::parse(r.text);
    nom = data["Nom"];
    prix = data["Prix"];
    NS = data["n° de serie"];
  }

  void afficher() const {
    std::cout << "Machine: " << nom << "\nPrix: " << prix
              << "\nn° de serie: " << NS << std::endl;
  }
};

class Stock {
 public:
  int ressource_id;
  int usine_id;
  long int Nombre;

  // Constructeur
  Stock(int id) {
    cpr::Response r =
        cpr::Get(cpr::Url{"http://localhost:8000/Stock/" + std::to_string(id)});
    json data = json::parse(r.text);
    ressource_id = data["Ressource ID"];
    usine_id = data["Usine ID"];
    Nombre = data["Nombre"];
  }

  void afficher() const {
    std::cout << "Stock Ressource: " << ressource_id << "\nUsine: " << usine_id
              << "\nNombre: " << Nombre << std::endl;
  }
};

class Produit {
 public:
  std::string nom;
  int prix;
  int Premiere_etape_ID;

  // Constructeur
  Produit(int id) {
    cpr::Response r = cpr::Get(
        cpr::Url{"http://localhost:8000/Produit/" + std::to_string(id)});
    json data = json::parse(r.text);
    nom = data["Nom"];
    prix = data["Prix"];
    Premiere_etape_ID = data["Premiere etape"];
  }

  void afficher() const {
    std::cout << "Produit: " << nom << "\nPrix: " << prix
              << "\nPremiere etape: " << Premiere_etape_ID << std::endl;
  }
};

auto main() -> int {
  std::cout << "\n -------------------------" << std::endl;
  // Création d'une instance de Ville avec un ID 1 pour notre ville TLS-01
  Ville v(1);
  v.afficher();
  std::cout << "\n -------------------------" << std::endl;
  // Création d'une instance de Machine avec un ID 3 car c'est le premier ID de
  // notre base de données
  Machine m(5);
  m.afficher();
  std::cout << "\n -------------------------" << std::endl;
  // Création d'une instance de stock avec un ID 1
  // Stock s(2);
  // s.afficher();
  // std::cout << "\n -------------------------" << std::endl;
  Produit p(2);
  p.afficher();
  std::cout << "\n -------------------------" << std::endl;

  return 0;
}
