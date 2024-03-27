SET @Identifiant_Vol = "0000 0000 0000 0000";
SET @Date_Depart = "2023-10-04";
SET @Date_Arrive = "2023-10-04";
SET @Nb_Voyageurs = 1;
SET @Aller_Retour = 0;
SET @Prem_Classe = 0;
SET @Avec_Escale = 0;

INSERT INTO ReservationVol VALUES (0, @Identifiant_Vol, @Date_Depart, @Date_Arrive, @Nb_Voyageurs, @Aller_Retour, @Prem_Classe, @Avec_Escale);