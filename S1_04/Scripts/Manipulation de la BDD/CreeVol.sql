SET @Nb_Avion = 0;
SET @Identifiant_Vol = "0000 0000 0000 0000";
SET @Aero_Depart = "Orly";
SET @Aero_Arrive = "Felix Eboue";
SET @Horaire_Arrive = "14:00:00";
SET @Horaire_Depart = "08:00:00";
SET @Tarif_Eco = 250.0; -- En Euros (€)
SET @Tarif_Eco_Hublot = 350.0; -- En Euros (€)
SET @Tarif_Prem = 400.0; -- En Euros (€)
SET @Tarif_Prem_Hublot = 450.0; -- En Euros (€)
SET @Bagages_Inclus = 0;
SET @Tarif_Ajout_1_Bagage = 50.0; -- En Euros (€)
SET @Tarif_Ajout_2_Bagages = 100.0; -- En Euros (€)
SET @Tarif_Ajout_3_Bagages = 150.0; -- En Euros (€)

INSERT INTO Vol VALUES (@Nb_Avion, @Identifiant_Vol, @Aero_Depart, @Aero_Arrive, @Horaire_Arrive, @Horaire_Depart, @Tarif_Eco, @Tarif_Eco_Hublot, @Tarif_Prem, @Tarif_Prem_Hublot, @Bagages_Inclus, @Tarif_Ajout_1_Bagage, @Tarif_Ajout_2_Bagages, @Tarif_Ajout_3_Bagages);