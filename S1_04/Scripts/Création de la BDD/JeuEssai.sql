INSERT INTO AppartementMaison VALUES (1, "Maison de la chouette", "France", "3 rue des patates", "Deux chambres, salon", 5, 1, 40, "Cuisine, Television, Wi-Fi", 1, 0, 0, 1, 1, 0, "Petite maison a double etage dans un coin de foret", "maisonchouette-dehors.png|maisonchouette-dedans.png");
INSERT INTO AppartementMaison VALUES (2, "Logement coup-de-coeur", "France", "637 impasse du vieux chene", "Une chambre", 1, 1, 20, "Television, Wi-Fi", 1, 1, 0, 0, 1, 1, "Logement singulier presentant un bon rapport qualite/prix, pour tout type de personne", "logementcdc.jpg");
INSERT INTO AppartementMaison VALUES (3, "Appartement etudiant", "France", "32 avenue du boeuf de kobe", "Une chambre", 2, 0, 10, "Lave-linge, Wi-Fi", 0, 0, 1, 0, 0, 1, "Petit appartement pour etudiant", "appartetu1.png|appartetu2.png|appartetu3.png");

INSERT INTO Hotel VALUES (1, "Hotel du beau soleil", "Bourgogne-Franche-Comte", "96 place du sourire", 3, "Marco DIAZ", "+33612345678", "", "Groupe MIAM", "Wi-Fi gratuit, Reception 24h/24", "04:00", "23:00", 0, 0, 1, 0, 1, "Francais, Anglais", 0, 1, "pension-complete", 0, 0.0);
INSERT INTO Hotel VALUES (2, "Hotel au petit ramonneur", "Normandie", "12 port du gros brochet", 1, "Bianca PITAINE", "+33386010203", "+33604050607", "Independant", "Chauffage, Bar", "06:00", "20:00", 1, 1, 0, 0, 0, "Francais", 0, 1, "demi-pension", 1, 15.0);

INSERT INTO AppartHotel VALUES (1, "Logement du pic glace", "Alpes", "Chemin de terre du moineau", "Wi-Fi gratuit, Chauffage, Sauna", "02:00", "22:00", 0, 1, 1, 0, 1, "Francais, Anglais, Espagnol", 1, 1, 0, 1, 0, 1, 0, 1, 112.0);
INSERT INTO AppartHotel VALUES (2, "Logement de ville", "Allie", "1971 rue des fleurs", "Terrasse, Jardin", "08:00", "20:00", 0, 0, 1, 0, 0, "Francais", 1, 0, 0, 1, 1, 0, 1, 1, 55.99);

INSERT INTO HotelAppart VALUES (1, "Deux chambres, salon", 5, 1, 20, 2, "picglaceapphotel.png");
INSERT INTO HotelStudio VALUES (1, 2, 0, 2, 20, "logvileapphotel.png");

INSERT INTO COMPRENDSAPPART VALUES (1, 1);
INSERT INTO COMPRENDSSTUDIO VALUES (1, 2);

INSERT INTO Chambre VALUES (1, 1, "Chambre classique", 20, "Vue sur la mer", 35.99, "Vue");
INSERT INTO Chambre VALUES (2, 1, "Chambre double", 40, "Vue sur la mer", 55.99, "Vue");
INSERT INTO Chambre VALUES (3, 2, "Chambre normale", 30, "Aucune", 70.0, "Television, Bouilloire electrique");
INSERT INTO Chambre VALUES (4, 2, "Chambre superieure", 50, "Vue sur la ville", 90.0, "Television, Bouilloire, Baignoire");
INSERT INTO Chambre VALUES (5, 2, "Chambre de luxe", 80, "Vue sur la ville", 110.0, "Television, Bouilloire, Baignoire, Balcon");

INSERT INTO Avion VALUES (1, "A220-100", 1);
INSERT INTO Avion VALUES (2, "A319neo", 2);
INSERT INTO Avion VALUES (3, "A350 XWB-900", 3);
INSERT INTO Avion VALUES (4, "A330-200F", 4);

INSERT INTO Siege VALUES (1, 1, 1, 1);
INSERT INTO Siege VALUES (2, 2, 0, 0);
INSERT INTO Siege VALUES (3, 2, 1, 0);
INSERT INTO Siege VALUES (4, 3, 1, 1);
INSERT INTO Siege VALUES (5, 3, 0, 0);
INSERT INTO Siege VALUES (6, 3, 0, 1);
INSERT INTO Siege VALUES (7, 4, 0, 0);
INSERT INTO Siege VALUES (8, 4, 0, 1);
INSERT INTO Siege VALUES (9, 4, 1, 0);
INSERT INTO Siege VALUES (10, 4, 1, 1);

INSERT INTO Vol VALUES ("FRUS 0000 CPP4 0801", 1, "Paris", "New York", "10:00", "16:00", 30.0, 40.0, 50.0, 70.0, 0, 60.0, 110.0, 160.0);
INSERT INTO Vol VALUES ("FRJP 0000 MNGA 2109", 4, "Paris", "Tokyo", "6:00", "18:00", 60.0, 65.0, 115.0, 130.0, 1, 30.0, 55.0, 80.0);

