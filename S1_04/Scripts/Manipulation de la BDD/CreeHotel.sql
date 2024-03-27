SET @Nb_Hebergeur = 1;

SET @Nom = "Hotel de la mer";
SET @Localite = "Paris";
SET @Adresse = "10 rue de l'Hotel de ville";
SET @Nb_Etoile = 2;
SET @Nom_Contact = "Machin BIDULE";
SET @Num_Telephone = "+33725658596";
SET @Num_Tel2 = "+33386542598";
SET @Chaine_Hoteliere = "IBIS";
SET @Equipement = "Lit deux places, SDB, Douche, Toilettes, Television";
SET @Horaire_Arrivee = "12:00:00";
SET @Horaire_Depart = "10:00:00";
SET @Fumeur = 0;
SET @Animaux = 0;
SET @Enfant = 0;
SET @Fete = 0;
SET @Ptit_Dej = 1;
SET @Langues = "fr";
SET @Navette_Aeroport = 0;
SET @Resto_Sur_Place = 1;
SET @Formule_Resto = "Entree, Plat, Fromage ou dessert";
SET @Lits_Appoint = 1;
SET @Tarif_Lit_Appoint = 10.50;

INSERT INTO Hotel VALUES (0, @Nom, @Localite, @Adresse, @Nb_Etoile, @Nom_Contact, @Num_Telephone, @Num_Tel2, @Chaine_Hoteliere, @Equipement, @Horaire_Arrivee, @Horaire_Depart, @Fumeur, @Animaux, @Enfant, @Fete, @Ptit_Dej, @Langues, @Navette_Aeroport, @Resto_Sur_Place, @Formule_Resto, @Lits_Appoint, @Tarif_Lit_Appoint);
INSERT INTO Hebergement VALUES (0, (SELECT nb_hotel FROM Hotel ORDER BY nb_hotel DESC LIMIT 1), "HOTEL", @Nb_Hebergeur);