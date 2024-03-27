SET @Nom_Hebergeur = "Monique DUPONT";

SELECT * FROM Hotel WHERE nb_hotel = ANY(SELECT nb_hebergement FROM Hebergement WHERE type = "HOTEL" AND nb_hebergeur = ANY(SELECT nb_hebergeur FROM Hebergeur WHERE nom_prenom = @Nom_Hebergeur));