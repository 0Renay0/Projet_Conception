#include <cpr/cpr.h>

#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

// Classe Ville correspondant au modèle Django Ville
class Ville {
 public:
  std::string nom;
  int code_postal;
  int prix_m_2;

  Ville(const std::string& nom, int code_postal, int prix_m_2)
      : nom(nom), code_postal(code_postal), prix_m_2(prix_m_2) {}

  Ville(const json& data)
      : nom(data["Nom"]), code_postal(data["CP"]), prix_m_2(data["Prix/m^2"]) {}

  Ville(int id) {
    cpr::Response r =
        cpr::Get(cpr::Url{"http://localhost:8000/Ville/" + std::to_string(id)});
    json data = json::parse(r.text);
    nom = data.value("Nom", "");
    code_postal = data.value("CP", 0);
    prix_m_2 = data.value("Prix/m^2", 0);
  }

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
  int resource_id;
  int quantite;
  std::shared_ptr<Ressource> ressource;  // Relation directe avec Ressource

  // Constructeur avec paramètres
  QuantiteRessource(int resource_id, int quantite)
      : resource_id(resource_id), quantite(quantite), ressource(nullptr) {}

  // Constructeur avec données JSON
  QuantiteRessource(const json& data)
      : resource_id(data["Resource"]), quantite(data["Quantite"]) {
    // Initialisation de la Ressource à partir des données JSON
    if (data.contains("ResourceData")) {
      ressource = std::make_shared<Ressource>(data["ResourceData"]);
    }
  }

  // Constructeur avec requête HTTP
  QuantiteRessource(int id) {
    cpr::Response r = cpr::Get(cpr::Url{
        "http://localhost:8000/QuantiteRessource/" + std::to_string(id)});
    json data = json::parse(r.text);

    resource_id = data.value("Resource", 0);
    quantite = data.value("Quantite", 0);

    // Récupérer les données de la Ressource associée
    ressource = std::make_shared<Ressource>(resource_id);
  }

  // Méthode pour afficher les données
  void afficher() const {
    std::cout << "Détails de la ressource:\n";
    ressource->afficher();
    std::cout << "Quantité de Ressource:" << quantite << std::endl;
  }
};

class Machine {
 public:
  std::string nom;
  int prix;
  long numero_serie;

  // Constructeur avec paramètres
  Machine(const std::string& nom, int prix, long numero_serie)
      : nom(nom), prix(prix), numero_serie(numero_serie) {}

  // Constructeur avec données JSON
  Machine(const json& data)
      : nom(data["Nom"]),
        prix(data["Prix"]),
        numero_serie(data["n° de serie"]) {}

  // Constructeur avec requête HTTP
  Machine(int id) {
    cpr::Response r = cpr::Get(
        cpr::Url{"http://localhost:8000/Machine/" + std::to_string(id)});
    json data = json::parse(r.text);
    nom = data.value("Nom", "");
    prix = data.value("Prix", 0);
    numero_serie = data.value("n° de serie", 0L);
  }

  // Méthode pour afficher les données
  void afficher() const {
    std::cout << "Machine: " << nom << "\nPrix: " << prix
              << "\nNuméro de série: " << numero_serie << std::endl;
  }
};

class Etape {
 public:
  std::string nom;
  int duree;
  int quantite_necessaire;           // Valeur par défaut si null
  std::shared_ptr<Machine> machine;  // Relation directe avec Machine
  int etape_suivante_id;  // ID de l'étape suivante dans les données JSON
  std::unique_ptr<Etape>
      etape_suivante;  // Relation récursive avec Etape (unique_ptr)

  // Constructeur avec données JSON
  Etape(const json& data)
      : nom(data.at("Nom")),
        duree(data.value("Dur\u00e9e", 0)),
        quantite_necessaire(0),    // Initialisation par défaut
        etape_suivante_id(0),      // Initialisation par défaut
        etape_suivante(nullptr) {  // Initialisation de la relation récursive

    // Gestion sécurisée de "Quantité necessaire"
    if (data.contains("Quantité necessaire") &&
        !data["Quantité necessaire"].is_null()) {
      quantite_necessaire = data["Quantité necessaire"].get<int>();
    }

    // Gestion sécurisée de "Etape suivante ID"
    if (data.contains("Etape suivante ID") &&
        !data["Etape suivante ID"].is_null()) {
      etape_suivante_id = data["Etape suivante ID"].get<int>();
    }

    // Initialisation de la Machine
    machine = std::make_shared<Machine>(data["Machine ID"]);
  }

