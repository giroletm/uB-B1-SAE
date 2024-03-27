SET @Nb_Hotel = 1;

SET @Emplacement_Hebergeables = "Chambre";
SET @Nb_Personnes_Hebergeables = 2;
SET @Nb_SDB = 1;
SET @Superficie = 30; -- En m²
SET @Nb_Chambre = 1;
SET @Noms_Photos = "apphotel-app1.png|apphotel-app1.png";

INSERT INTO HotelAppart VALUES (0, @Emplacement_Hebergeables, @Nb_Personnes_Hebergeables, @Nb_SDB, @Superficie, @Nb_Chambre, @Noms_Photos);
INSERT INTO COMPRENDSAPPART VALUES ((SELECT nb_appart FROM HotelAppart ORDER BY nb_appart DESC LIMIT 1), @Nb_Hotel);