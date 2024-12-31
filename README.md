# Projet Crayon
Le **Projet Crayon** a été développé dans le cadre du module de **Conception de Systèmes Orientés Objet à temps réel** du Master 2 ISTR à l'Université Paul Sabatier.
![Université Paul Sabatier]((https://upload.wikimedia.org/wikipedia/fr/a/a4/Logo_UT3.jpg)

## Environnement virtuel 
Creer un environnement virtuel afin de ne pas toucher à vos propres modlues deja intallés. Pour cela, il faut suivre les etapes suivantes: 
```bash
python3 -m venv .venv
echo .venv >> .gitignore
source .venv/bin/activate
```
## Dependencies 
Dans le fichier "requirements.txt" vous trouverz l'ensemble des dependances necessaires pour ce projet. 
Afin d'installer l'ensemble des dependances, il suffit d'utiliser la commande suivante:
```bash
pip install -r /path/to/requirements.txt
```
## Installation
1- cloner le repertoire 
```bash
git clone https://github.com/github_username/repo_name.git
```
2- Changer le "git remote url" pour eviter les "push" vers le projet de base 
```bash
git remote set-url origin github_username/repo_name
git remote -v # confirm the changes
```
## Fonctionnalité 1 -- High level
Création de l’interface d’administration
```bash
./manage.py makemigrations
./manage.py migrate
./manage.py createsuperuser
./manage.py runserver
```
### Utilisation de l’interface d’administration
Pour utiliser l'interface, il faut se rendre au lien suivant: 
http://localhost:8000/admin

### Accees au données des objets
Se rendre au lien suivant:
http://localhost:8000/Object_name/Object_ID/

## Fonctionnalité 2 -- low level
Compilation du projet
```bash
cmake -B build -S .
cmake --build build
./build/low_level
echo build >> .gitignore
```

## Fonctionnalité 3 -- Test unitaire 
Pour executer les tests unitaires: 
```bash
./manage.py test
```
Pour acceder aux tests unitaire:
```bash
cd high_level
open tests.py
```

## Licence

Ce projet est sous licence [Eclipse Public License - v 2.0](LICENSE).

Vous pouvez utiliser, modifier et distribuer le programme conformément aux termes de cette licence. Consultez le fichier `LICENSE` dans le dépôt pour plus de détails.
![Licence: Eclipse Public License 2.0](https://img.shields.io/badge/License-EPL%202.0-blue.svg)

## Contact
- **Mail**: [rayen.hamadi@univ-tlse3.fr](mailto:rayen.hamadi@univ-tlse3.fr)
- **Mail**: [rayenhamadi48@gmail.com](mailto:rayenhamadi48@gmail.com)
