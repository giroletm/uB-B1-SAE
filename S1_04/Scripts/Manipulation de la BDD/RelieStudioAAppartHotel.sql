SET @Nb_Hotel = 1;

SET @Nb_Lits_simples = 1;
SET @Nb_Lits_doubles = 0;
SET @Nb_Personnes = 1;
SET @Superficie = 30; -- En m²
SET @Noms_Photos = "apphotel-studio1.png|apphotel-studio1.png";

INSERT INTO HotelStudio VALUES (0, @Nb_Lits_simples, @Nb_Lits_doubles, @Nb_Personnes, @Superficie, @Noms_Photos);
INSERT INTO COMPRENDSSTUDIO VALUES ((SELECT nb_studio FROM HotelStudio ORDER BY nb_studio DESC LIMIT 1), @Nb_Hotel);