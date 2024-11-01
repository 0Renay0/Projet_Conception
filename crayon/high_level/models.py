# Create your models here.
from django.db import models


### def costs dans stock / usine / machines / ensuite faire la somme
# Modèle représentant une ville avec nom, code postal et prix au mètre carré
class Ville(models.Model):
    nom = models.CharField(max_length=100)
    code_postal = models.IntegerField()
    prix_m_2 = models.IntegerField()

    def __str__(self):
        return self.nom

    def json(self):
        d = {"Nom": self.nom, "CP": self.code_postal, "Prix/m^2": self.prix_m_2}
        return d

    def json_extended(self):
        return self.json()


# Classes abstraites


# Modèle abstrait pour les locaux (ex : usine, siège social)
class Local(models.Model):
    nom = models.CharField(max_length=100)
    surface = models.IntegerField()
    ville = models.ForeignKey(
        Ville,  # Association avec une ville
        on_delete=models.PROTECT,  # La suppression d'une ville n'entraînera pas celle du local
    )

    def __str__(self):
        return self.nom

    class Meta:
        abstract = True  # Indique que cette classe est abstraite


# Modèle abstrait pour les objets (ex : ressource, produit)
class Objet(models.Model):
    nom = models.CharField(max_length=100)
    prix = models.IntegerField()

    def __str__(self):
        return self.nom

    class Meta:
        abstract = True  # Indique que cette classe est abstraite


# Classes concrètes (non-abstraites)


# Modèle représentant une ressource, qui hérite d'Objet
class Ressource(Objet):
    def json(self):
        d = {"Nom": self.nom, "Prix": self.prix}
        return d

    def json_extended(self):
        return self.json()


# Modèle représentant la quantité d'une ressource spécifique
class QuantiteRessource(models.Model):
    quantite = models.IntegerField()  # Quantité de la ressource
    ressource = models.ForeignKey(
        Ressource,  # Association avec une ressource
        on_delete=models.PROTECT,  # La suppression d'une ressource n'entraîne pas celle de la quantité
    )

    def __str__(self):
        return self.ressource.nom

    def costs(self):
        Total = self.ressource.prix * self.quantite
        return Total

    def json(self):
        d = {"Resource": self.ressource.id, "Quantite": self.quantite}
        return d

    def json_extended(self):
        d = {"Resource": self.ressource.json_extended(), "Quantite": self.quantite}
        return d


# Modèle représentant un siège social, héritant de la classe Local
class SiegeSocial(Local):
    def json(self):
        d = {"Nom": self.nom, "Surface": self.surface, "Ville": self.ville.id}
        return d

    def json_extended(self):
        d = {
            "Nom": self.nom,
            "Surface": self.surface,
            "Ville": self.ville.json_extended(),
        }
        return d


# Modèle représentant une machine avec son nom, prix et numéro de série
class Machine(models.Model):
    nom = models.CharField(max_length=100)
    prix = models.IntegerField()
    n_serie = models.IntegerField()

    def __str__(self):
        return self.nom

    def costs(self):
        return self.prix

    def json(self):
        d = {"Nom": self.nom, "Prix": self.prix, "n° de serie": self.n_serie}
        return d

    def json_extended(self):
        return self.json()


# Modèle représentant une étape dans un processus de production
class Etape(models.Model):
    nom = models.CharField(max_length=100)  # Nom de l'étape
    duree = models.IntegerField()  # Durée de l'étape
    quantite_ressource = models.ForeignKey(
        QuantiteRessource,  # Ressource requise pour cette étape
        on_delete=models.PROTECT,  # La suppression d'une ressource n'entraîne pas celle de l'étape
        blank=True,
        null=True,
    )

    machine = models.ForeignKey(
        Machine,  # Machine utilisée pour cette étape
        on_delete=models.PROTECT,  # La suppression d'une machine n'entraîne pas celle de l'étape
    )

    etape_suivante = models.ForeignKey(
        "self",  # Référence à l'étape suivante (relation récursive vers la classe elle meme)
        blank=True,
        null=True,
        on_delete=models.PROTECT,
    )

    def __str__(self):
        return self.nom

    def json(self):
        d = {
            "Nom": self.nom,
            "Durée": self.duree,
            "Quantité necessaire": self.quantite_ressource.id
            if self.quantite_ressource
            else None,
            "Machine ID": self.machine.id if self.machine else None,
            "Etape suivante ID": self.etape_suivante.id
            if self.etape_suivante
            else None,
        }
        return d

    def json_extended(self):
        d = {
            "Nom": self.nom,
            "Durée": self.duree,
            "Quantité necessaire": self.quantite_ressource.json_extended()
            if self.quantite_ressource
            else None,
            "Machine": self.machine.json_extended() if self.machine else None,
            "Etape suivante": self.etape_suivante.json_extended()
            if self.etape_suivante
            else None,
        }
        return d


