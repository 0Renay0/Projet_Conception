# Create your models here.
from django.db import models

# Modèle représentant une ville avec nom, code postal et prix au mètre carré
class Ville(models.Model):
    nom = models.CharField(max_length=100)
    code_postal = models.IntegerField()
    prix_m_2 = models.IntegerField()


# Classes abstraites

# Modèle abstrait pour les locaux (ex : usine, siège social)
class Local(models.Model):
    nom = models.CharField(max_length=100)
    surface = models.IntegerField()
    ville = models.ForeignKey(
        Ville,  # Association avec une ville
        on_delete=models.PROTECT, # La suppression d'une ville n'entraînera pas celle du local
    )

    def __str__(self):
        return self.nom

    class Meta:
        abstract = True # Indique que cette classe est abstraite


# Modèle abstrait pour les objets (ex : ressource, produit)
class Objet(models.Model):
    nom = models.CharField(max_length=100)
    prix = models.IntegerField()

    def __str__(self):
        return self.nom

    class Meta:
        abstract = True  # Indique que cette classe est abstraite


# Classes concrètes (non-abstraites)

# Modèle représentant un siège social, héritant de la classe Local
class SiegeSocial(Local):
    pass


# Modèle représentant une machine avec son nom, prix et numéro de série
class Machine(models.Model):
    nom = models.CharField(max_length=100)
    prix = models.IntegerField()
    n_serie = models.IntegerField()

    def __str__(self):
        return self.nom


# Modèle représentant une usine, qui possède plusieurs machines
class Usine(Local):
    machines = models.ManyToManyField(Machine)


# Modèle représentant une ressource, qui hérite d'Objet
class Ressource(Objet):
    pass


# Modèle représentant la quantité d'une ressource spécifique
class QuantiteRessource(models.Model):
    quantite = models.IntegerField()  # Quantité de la ressource
    ressource = models.ForeignKey(
        Ressource,  # Association avec une ressource
        on_delete=models.PROTECT,  # La suppression d'une ressource n'entraîne pas celle de la quantité
    )

    def __str__(self):
        return self.ressource


# Modèle représentant le stock d'un objet
class Stock(models.Model):
    objet = models.ForeignKey(
        Objet,  # Association avec un objet
        on_delete=models.PROTECT,  # La suppression d'un objet n'entraîne pas celle du stock
    )
    nombre = models.IntegerField()  # Nombre d'objets en stock

    def __str__(self):
        return str(self.objet)


# Modèle représentant une étape dans un processus de production
class Etape(models.Model):
    nom = models.CharField(max_length=100)  # Nom de l'étape
    duree = models.IntegerField()  # Durée de l'étape
    quantite_ressource = models.ForeignKey(
        QuantiteRessource, # Ressource requise pour cette étape
        on_delete=models.PROTECT,  # La suppression d'une ressource n'entraîne pas celle de l'étape
    )

    machine = models.ForeignKey(
        Machine,  # Machine utilisée pour cette étape
        on_delete=models.PROTECT,  # La suppression d'une machine n'entraîne pas celle de l'étape
    )

    etape_suivante = models.ForeignKey(
        "self",  # Référence à l'étape suivante (relation récursive)
        on_delete=models.PROTECT,
    )

    def __str__(self):
        return self.nom
        
        
# Modèle représentant un produit qui a une première étape de production
class Produit(Objet):
    premiere_etape = models.ForeignKey(
        Etape,  # Première étape de fabrication du produit
        on_delete=models.PROTECT,  # La suppression d'une étape n'entraîne pas celle du produit
    )