INSERT INTO Hebergeur VALUES (1, "Monique DUPONT", "28 rue du beau charmeur", "+33908070605", "moniquedupong@hotmail.com", "856e407ff24ffc53b85da20e033fe94f8396493761d20c5aef14441aa0c9f681");
INSERT INTO Hebergeur VALUES (2, "Henri CASTAFOLTE", "3 souterrain du passant", "+33112357894", "henri.castafolte@vdf.fr", "3a3e0ae1fe27735a8db6ae0060af663c66dc8d5295828e1206b34b74d50bb5bf");
INSERT INTO Hebergeur VALUES (3, "Danielle ROUSSEL", "33 boulevard de Normandie", "+33519806914", "danielleroussel@jourrapide.com", "07567d11b04ae73a2250b27c94ae49146818e07d8bd20bfdc9f7afba7186c579");
INSERT INTO Hebergeur VALUES (4, "Rosamonde BELLEFEUILLE", "2 rue Charles Corbeau", "+33217754232", "rosamondebellefeuille@rhyta.com", "2f72fa9fb3487ffbaf654cedfe13e9ddee0f24e3a7e453a15b858463cccdb515");
INSERT INTO Hebergeur VALUES (5, "Chantal BAZINET", "17 rue du General Ailleret", "+33235385349", "chantalbazinet@rhyta.com", "ec98cdff032fbc2c2eb1d3edfd66636477c1ebe488de7985ff64984e7cda7568");
INSERT INTO Hebergeur VALUES (6, "Arnaude THIVI", "30 Square de la Couronne", "+33124029297", "arnaudethivierge@jourrapide.com", "44d9c62ab336ee955a7c560507792f0b5089861fec4c9840c74a9d29fddc2e72");

INSERT INTO Client VALUES (1, "Wayne FERGUSON", "90 rue de la Mare aux Carats", "+33155597536", "wayne.ferguson@hotmail.com", "6b86b273ff34fce19d6b804eff5a3f5747ada4eaa22f1d49c01e52ddb7875b4b", "1988-10-14", "M", "fr");
INSERT INTO Client VALUES (2, "Hu LEBLANC", "59 rue de la Fosse des Tanneurs", "+33429558678", "leblanchu@hotmail.edu", "d4735e3a265e16eee03f59718b9b5d03019c07d8b6c51f90da3a666eec13ab35", "1975-10-14", "M", "fr");
INSERT INTO Client VALUES (3, "Gil CROMWELL", "59 rue Bonneterie", "+33471443947", "gil.chmwll@gmail.com", "4e07408562bedb8b60ce05c1decfe3ad16b72230967de01f640b7e4729b49fce", "1980-12-02", "M", "us");
INSERT INTO Client VALUES (4, "Odysseus AAAA", "29 avenue Voltaire", "+33873781894", "aaaa@protonmail.com", "4b227777d4dd1fc61c6f884f48641d02b4d121d3fd328cb08b5531fcacdabf8a", "1998-05-02", "F", "fr");
INSERT INTO Client VALUES (5, "Aurore LYNN", "37 avenue des pres", "+33556180844", "aurore@outlook.net", "ef2d127de37b942baad06145e54b0c619a1f22327b2ebbcfbec78f5564afe39d", "1974-10-18", "F", "fr");
INSERT INTO Client VALUES (6, "Madonna BRIEN", "76 place Napoleon", "+33255617743", "themadd@protonmail.ca", "e7f6c011776e8db7cd330b54174fd76f7d0216b612387a5ffcfb81e6f0919683", "1990-12-27", "F", "us");
INSERT INTO Client VALUES (7, "Vera JEUSEPA", "66 boulevard Amiral Courbet", "+33233748258", "jeusepaki@gmail.com", "7902699be42c8a8e46fbbb4501726517e86b22c56a189f7625a6da49081b2451", "1989-08-31", "F", "fr");
INSERT INTO Client VALUES (8, "Mara RIOS", "62 Cours Marechal-Joffre", "+33962492263", "maria.rios@yahoo.couk", "2c624232cdd221771294dfbb310aca000a0df6ac8b66b696d90ef06fdefb64a3", "1979-12-30", "M", "es");
INSERT INTO Client VALUES (9, "Ciaran PONTS", "74 rue du Faubourg National", "+33452514178", "pontsu@aol.com", "19581e27de7ced00ff1ce50b2047e7a567c76b1cbaebabe5ef03f7c3017bb5b7", "1974-09-29", "M", "fr");
INSERT INTO Client VALUES (10, "Barclay EEEE", "42 rue Bonnet", "+33225152858", "eeee@icloud.couk", "4a44dc15364204a80fe80e9039455cc1608281820fe2b24f1e5233ade6af1dd5", "1977-08-23", "F", "uk");

INSERT INTO Hebergement VALUES (1, 1, "APPMAISON", 1);
INSERT INTO Hebergement VALUES (2, 2, "APPMAISON", 1);
INSERT INTO Hebergement VALUES (3, 3, "APPMAISON", 1);
INSERT INTO Hebergement VALUES (4, 1, "HOTEL", 1);
INSERT INTO Hebergement VALUES (7, 2, "HOTEL", 2);
INSERT INTO Hebergement VALUES (5, 1, "APPHOTEL", 1);
INSERT INTO Hebergement VALUES (6, 2, "APPHOTEL", 1);

INSERT INTO ReservationHebergement VALUES (1, 1, "2023-02-25", "2023-02-26", 1, 0, "Marzy", "France", 0);
INSERT INTO ReservationVol VALUES (2, "FRJP 0000 MNGA 2109", "2023-03-12", "2023-03-12", 2, 0, 0, 0);
INSERT INTO ReservationSejour VALUES (3, 7, "FRUS 0000 CPP4 0801", "Formule IDP");