  // Constructeur avec requête HTTP
  Etape(int id) {
    cpr::Response r =
        cpr::Get(cpr::Url{"http://localhost:8000/Etape/" + std::to_string(id)});
    json data = json::parse(r.text);

    nom = data.at("Nom");
    duree = data.value("Dur\u00e9e", 0);
    quantite_necessaire = 0;  // Initialisation par défaut
    etape_suivante_id = 0;    // Initialisation par défaut

    // Gestion sécurisée de "Quantité necessaire"
    if (data.contains("Quantité necessaire") &&
        !data["Quantité necessaire"].is_null()) {
      quantite_necessaire = data["Quantité necessaire"].get<int>();
    }

    // Gestion sécurisée de "Etape suivante ID"
    if (data.contains("Etape suivante ID") &&
        !data["Etape suivante ID"].is_null()) {
      etape_suivante_id = data["Etape suivante ID"].get<int>();
    }

    // Initialisation de la Machine
    machine = std::make_shared<Machine>(data.value("Machine ID", 0));

    // Initialisation de l'étape suivante si l'ID est valide
    if (etape_suivante_id > 0) {
      etape_suivante = std::make_unique<Etape>(etape_suivante_id);
    }
  }

  // Méthode pour afficher les données
  void afficher() const {
    std::cout << "Etape: " << nom << "\nDur\u00e9e: " << duree;

    std::cout << "\nQuantité nécessaire: ";
    if (quantite_necessaire > 0) {
      std::cout << quantite_necessaire;
    } else {
      std::cout << "null";
    }

    std::cout << "\nDétails de la Machine:\n";
    machine->afficher();

    std::cout << "Etape suivante ID: ";
    if (etape_suivante_id > 0) {
      std::cout << etape_suivante_id;
    } else {
      std::cout << "null";
    }
    std::cout << std::endl;

    if (etape_suivante) {
      std::cout << "Détails de l'étape suivante:\n\t ";
      etape_suivante->afficher();
    } else {
      std::cout << "Fin du produit !." << std::endl;
    }
  }
};

class Produit {
 public:
  std::string nom;
  int prix;
  int premiere_etape_id;
  std::shared_ptr<Etape> premiere_etape;  // Relation directe avec Etape

  // Constructeur avec paramètres
  Produit(const std::string& nom, int prix, int premiere_etape_id)
      : nom(nom),
        prix(prix),
        premiere_etape_id(premiere_etape_id),
        premiere_etape(nullptr) {}

  // Constructeur avec données JSON
  Produit(const json& data)
      : nom(data.at("Nom")),
        prix(data.at("Prix")),
        premiere_etape_id(data.at("Premiere etape")),
        premiere_etape(std::make_shared<Etape>(data["Premiere etape data"])) {}

  // Constructeur avec requête HTTP
  Produit(int id) {
    cpr::Response r = cpr::Get(
        cpr::Url{"http://localhost:8000/Produit/" + std::to_string(id)});
    json data = json::parse(r.text);
    nom = data.at("Nom");
    prix = data.value("Prix", 0);
    premiere_etape_id = data.value("Premiere etape", 0);

    // Initialisation de la première étape si les données sont disponibles
    if (premiere_etape_id > 0) {
      premiere_etape = std::make_shared<Etape>(premiere_etape_id);
    }
  }

  // Méthode pour afficher les données
  void afficher() const {
    std::cout << "Produit: " << nom << "\nPrix: " << prix
              << "\nPremière étape ID: " << premiere_etape_id << std::endl;
    std::cout << "Détails de la première étape:\n";
    premiere_etape->afficher();
  }
};

class SiegeSocial {
 public:
  std::string nom;
  int surface;
  int ville_id;
  std::unique_ptr<Ville> ville;  // Relation avec Ville via pointeur unique

  // Constructeur avec paramètres
  SiegeSocial(const std::string& nom, int surface, int ville_id)
      : nom(nom),
        surface(surface),
        ville_id(ville_id),
        ville(std::make_unique<Ville>(ville_id)) {}

