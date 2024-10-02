# Create your tests here.
from django.test import TestCase
from .models import Machine
from .models import Ville
from .models import Ressource
from .models import Usine
from .models import Stock


class MachineModelTests(TestCase):
    def test_machine_creation(self):
    	print("Test creation de machine [START] \n \n")
    	self.assertEqual(Machine.objects.count(), 0)  # verifier qu'au debut on a rien
    	Machine.objects.create(nom="scie", prix=1000, n_serie="16832")
    	self.assertEqual(Machine.objects.count(), 1) # basculer entre 1 et 2 Pour mieux visualiser et interpreter le test
    	print("Test creation de machine [END] \n \n")

    def test_usine_creation(self):
        print("Test creation de l'usine [START] \n \n")
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
        print("Test creation de l'usine [END] \n \n")
