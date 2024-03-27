SET @Nom_Hebergeur = "Monique DUPONT";

SELECT * FROM AppartHotel WHERE nb_apphotel = ANY(SELECT nb_hebergement FROM Hebergement WHERE type = "APPHOTEL" AND nb_hebergeur = ANY(SELECT nb_hebergeur FROM Hebergeur WHERE nom_prenom = @Nom_Hebergeur));