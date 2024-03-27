SET @Nom_Prenom = "Truc MUCHE";
SET @Adresse = "2 rue du petit bois";
SET @Num_Tel = "+33789632514";
SET @Email = "darktrucmuche@gmail.com";
SET @MDP = "5994471abb01112afcc18159f6cc74b4f511b99806da89b3caf5a9c174cacfc2"; -- Sous la forme d'un hash SHA-256

INSERT INTO Hebergeur VALUES (0, @Nom_Prenom, @Adresse, @Num_Tel, @Email, @MDP);