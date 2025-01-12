"""
URL configuration for crayon project.

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/4.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""

from django.contrib import admin
from django.urls import path
from high_level import views

urlpatterns = [
    path("admin/", admin.site.urls),
    path(
        "Ville/<int:pk>/",
        views.VilleDetailView.as_view(),  # Indice commence à 1
        name="Ville",
    ),  # <int:pk> c'est pour indiquer l'id de la ville
    path(
        "QuantiteRessource/<int:pk>/",
        views.QuantiteRessourceDetailView.as_view(),  # Indice commence à 4
        name="Quantite de ressource",
    ),
    path(
        "SiegeSocial/<int:pk>/",
        views.SiegeSocialDetailView.as_view(),
        name="Siege Social",  # Indice commence à 1
    ),
    path(
        "Machine/<int:pk>/",
        views.MachineDetailView.as_view(),  # Indice commence à 3
        name="Machine",
    ),
    path(
        "Stock/<int:pk>/",
        views.StockDetailView.as_view(),  # Indice commence à 1
        name="Stock",
    ),
    path(
        "Etape/<int:pk>/",
        views.EtapeDetailView.as_view(),  # Indice commence à 3
        name="Etape",
    ),
    path(
        "Produit/<int:pk>/",
        views.ProduitDetailView.as_view(),  # Indice commence à 2
        name="Produit",
    ),
    path(
        "Usine/<int:pk>/",
        views.UsineDetailView.as_view(),  # Indice commence à 1
        name="Usine",
    ),
    path(
        "Ressource/<int:pk>/",
        views.RessourceDetailView.as_view(),  # Indice commence à 1
        name="Ressource",
    ),
    path(
        "Api/<int:pk>/",
        views.ApiDetailView.as_view(),  # Indice commence à 1
        name="Api",
    ),
]
