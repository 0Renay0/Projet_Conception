# Create your tests here.
from django.test import TestCase
from .models import Machine
from .models import Ville
from .models import Ressource
from .models import Usine
from .models import Stock
from .models import Etape
from .models import QuantiteRessource
from .models import Produit

class MachineModelTests(TestCase):
    def test_machine_creation(self):
        print("\n\nTest creation de machine [START] \n\n")
        self.assertEqual(Machine.objects.count(), 0)  # verifier qu'au debut on a rien
        M1 = Machine.objects.create(nom="scie", prix=1000, n_serie="16832")
        self.assertEqual(
            Machine.objects.count(), 1
        )  # basculer entre 1 et 2 Pour mieux visualiser et interpreter le test
        print(f"Nom de la machine : {M1.nom}")
        print(f"Prix de la machine : {M1.prix}")
        print(f"Numéro de série de la machine : {M1.n_serie}")
        print("\n\nTest creation de machine [END] \n\n")

    def test_usine_creation(self):
        print("\n\nTest creation de l'usine [START] \n\n")
        M1 = Machine.objects.create(nom="machine 1", prix=1000, n_serie="16832")
        M2 = Machine.objects.create(nom="machine 2", prix=2000, n_serie="26832")

        V1 = Ville.objects.create(nom="Labege", code_postal=31400, prix_m_2=2000)

        R1 = Ressource.objects.create(nom="bois", prix=10)
        R2 = Ressource.objects.create(nom="mine", prix=15)

        # QR1 = QuantiteRessource.objects.create(ressource=R1, quantite=5)
        # QR2 = QuantiteRessource.objects.create(ressource=R2, quantite=1)

        U1 = Usine.objects.create(nom="TLS1", ville=V1, surface=50)
        # Attribuer chaque machine à l'usine
        U1.machines1 = M1
        U1.machines2 = M2

        S1 = Stock.objects.create(ressource=R1, usine=U1, nombre=1000)
        S2 = Stock.objects.create(ressource=R2, usine=U1, nombre=50)

        Total = U1.costs() + S1.costs() + S2.costs() + M1.costs() + M2.costs()

        print("Cout usine 1 : ", Usine.objects.first().costs())
        print("Cout machine : ", M1.costs() + M2.costs())
        print("Cout stock : ", S1.costs() + S2.costs())
        print("Cout total de l'usine : ", Total)  # Usine.objects.first().costs()
        print("\n\nTest creation de l'usine [END] \n\n")
        
        
    def test_acheter_ressources(self):
        # Créez une ville (nécessaire pour l'usine) avec le code postal
        print("\n\nTest achat de ressources necessaires [START] \n\n")
        ville = Ville.objects.create(nom="Toulouse", prix_m_2=1000, code_postal="31400")
        
        # Créez les ressources
        ressource1 = Ressource.objects.create(nom="Bois", prix=10)
        ressource2 = Ressource.objects.create(nom="Graphite", prix=20)
        
        # Créez les machines avec le champ `n_serie`
        machine1 = Machine.objects.create(nom="Scie", prix=5000, n_serie="001")
        machine2 = Machine.objects.create(nom="Assembleuse", prix=7000, n_serie="002")
        
        # Créez les quantités de ressources associées
        quantite_ressource1 = QuantiteRessource.objects.create(ressource=ressource1, quantite=5)
        quantite_ressource2 = QuantiteRessource.objects.create(ressource=ressource2, quantite=3)
        
        # Créez les étapes du produit en utilisant les quantités de ressources et les machines
        etape1 = Etape.objects.create(
            nom="Coupe du bois", 
            quantite_ressource=quantite_ressource1,
            duree=10,  # Durée obligatoire
            machine=machine1  # Machine obligatoire
        )
        etape2 = Etape.objects.create(
            nom="Insertion de graphite", 
            quantite_ressource=quantite_ressource2,
            etape_suivante=etape1,
            duree=15,  # Durée obligatoire
            machine=machine2  # Machine obligatoire
        )
        
        # Créez le produit avec une première étape et un prix
        produit = Produit.objects.create(nom="Crayon", premiere_etape=etape2, prix=5)
        
        # Créez l'usine et ajoutez un stock initial
        usine = Usine.objects.create(nom="Usine de crayons", surface=500, ville=ville)
        Stock.objects.create(usine=usine, ressource=ressource1, nombre=10)
        Stock.objects.create(usine=usine, ressource=ressource2, nombre=2)
        
        # Définissez le nombre de crayons à fabriquer
        nombre_crayon = 10
        print(f"Nombre de crayons à fabriquer : {nombre_crayon}")
        
        # Affichez le stock avant l'achat
        print("Stock avant achat :")
        for stock in usine.stock.all():
            print(f"Ressource : {stock.ressource.nom}, Quantité : {stock.nombre}")
        
        # Appel de la méthode avec le produit et une quantité de crayons
        ressources_manquantes = usine.Acheter_Ressources(produit=produit, nombre_crayon=nombre_crayon)
        
        # Affichez les ressources manquantes après calcul
        print("Ressources manquantes :")
        for ressource, quantite in ressources_manquantes.items():
            print(f"Ressource : {ressource.nom}, Quantité à acheter : {quantite}")
        
        # Vérifiez que les ressources ont été achetées et ajoutées au stock
        print("Stock après achat :")
        for stock in usine.stock.all():
            print(f"Ressource : {stock.ressource.nom}, Quantité : {stock.nombre}")
        
        # Assertions pour vérifier les quantités
        stock1 = Stock.objects.get(usine=usine, ressource=ressource1)
        stock2 = Stock.objects.get(usine=usine, ressource=ressource2)
        
        self.assertEqual(stock1.nombre, 50)  # 10 existant + 40 achetés
        self.assertEqual(stock2.nombre, 30)  # 2 existant + 28 achetés
        print("\n\nTest achat de ressources necessaires [END] \n\n")



