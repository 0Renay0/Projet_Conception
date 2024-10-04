#include <cpr/cpr.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <string>
using namespace std;

class Ville {
 public:
  string nom;
  int code_postal;
  float prix_m_2;

 public:
  // Constructeur
  Ville(string n, int cp, float p) {
    nom = n;
    code_postal = cp;
    prix_m_2 = p;
  }

  // Constructeur JSON data
  Ville(json data) : Ville(data["nom"], data["CP"], data["Prix/m^2"]) {}

  // Constructeur ID qui fera lui meme la requette
  Ville(int id) {
    cpr::Response r =
        cpr::Get(cpr::Url{"http://localhost:8000/Ville/" + to_string(id)});
    r.status_code;  // 200 Ok si 404 not found
    r.text;         // JSON text string

    json data = json::parse(r.text);

    // Ville(data);

    nom = data["Nom"];
    code_postal = data["CP"];
    prix_m_2 = data["Prix/m^2"];
  }

  // Methodes
  //  methode equivalente à la methode str en models.py
  friend std::ostream& operator<<(std::ostream& out, const Ville& v) {
    return out << v.nom;
  }
};

class Local {
 public:
  string nom;
  int surface;
  Ville* ville;  // Pointeur
};

// Classe abstraite Objet
class Objet {
 public:
  string nom;
  int prix;
};

/*
std::unique_ptr<A> pa = std::make_unique<A>(..... parametre du const ....);
const auto pa = std::make_unique<A>(......) dans la vrai vie
*/

// Fonction main
auto main() -> int {
  const auto v = Ville("Toulouse", 31400, 2000);
  std::cout << "Ville: " << v << std ::endl;

  /*
    cpr::Response r = cpr::Get(cpr::Url{"http://localhost:8000/Ville/1/"});


    r.status_code;             // 200 Ok si 404 not found
    r.header["content-type"];  // application/json; charset=utf-8
    r.text;                    // JSON text string

    std::cout << r.status_code
              << std::endl;  // verification si status code est OK ou pas
    std::cout << r.text << std::endl;

    json ville = json::parse(r.text);
    // verification
    std::cout << ville["CP"] << std::endl;  // Regarder le Readme JSON

  */
  const auto v1 = Ville(1);
  std::cout << v1 << std::endl;

  std::unique_ptr<Ville> pLocal = std::make_unique<Local>()

      return 0;
}

/*y
class Usine{
std::unique_ptr<Ville> ville;
}

dans cette classe, on definit un pointeur unique qui indique que une unsine peut
exister que dans une seule ville.

*/
