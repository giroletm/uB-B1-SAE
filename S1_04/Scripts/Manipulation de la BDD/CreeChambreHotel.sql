SET @Nb_Hotel = 10;
SET @Type_Chambre = "Chambre 1 Personne";
SET @Superficie = 15; -- En m²
SET @Vue = "Vue sur cour";
SET @Tarif = 65.50; -- En Euros (€)
SET @Equipement = "Douche, Toilettes, Lavabo, Television";

INSERT INTO Chambre VALUES (0, @Nb_Hotel, @Type_Chambre, @Superficie, @Vue, @Tarif, @Equipement);