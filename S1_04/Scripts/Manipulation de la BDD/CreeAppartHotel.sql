SET @Nb_Hebergeur = 1;

SET @Nom = "Appart-Hotel du saint donut";
SET @Localite = "Kelkeupar";
SET @Adresse = "9 rue d'auvergne";
SET @Equipements = "Television, Baignoire";
SET @Horaire_Arrivee = "10:00:00";
SET @Horaire_Depart = "08:00:00";
SET @Fumeur = 0;
SET @Animaux = 0;
SET @Enfant = 1;
SET @Fete = 0;
SET @Ptit_Dej = 0;
SET @Langues = "fr";
SET @Douche_ou_Baignoire = 1;
SET @Serviettes = 1;
SET @Seche_cheveux = 0;
SET @Micro_ondes = 0;
SET @Plaques_cuisson = 1;
SET @Ustensiles_cuisine = 1;
SET @Bouilloire = 0;
SET @Frigo = 1;
SET @Tarif = 65.99; -- En Euros (€)

INSERT INTO AppartHotel VALUES (0, @Nom, @Localite, @Adresse, @Equipements, @Horaire_Arrivee, @Horaire_Depart, @Fumeur, @Animaux, @Enfant, @Fete, @Ptit_Dej, @Langues, @Douche_ou_Baignoire, @Serviettes, @Seche_cheveux, @Micro_ondes, @Plaques_cuisson, @Ustensiles_cuisine, @Bouilloire, @Frigo, @Tarif);
INSERT INTO Hebergement VALUES (0, (SELECT nb_apphotel FROM AppartHotel ORDER BY nb_apphotel DESC LIMIT 1), "APPHOTEL", @Nb_Hebergeur);