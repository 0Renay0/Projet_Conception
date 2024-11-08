#include <cpr/cpr.h>

#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

using json = nlohmann::json;

// Classe Ville correspondant au modèle Django Ville
class Ville {
 public:
  std::string nom;
  int code_postal;
  int prix_m_2;

  // Constructeur prenant directement les attributs
  Ville(const std::string& nom, int code_postal, int prix_m_2)
      : nom(nom), code_postal(code_postal), prix_m_2(prix_m_2) {}

  // Constructeur prenant un json pour initialiser les attributs
  Ville(const json& data)
      : nom(data["Nom"]), code_postal(data["CP"]), prix_m_2(data["Prix/m^2"]) {}

  // Constructeur prenant un ID et récupérant les données via HTTP
  Ville(int id) {
    cpr::Response r =
        cpr::Get(cpr::Url{"http://localhost:8000/Ville/" + std::to_string(id)});
    json data = json::parse(r.text);
    nom = data.value("Nom", "");
    code_postal = data.value("CP", 0);
    prix_m_2 = data.value("Prix/m^2", 0);
  }

  // Affichage
  void afficher() const {
    std::cout << "Ville: " << nom << "\nCode Postal: " << code_postal
              << "\nPrix au mètre carré: " << prix_m_2 << std::endl;
  }
};

class Ressource {
 public:
  std::string nom;
  int prix;

  Ressource(const std::string& nom, int prix) : nom(nom), prix(prix) {}

  Ressource(const json& data) : nom(data["Nom"]), prix(data["Prix"]) {}

  Ressource(int id) {
    cpr::Response r = cpr::Get(
        cpr::Url{"http://localhost:8000/Ressource/" + std::to_string(id)});
    json data = json::parse(r.text);
    nom = data.value("Nom", "");
    prix = data.value("Prix", 0);
  }

  void afficher() const {
    std::cout << "Ressource: " << nom << "\nPrix: " << prix << std::endl;
  }
};

class QuantiteRessource {
 public:
  int quantite;
  int ressource_id;

  QuantiteRessource(int quantite, int ressource_id)
      : quantite(quantite), ressource_id(ressource_id) {}

  // Constructeur prenant un objet JSON
  QuantiteRessource(const json& data)
      : quantite(data.value("Quantite", 0)),
        ressource_id(data.value("Resource", 0)) {}

  // Constructeur prenant un ID et récupérant les données via HTTP
  QuantiteRessource(int id) {
    cpr::Response r = cpr::Get(cpr::Url{
        "http://localhost:8000/QuantiteRessource/" + std::to_string(id)});
    json data = json::parse(r.text);

    quantite = data.value("Quantite", 0);
    ressource_id =
        data.value("Resource", 0);  // Récupération de l'ID de la ressource
  }

  void afficher() const {
    std::cout << "Quantite de Ressource: " << quantite
              << "\nID de Ressource: " << ressource_id << std::endl;
  }
};

class Machine {
 public:
  std::string nom;
  int prix;
  int n_serie;

  Machine(const std::string& nom, int prix, int n_serie)
      : nom(nom), prix(prix), n_serie(n_serie) {}

  Machine(const json& data)
      : nom(data["Nom"]), prix(data["Prix"]), n_serie(data["n° de serie"]) {}

  Machine(int id) {
    cpr::Response r = cpr::Get(
        cpr::Url{"http://localhost:8000/Machine/" + std::to_string(id)});
    json data = json::parse(r.text);
    nom = data.value("Nom", "");
    prix = data.value("Prix", 0);
    n_serie = data.value("n° de serie", 0);
  }

  void afficher() const {
    std::cout << "Machine: " << nom << "\nPrix: " << prix
              << "\nNuméro de série: " << n_serie << std::endl;
  }
};

class Etape {
 public:
  std::string nom;
  int duree;
  int quantite_ressource_id;  // ID de QuantiteRessource avec valeur par défaut
  int machine_id;             // ID de la machine
  int etape_suivante_id;      // ID de l'étape suivante avec valeur par défaut

  Etape(const std::string& nom, int duree, int machine_id)
      : nom(nom),
        duree(duree),
        quantite_ressource_id(0),
        machine_id(machine_id),
        etape_suivante_id(0) {}

  // Constructeur prenant un objet JSON
  Etape(const json& data)
      : nom(data.value("Nom", "")),
        duree(data.value("Durée", 0)),
        quantite_ressource_id(data.value("QuantiteRessource ID", 0)),
        machine_id(data.value("Machine ID", 0)),
        etape_suivante_id(data.value("Etape Suivante ID", 0)) {}

