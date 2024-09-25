# Create your tests here.
from django.test import TestCase
from .models import Machine


class MachineModelTests(TestCase):
    def test_machine_creation(self):
        self.assertEqual(Machine.objects.count(), 0)
        Machine.objects.create(nom="scie", prix=1000, n_serie="16832")
        self.assertEqual(Machine.objects.count(), 1)
        # basculer entre 1 et 2 Pour mieux visualiser et interpreter le test
