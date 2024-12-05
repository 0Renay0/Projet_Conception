# Projet Crayon
Dans le cadre du Module de conception de systeme orientée objet M2-ISTR. 

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