  // Constructeur prenant un ID et récupérant les données via HTTP
  Etape(int id) {
    cpr::Response r =
        cpr::Get(cpr::Url{"http://localhost:8000/Etape/" + std::to_string(id)});
    json data = json::parse(r.text);

    nom = data.value("Nom", "");
    duree = data.value("Durée", 0);
    quantite_ressource_id = data.value("Quantité necessaire", 0);
    machine_id = data.value("Machine ID", 0);
    etape_suivante_id = data.value("Etape suivante ID", 0);
  }

  void afficher() const {
    std::cout << "Etape: " << nom << "\nDurée: " << duree
              << "\nMachine: " << machine_id
              << "\nQuantite de Ressource: " << quantite_ressource_id
              << "\nEtape Suivante: " << etape_suivante_id << std::endl;
  }
};

class Produit {
 public:
  std::string nom;
  int prix;
  int premiere_etape_id;

  Produit(const std::string& nom, int prix, int premiere_etape_id)
      : nom(nom), prix(prix), premiere_etape_id(premiere_etape_id) {}

  // Constructeur prenant un objet JSON
  Produit(const json& data)
      : nom(data.value("Nom", "")),
        prix(data.value("Prix", 0)),
        premiere_etape_id(data.value("Premiere etape", 0)) {}

  // Constructeur prenant un ID et récupérant les données via HTTP
  Produit(int id) {
    cpr::Response r = cpr::Get(
        cpr::Url{"http://localhost:8000/Produit/" + std::to_string(id)});
    json data = json::parse(r.text);

    nom = data.value("Nom", "");
    prix = data.value("Prix", 0);
    premiere_etape_id = data.value("Premiere etape", 0);
  }

  void afficher() const {
    std::cout << "Produit: " << nom << "\nPrix: " << prix
              << "\nID de la Premiere Etape: " << premiere_etape_id
              << std::endl;
  }
};

class Usine {
 public:
  std::string nom;
  int surface;
  int ville_id;                  // ID de la ville
  int siege_social_id;           // ID du siège social
  std::vector<int> machine_ids;  // Liste des IDs des machines
  std::vector<int> produit_ids;  // Liste des IDs des produits
  std::unordered_map<int, int>
      ressources_manquantes;  // ID des ressources et leurs quantités manquantes

  // Constructeur prenant un nom, surface, ville_id, etc.
  Usine(const std::string& nom, int surface, int ville_id, int siege_social_id,
        const std::vector<int>& machine_ids,
        const std::vector<int>& produit_ids,
        const std::unordered_map<int, int>& ressources_manquantes)
      : nom(nom),
        surface(surface),
        ville_id(ville_id),
        siege_social_id(siege_social_id),
        machine_ids(machine_ids),
        produit_ids(produit_ids),
        ressources_manquantes(ressources_manquantes) {}

  // Constructeur prenant un objet JSON
  Usine(const json& data)
      : nom(data.value("Nom", "")),         // Récupère directement le nom
        surface(data.value("Surface", 0)),  // Récupère directement la surface
        ville_id(data.value("Ville", 0)),   // ID de la ville
        siege_social_id(data.value("Siege Social", 0)),  // ID du siège social
        machine_ids(data.value(
            "Machine", std::vector<int>{})),  // Liste des IDs de machines
        produit_ids(data.value(
            "Produit", std::vector<int>{})) {  // Liste des IDs de produits

    // Récupération des ressources manquantes comme un dictionnaire d'ID ->
    // quantité
    for (const auto& [key, value] : data["Ressources manquantes"].items()) {
      int ressource_id = std::stoi(key);  // Convertir la clé en entier
      int quantite = value;
      ressources_manquantes[ressource_id] = quantite;
    }
  }

  // Constructeur prenant un ID et récupérant les données via HTTP
  Usine(int id) {
    cpr::Response r =
        cpr::Get(cpr::Url{"http://localhost:8000/Usine/" + std::to_string(id)});
    json data = json::parse(r.text);

    nom =
        data.at("Nom").at(0);  // Récupère le premier élément de la liste "Nom"
    surface = data.at("Surface").at(
        0);  // Récupère le premier élément de la liste "Surface"
    ville_id = data.value("Ville", 0);
    siege_social_id = data.value("Siege Social", 0);
    machine_ids = data.value("Machine", std::vector<int>{});
    produit_ids = data.value("Produit", std::vector<int>{});

    // Récupération des ressources manquantes
    for (const auto& [key, value] : data["Ressources manquantes"].items()) {
      int ressource_id = std::stoi(key);  // Convertir la clé en entier
      int quantite = value;
      ressources_manquantes[ressource_id] = quantite;
    }
  }

