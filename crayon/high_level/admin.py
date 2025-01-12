"""
Ce fichier permet l'intégration des modeles qu'on a définis dans le fichier models.py
avec l'interface d'administration de Django. Ainsi, on peut gérer les données via l'interface
admin accessible depuis le navigateur.
"""

# Register your models here.
from django.contrib import admin

from . import models  # Importation des modèles

# Enregistrement des modèles
admin.site.register(models.Ville)
admin.site.register(models.Machine)
admin.site.register(models.SiegeSocial)
admin.site.register(models.Usine)
admin.site.register(models.Ressource)
admin.site.register(models.QuantiteRessource)
admin.site.register(models.Stock)
admin.site.register(models.Etape)
admin.site.register(models.Produit)
