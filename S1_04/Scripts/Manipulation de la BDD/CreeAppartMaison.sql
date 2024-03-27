SET @Nb_Hebergeur = 1;

SET @Nom = "Maison de la Bellevue";
SET @Pays = "France";
SET @Adresse = "47 chemin de la Riviere";
SET @Emplacement_Hebergeables = 2;
SET @Nb_Salle_De_Bain = 1;
SET @Superficie = 20;
SET @Equipement = "Deux lits, Une SDB avec douche, Toilettes, Lavabo, Télévision, Volets électriques";
SET @Parking = 1;
SET @Parking_Payant = 0;
SET @Fumeur = 0;
SET @Animaux = 0;
SET @Enfants = 1;
SET @Fetes = 0;
SET @Commentaire = "aucun commentaire";
SET @Noms_Photos = "exemple1.png|exemple2.png";

INSERT INTO AppartementMaison VALUES (0, @Nom, @Pays, @Adresse, @Emplacement_Hebergeables, @Nb_Salle_De_Bain, @Superficie, @Equipement, @Parking, @Parking_Payant, @Fumeur, @Animaux, @Enfants, @Fetes, @Commentaire, @Noms_Photos);
INSERT INTO Hebergement VALUES (0, (SELECT nb_appmaison FROM AppartementMaison ORDER BY nb_appmaison DESC LIMIT 1), "APPMAISON", @Nb_Hebergeur);