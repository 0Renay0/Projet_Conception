# Generated by Django 5.1.2 on 2024-10-31 00:09

import django.db.models.deletion
from django.db import migrations, models


class Migration(migrations.Migration):
    dependencies = [
        ("high_level", "0004_usine_siege_social"),
    ]

    operations = [
        migrations.AlterField(
            model_name="usine",
            name="Siege_Social",
            field=models.ForeignKey(
                blank=True,
                null=True,
                on_delete=django.db.models.deletion.PROTECT,
                to="high_level.siegesocial",
            ),
        ),
    ]