  // Constructeur avec données JSON
  SiegeSocial(const json& data)
      : nom(data.at("Nom")),
        surface(data.at("Surface")),
        ville_id(data.at("Ville")),
        ville(std::make_unique<Ville>(data.at("Ville"))) {}

  // Constructeur avec requête HTTP
  SiegeSocial(int id) {
    cpr::Response r = cpr::Get(
        cpr::Url{"http://localhost:8000/SiegeSocial/" + std::to_string(id)});
    json data = json::parse(r.text);
    nom = data.value("Nom", "");
    surface = data.value("Surface", 0);
    ville_id = data.value("Ville", 0);

    // Initialisation de la relation Ville
    ville = std::make_unique<Ville>(ville_id);
  }

  // Méthode pour afficher les données
  void afficher() const {
    std::cout << "Siège Social: " << nom << "\nSurface: " << surface
              << "\nVille ID: " << ville_id << "\n";
    std::cout << "Détails de la Ville:\n";
    ville->afficher();
  }
};

class Usine {
 public:
  std::string nom;
  int ville_id;
  int surface;
  int cout_total;
  int siege_social_id;
  std::vector<int> machines_ids;
  std::vector<int> produits_ids;
  std::map<std::string, int> ressources_manquantes;

  std::unique_ptr<Ville> ville;                    // Relation avec Ville
  std::unique_ptr<SiegeSocial> siege_social;       // Relation avec SiegeSocial
  std::vector<std::unique_ptr<Machine>> machines;  // Relation avec Machines
  std::vector<std::unique_ptr<Produit>> produits;  // Relation avec Produits

  // Constructeur avec paramètres
  Usine(const std::string& nom, int ville_id, int surface, int cout_total,
        int siege_social_id, const std::vector<int>& machines_ids,
        const std::vector<int>& produits_ids,
        const std::map<std::string, int>& ressources_manquantes)
      : nom(nom),
        ville_id(ville_id),
        surface(surface),
        cout_total(cout_total),
        siege_social_id(siege_social_id),
        machines_ids(machines_ids),
        produits_ids(produits_ids),
        ressources_manquantes(ressources_manquantes),
        ville(std::make_unique<Ville>(ville_id)),
        siege_social(std::make_unique<SiegeSocial>(siege_social_id)) {
    for (const auto& id : machines_ids) {
      machines.push_back(std::make_unique<Machine>(id));
    }
    for (const auto& id : produits_ids) {
      produits.push_back(std::make_unique<Produit>(id));
    }
  }

  // Constructeur avec données JSON
  Usine(const json& data)
      : nom(data.at("Nom").at(0)),  // Extraire la première valeur du tableau
        ville_id(data.at("Ville")),
        surface(data.at("Surface").at(
            0)),  // Extraire la première valeur du tableau
        cout_total(data.at("Cout Total")),
        siege_social_id(data.at("Siege Social")),
        machines_ids(data.at("Machine").get<std::vector<int>>()),
        produits_ids(data.at("Produit").get<std::vector<int>>()),
        ressources_manquantes(
            data.at("Ressources manquantes").get<std::map<std::string, int>>()),
        ville(std::make_unique<Ville>(data.at("Ville"))),
        siege_social(std::make_unique<SiegeSocial>(data.at("Siege Social"))) {
    for (const auto& id : machines_ids) {
      machines.push_back(std::make_unique<Machine>(id));
    }
    for (const auto& id : produits_ids) {
      produits.push_back(std::make_unique<Produit>(id));
    }
  }

  // Constructeur avec requête HTTP
  Usine(int id) {
    cpr::Response r =
        cpr::Get(cpr::Url{"http://localhost:8000/Usine/" + std::to_string(id)});
    json data = json::parse(r.text);
    nom = data.at("Nom").at(0);
    surface = data.at("Surface").at(0);
    ville_id = data.value("Ville", 0);
    cout_total = data.value("Cout Total", 0);
    siege_social_id = data.value("Siege Social", 0);
    machines_ids = data["Machine"].get<std::vector<int>>();
    produits_ids = data["Produit"].get<std::vector<int>>();
    ressources_manquantes =
        data["Ressources manquantes"].get<std::map<std::string, int>>();

    // Initialisation des relations
    ville = std::make_unique<Ville>(ville_id);
    siege_social = std::make_unique<SiegeSocial>(siege_social_id);
    for (const auto& id : machines_ids) {
      machines.push_back(std::make_unique<Machine>(id));
    }
    for (const auto& id : produits_ids) {
      produits.push_back(std::make_unique<Produit>(id));
    }
  }

