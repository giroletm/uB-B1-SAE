SET @Nb_Hebergement = 1;
SET @Date_Arrivee = "2023-10-04";
SET @Date_Depart = "2023-10-04";
SET @Nb_Adultes = 1;
SET @Nb_Enfants = 2;
SET @Ville = "Paris";
SET @Pays = "France";
SET @Remboursable = 1;

INSERT INTO Hebergement VALUES (0, @Nb_Hebergement, @Date_Arrivee, @Date_Depart, @Nb_Adultes, @Nb_Enfants, @Ville, @Pays, @Remboursable);