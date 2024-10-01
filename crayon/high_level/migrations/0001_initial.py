# Generated by Django 5.1.1 on 2024-10-01 06:12

import django.db.models.deletion
from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Machine',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('nom', models.CharField(max_length=100)),
                ('prix', models.IntegerField()),
                ('n_serie', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='QuantiteRessource',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('quantite', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='Ressource',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('nom', models.CharField(max_length=100)),
                ('prix', models.IntegerField()),
            ],
            options={
                'abstract': False,
            },
        ),
        migrations.CreateModel(
            name='Ville',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('nom', models.CharField(max_length=100)),
                ('code_postal', models.IntegerField()),
                ('prix_m_2', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='Etape',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('nom', models.CharField(max_length=100)),
                ('duree', models.IntegerField()),
                ('etape_suivante', models.ForeignKey(on_delete=django.db.models.deletion.PROTECT, to='high_level.etape')),
                ('machine', models.ForeignKey(on_delete=django.db.models.deletion.PROTECT, to='high_level.machine')),
                ('quantite_ressource', models.ForeignKey(on_delete=django.db.models.deletion.PROTECT, to='high_level.quantiteressource')),
            ],
        ),
        migrations.CreateModel(
            name='Produit',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('nom', models.CharField(max_length=100)),
                ('prix', models.IntegerField()),
                ('premiere_etape', models.ForeignKey(on_delete=django.db.models.deletion.PROTECT, to='high_level.etape')),
            ],
            options={
                'abstract': False,
            },
        ),
        migrations.AddField(
            model_name='quantiteressource',
            name='ressource',
            field=models.ForeignKey(on_delete=django.db.models.deletion.PROTECT, to='high_level.ressource'),
        ),
        migrations.CreateModel(
            name='Stock',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('nombre', models.IntegerField()),
                ('ressource', models.ForeignKey(on_delete=django.db.models.deletion.PROTECT, to='high_level.ressource')),
            ],
        ),
        migrations.CreateModel(
            name='Usine',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('nom', models.CharField(max_length=100)),
                ('surface', models.IntegerField()),
                ('machines', models.ManyToManyField(to='high_level.machine')),
                ('ville', models.ForeignKey(on_delete=django.db.models.deletion.PROTECT, to='high_level.ville')),
            ],
            options={
                'abstract': False,
            },
        ),
        migrations.CreateModel(
            name='SiegeSocial',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('nom', models.CharField(max_length=100)),
                ('surface', models.IntegerField()),
                ('ville', models.ForeignKey(on_delete=django.db.models.deletion.PROTECT, to='high_level.ville')),
            ],
            options={
                'abstract': False,
            },
        ),
    ]