  // Méthode pour afficher les données
  void afficher() const {
    std::cout << "Usine: " << nom << "\nVille ID: " << ville_id
              << "\nSurface: " << surface << "\nCoût Total: " << cout_total
              << "\nSiège Social ID: " << siege_social_id << "\nMachines: ";
    for (const auto& machine : machines) {
      machine->afficher();
    }
    std::cout << "\nProduits: ";
    for (const auto& produit : produits) {
      produit->afficher();
    }
    std::cout << "\nRessources Manquantes: ";
    for (const auto& rm : ressources_manquantes) {
      std::cout << rm.first << ": " << rm.second << " ";
    }
    std::cout << std::endl;

    std::cout << "Détails de la Ville:\n";
    if (ville) {
      ville->afficher();
    }
    std::cout << "Détails du Siège Social:\n";
    if (siege_social) {
      siege_social->afficher();
    }
  }
};

class Stock {
 public:
  int ressource_id;  // ID de la ressource
  int usine_id;      // ID de l'usine
  int nombre;        // Quantité de ressource en stock
  std::unique_ptr<Ressource>
      ressource;                 // Relation avec Ressource via pointeur unique
  std::unique_ptr<Usine> usine;  // Relation avec Usine via pointeur unique

  // Constructeur avec paramètres
  Stock(int ressource_id, int usine_id, int nombre)
      : ressource_id(ressource_id),
        usine_id(usine_id),
        nombre(nombre),
        ressource(std::make_unique<Ressource>(ressource_id)),
        usine(std::make_unique<Usine>(usine_id)) {}

  // Constructeur avec données JSON
  Stock(const json& data)
      : ressource_id(data.at("Ressource ID ")),
        usine_id(data.at("Usine ID")),
        nombre(data.at("Nombre")),
        ressource(std::make_unique<Ressource>(data.at("Ressource"))),
        usine(std::make_unique<Usine>(data.at("Usine"))) {}

  // Constructeur avec requête HTTP
  Stock(int id) {
    cpr::Response r =
        cpr::Get(cpr::Url{"http://localhost:8000/Stock/" + std::to_string(id)});
    json data = json::parse(r.text);
    ressource_id = data.value("Ressource ID ", 0);
    usine_id = data.value("Usine ID", 0);
    nombre = data.value("Nombre", 0);

    ressource = std::make_unique<Ressource>(ressource_id);
    usine = std::make_unique<Usine>(usine_id);
  }

  // Méthode pour afficher les données
  void afficher() const {
    std::cout << "Ressource ID: " << ressource_id << "\n";
    std::cout << "Détails de la ressource:\n";
    ressource->afficher();
    std::cout << "Usine ID: " << usine_id << "\n";
    std::cout << "Détails de l'usine:\n";
    usine->afficher();
    std::cout << "Nombre: " << nombre << "\n";
  }
};

int main() {
  Ville ville(1);
  ville.afficher();
  std::cout << "\n -------------------------\n" << std::endl;
  Ressource ressource(1);
  ressource.afficher();
  std::cout << "\n -------------------------\n" << std::endl;
  QuantiteRessource quantiteRessource(4);  // Indice commence à 4
  quantiteRessource.afficher();
  std::cout << "\n -------------------------\n" << std::endl;
  Machine machine(3);  // Indice commence à 3
  machine.afficher();
  std::cout << "\n -------------------------\n" << std::endl;
  Etape etape(4);  // Indice commence à 4
  etape.afficher();

  std::cout << "\n -------------------------\n" << std::endl;
  Produit produit(2);  // Indice commence à 2
  produit.afficher();

  std::cout << "\n -------------------------\n" << std::endl;
  Stock stock(2);
  stock.afficher();

  std::cout << "\n -------------------------\n" << std::endl;
  SiegeSocial siegeSocial(1);
  siegeSocial.afficher();

  std::cout << "\n -------------------------\n" << std::endl;
  Usine usine(1);
  usine.afficher();
  std::cout << "\n -------------------------\n" << std::endl;
  return 0;
}
