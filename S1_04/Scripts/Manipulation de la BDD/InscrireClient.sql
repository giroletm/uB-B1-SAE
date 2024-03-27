SET @Nom_Prenom = "Jean DUPONT";
SET @Adresse = "5 rue des oies";
SET @Num_Tel = "+33736211227";
SET @Email = "jean.dupont@gmail.com";
SET @MDP = "5994471abb01112afcc18159f6cc74b4f511b99806da59b3caf5a9c173cacfc5"; -- Sous la forme d'un hash SHA-256
SET @DateNaissance = "1865-04-05";
SET @Sexe = "M";
SET @Langue = "fr"; -- Sous la forme d'un code ISO 639-1

INSERT INTO Client VALUES (0, @Nom_Prenom, @Adresse, @Num_Tel, @Email, @MDP, @DateNaissance, @Sexe, @Langue);