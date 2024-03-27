SET @Nom_Hebergeur = "Monique DUPONT";

SELECT * FROM AppartementMaison WHERE nb_appmaison = ANY(SELECT nb_hebergement FROM Hebergement WHERE type = "APPMAISON" AND nb_hebergeur = ANY(SELECT nb_hebergeur FROM Hebergeur WHERE nom_prenom = @Nom_Hebergeur));