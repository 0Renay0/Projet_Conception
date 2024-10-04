#include <cpr/cpr.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <string>
using namespace std;

class Ville {
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

  // Methodes
  //  methode equivalente à la methode str en models.py
  friend std::ostream& operator<<(std::ostream& out, const Ville& v) {
    return out << v.nom;
  }
};

// Fonction main
auto main() -> int {
  const auto v = Ville("Toulouse", 31400, 2000);
  std::cout << "Ville: " << v << std ::endl;

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

  return 0;
}