  void afficher() const {
    std::cout << "Usine: " << nom << "\nSurface: " << surface
              << "\nID de Ville: " << ville_id
              << "\nID de Siege Social: " << siege_social_id
              << "\nIDs des Machines: ";
    for (int id : machine_ids) {
      std::cout << id << " ";
    }
    std::cout << "\nIDs des Produits: ";
    for (int id : produit_ids) {
      std::cout << id << " ";
    }
    std::cout << "\nRessources Manquantes: ";
    for (const auto& [ressource_id, quantite] : ressources_manquantes) {
      std::cout << "ID: " << ressource_id << ", Quantite: " << quantite << "; ";
    }
    std::cout << std::endl;
  }
};

class Stock {
 public:
  int ressource_id;
  int usine_id;
  int nombre;

  Stock(int ressource_id, int usine_id, int nombre)
      : ressource_id(ressource_id), usine_id(usine_id), nombre(nombre) {}

  // Constructeur prenant un objet JSON
  Stock(const json& data)
      : ressource_id(data.value("Ressource ID ", 0)),
        usine_id(data.value("Usine ID", 0)),
        nombre(data.value("Nombre", 0)) {}

  // Constructeur qui récupére les données via HTTP
  Stock(int id) {
    cpr::Response r =
        cpr::Get(cpr::Url{"http://localhost:8000/Stock/" + std::to_string(id)});
    json data = json::parse(r.text);

    ressource_id = data.value("Ressource ID ", 0);
    usine_id = data.value("Usine ID", 0);
    nombre = data.value("Nombre", 0);
  }

  void afficher() const {
    std::cout << "ID de Ressource: " << ressource_id
              << "\nID de l'Usine: " << usine_id << "\nNombre: " << nombre
              << std::endl;
  }
};

class SiegeSocial {
 public:
  std::string nom;
  int surface;
  int ville_id;

  SiegeSocial(const std::string& nom, int surface, int ville_id)
      : nom(nom), surface(surface), ville_id(ville_id) {}

  // Constructeur prenant un objet JSON
  SiegeSocial(const json& data)
      : nom(data.value("Nom", "")),
        surface(data.value("Surface", 0)),
        ville_id(data.value("Ville", 0)) {}

  // Constructeur prenant un ID et récupérant les données via HTTP
  SiegeSocial(int id) {
    cpr::Response r = cpr::Get(
        cpr::Url{"http://localhost:8000/SiegeSocial/" + std::to_string(id)});
    json data = json::parse(r.text);

    nom = data.value("Nom", "");
    surface = data.value("Surface", 0);
    ville_id = data.value("Ville", 0);
  }

  void afficher() const {
    std::cout << "Siege Social: " << nom << "\nSurface: " << surface
              << "\nID de Ville: " << ville_id << std::endl;
  }
};

int main() {
  // Création d'une instance de Ville avec des valeurs d'exemple
  // Ville ville1("Toulouse", 31000, 5000);
  // ville1.afficher();
  std::cout << "\n -------------------------\n" << std::endl;
  // Création d'une instance de Ville en utilisant un ID pour récupérer les
  // données depuis l'API
  Ville ville2(
      1);  // Remplacez '1' par un ID réel présent dans votre base de données
  ville2.afficher();
  std::cout << "\n -------------------------\n" << std::endl;

  Ressource ressource1(1);
  ressource1.afficher();

  std::cout << "\n -------------------------\n" << std::endl;

  QuantiteRessource quantiteressource(
      4);  // ID de quantiteressource commence de 4
  quantiteressource.afficher();

  std::cout << "\n -------------------------\n" << std::endl;

  Machine machine(3);  // ID de machine commence de 3
  machine.afficher();

  std::cout << "\n -------------------------\n" << std::endl;

  Etape etape(3);
  etape.afficher();

  std::cout << "\n -------------------------\n" << std::endl;

  Produit produit(2);
  produit.afficher();

  std::cout << "\n -------------------------\n" << std::endl;

  Usine usine(1);
  usine.afficher();

  std::cout << "\n -------------------------\n" << std::endl;

  Stock stock(1);
  stock.afficher();

  std::cout << "\n -------------------------\n" << std::endl;

  SiegeSocial siegesocial(1);
  siegesocial.afficher();

  std::cout << "\n -------------------------\n" << std::endl;

  return 0;
}