# Modèle représentant un produit qui a une première étape de production
class Produit(Objet):
    premiere_etape = models.ForeignKey(
        Etape,  # Première étape de fabrication du produit
        on_delete=models.PROTECT,
    )

    def json(self):
        d = {
            "Nom": self.nom,
            "Prix": self.prix,
            "Premiere etape": self.premiere_etape.id,
        }
        return d

    def json_extended(self):
        d = {
            "Nom": self.nom,
            "Prix": self.prix,
            "Premiere etape": self.premiere_etape.json_extended(),
        }
        return d


# Modèle représentant une usine, qui possède plusieurs machines
class Usine(Local):
    machines = models.ManyToManyField(Machine)
    produit = models.ManyToManyField(Produit)
    Siege_Social = models.ForeignKey(
        SiegeSocial,
        on_delete=models.PROTECT,
        null=True,
        blank=True,
    )

    def costs(self):
        Prix_terrain = self.ville.prix_m_2 * self.surface
        Prix_machines = 0  # Initialisation

        for machines in self.machines.all():
            Prix_machines = Prix_machines + machines.prix

        return Prix_terrain + Prix_machines

    def Acheter_Ressources(self, produit, nombre_crayon):
        ressources_manquantes = {}
        # On parcourt les étapes du produit pour déterminer la quantité des ressources manquantes
        etape = produit.premiere_etape

        while etape:
            # Vérifie que l'étape nécessite des ressources (quantite_ressource n'est pas None)
            if etape.quantite_ressource:
                quantite_necessaire = etape.quantite_ressource.quantite * nombre_crayon
                ressource = etape.quantite_ressource.ressource
                # On vérifie le stock disponible
                stock = self.stock.filter(ressource=ressource).first()
                quantite_disponible = stock.nombre if stock else 0
                # Si la quantité est insuffisante, on achète la différence
                if quantite_necessaire > quantite_disponible:
                    ressources_manquantes[ressource] = (
                        quantite_necessaire - quantite_disponible
                    )

            # Passe à l'étape suivante, même si aucune ressource n'est nécessaire
            etape = etape.etape_suivante

        # Achat des ressources manquantes
        for ressource, quantite_a_acheter in ressources_manquantes.items():
            cout_total = quantite_a_acheter * ressource.prix
            self.acheter(ressource, quantite_a_acheter, cout_total)

        return ressources_manquantes

    def acheter(self, ressource, quantite, cout_total):
        stock, created = Stock.objects.get_or_create(usine=self, ressource=ressource)
        stock.nombre += quantite
        stock.save()

        return stock

    def json(self):
        nombre_crayon = (
            1000  # à modifier selon le besoin voir si on l'ajoute comme attribut
        )
        produit = Produit.objects.filter(pk=2).first()
        ressources_manquantes = self.Acheter_Ressources(produit, nombre_crayon)

        d = {
            "Machine": [machines.pk for machines in Machine.objects.all()],
            "Produit": [produit.pk for produit in Produit.objects.all()],
            "Siege Social": self.Siege_Social.id,
            "Cout Total": self.costs(),
            "Ressources manquantes": {
                str(ressource.id): quantite
                for ressource, quantite in ressources_manquantes.items()
            },
        }
        return d

    def json_extended(self):
        nombre_crayon = (
            1000  # à modifier selon le besoin voir si on l'ajoute comme attribut
        )
        produit = Produit.objects.filter(pk=2).first()
        ressources_manquantes = self.Acheter_Ressources(produit, nombre_crayon)

        d = {
            "Nom": self.nom,
            "Ville": self.ville.json_extended(),
            "Surface": self.surface,
            "Cout Total": self.costs(),
            "Siege Social": self.Siege_Social.json_extended(),
            "Machine": [machines.json_extended() for machines in Machine.objects.all()],
            "Produit": [produit.json_extended() for produit in Produit.objects.all()],
            "Ressources manquantes": {
                str(ressource.json_extended()): quantite
                for ressource, quantite in ressources_manquantes.items()
            },
        }
        return d


# Modèle représentant le stock d'un objet
class Stock(models.Model):
    ressource = models.ForeignKey(
        Ressource,  # Association avec une ressource
        on_delete=models.PROTECT,  # La suppression d'une ressource n'entraîne pas celle du stock
    )
    usine = models.ForeignKey(
        Usine,  # Association avec Usine
        on_delete=models.PROTECT,  # La suppression d'un stock ne supprime pas l'usine
        related_name="stock",
    )
    nombre = models.IntegerField()

    def __str__(self):
        return self.ressource.nom

    def costs(self):
        Prix_stock = self.nombre * self.ressource.prix
        return Prix_stock

    def json(self):
        d = {
            "Ressource ID ": self.ressource.id,
            "Usine ID": self.usine.id,
            "Nombre": self.nombre,
        }
        return d

    def json_extended(self):
        d = {
            "Ressource": self.ressource.json_extended(),
            "Usine": self.usine.json_extended(),
            "Nombre": self.nombre.json_extended(),
        }
        return d
