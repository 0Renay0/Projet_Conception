# Create your models here.
from django.db import models


class Ville(models.Model):
    nom = models.CharField(max_length=100)
    code_postal = models.IntegerField()
    prix_m_2 = models.IntegerField()


# Classes abstraites
class Local(models.Model):
    nom = models.CharField(max_length=100)
    surface = models.IntegerField()

    ville = models.ForeignKey(
        Ville,  # ou "self",
        on_delete=models.PROTECT,
        # blank=True, null=True,
        # related_name="+",
    )

    def __str__(self):
        return self.nom

    class meta:
        abstract = True


class Objet(models.Model):
    nom = models.CharField(max_length=100)
    prix = models.IntegerField()

    def __str__(self):
        return self.nom

    class meta:
        abstract = True


# Classes non abstraites


class SiegeSocial(Local):
    pass


class Machine(models.Model):
    nom = models.CharField(max_length=100)
    prix = models.IntegerField()
    n_serie = models.IntegerField()

    def __str__(self):
        return self.nom


class Usine(Local):
    machines = models.ManyToManyField(Machine)


class Ressource(Objet):
    pass


class QuantiteRessource(models.Model):
    # ressource = models.CharField(max_length=100)
    quantite = models.IntegerField()

    ressource = models.ForeignKey(
        Ressource,  # ou "self",
        on_delete=models.PROTECT,
        # blank=True, null=True,
        # related_name="+",
    )

    def __str__(self):
        return self.ressource


class Stock(models.Model):
    objet = models.CharField(max_length=100)
    nombre = models.IntegerField()

    def __str__(self):
        return self.objet


class Etape(models.Model):
    nom = models.CharField(max_length=100)
    duree = models.IntegerField()

    def __str__(self):
        return self.nom

    quantite_ressource = models.ForeignKey(
        QuantiteRessource,  # ou "self",
        on_delete=models.PROTECT,
        # blank=True, null=True,
        # related_name="+",
    )

    machine = models.ForeignKey(
        Machine,  # ou "self",
        on_delete=models.PROTECT,
        # blank=True, null=True,
        # related_name="+",
    )

    etape_suivante = models.ForeignKey(
        "self",
        on_delete=models.PROTECT,
        # blank=True, null=True,
        # related_name="+",
    )


class Produit(Objet):
    premiere_etape = models.ForeignKey(
        Etape,  # ou "self",
        on_delete=models.PROTECT,
        # blank=True, null=True,
        # related_name="+",
    )
