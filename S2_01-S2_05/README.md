# SAÉ 2.01 & 2.05 - Développement d'une application & Gestion d'un projet

[English version](https://github.com/giroletm/uB-B1-SAE/tree/master/S2_01-S2_05/README_en.md)

![Logo du jeu](https://github.com/giroletm/uB-B1-SAE/tree/master/S2_01-S2_05/game/common/assets/UI/Logo.png?raw=true)

# Sujet

Contraintes :
- L'interface principale du jeu est un plateau d'[au moins] 10 cases par 10 sur lequel se passe la plupart des actions.
- Les cases peuvent contenir des éléments, des personnages, des monstres, des bâtiments, des objets, des coffres, ...
- Les 4 touches directionnelles du clavier doivent avoir une action sur ce plateau (déplacer un personnage, choisir une case, ...)
- En dehors du plateau doivent se trouver des informations sur l'état du jeu (points de vie, inventaire, ressources disponibles restantes, ...)
- En dehors du plateau doivent également se trouver un ou plusieurs boutons qui auront des actions sur le jeu (Boire une potion, construire un bâtiment, ...)
- Vous devrez inclure un système de drag and drop quelque part dans l'application.
- Le jeu devra posséder des règles qui seront expliquées au joueur, un but et un écran de fin. Vous êtes libre d'inventer vos règles.

À rendre :
- Vous devrez héberger votre code sur github et planifier vos tâches sur trello.
- Vous devrez concevoir un diagramme UML.
- Pas besoin de rapport, mais des slides de soutenance.

[...]

# Structure

Le repo est structuré de la manière suivante :
* **game** : Ce dossier contient le code du jeu
* **editor** : Ce dossier contient le code de l'éditeur de maps

# Crédits

*Les noms de famille ont volontairement été omis. Si vous voulez plus d'informations sur les personnes citées, consultez leur profil.*

- Sujet : [IUT de Dijon / Auxerre / Nevers](https://iutdijon.u-bourgogne.fr/www)
- Mécaniques principales, système d'acteurs, machine d'états, système de carte, portages, scénario : [Matthieu](https://github.com/giroletm) (moi)
- Système de vie, PNJs, dialogues, boîtes de texte, scénario : [Agathe](https://github.com/D4cta)
- Inventaires, cartes, design des personnages, scénario : [Tahar](https://github.com/TaharChtioui)
- Bande son originale : Étienne
- Moteur de rendu graphique [SDL2](https://github.com/libsdl-org/SDL) et modules : [Sam LANTINGA](https://github.com/slouken) & [Contributeurs](https://github.com/libsdl-org/SDL/graphs/contributors)
- Sprites : [RPG Maker](https://www.rpgmakerweb.com/)
- Effets sonores : [Undertale](https://undertale.com/), [Minecraft](https://www.minecraft.net/)

# Exception au droit d'auteur

```
Lorsque l'oeuvre a été divulguée, l'auteur ne peut interdire :

3° Sous réserve que soient indiqués clairement le nom de l'auteur et la source :

a) Les analyses et courtes citations justifiées par le caractère critique, polémique, pédagogique, scientifique ou d'information de l'oeuvre à laquelle elles sont incorporées ;
```

Source: [Article L. 122-5 du code de la propriété intellectuelle](https://www.legifrance.gouv.fr/codes/section_lc/LEGITEXT000006069414/LEGISCTA000006161637/?anchor=LEGIARTI000048603495#LEGIARTI000048603495)

Ce projet rentre dans l'exception citée, puisque :
- Il s'agit d'un projet universitaire, donc à caractère pédagogique
- Ne sont utilisés que très peu de contenu protégé par rapport à la taille des oeuvres dont il vient, permettant ainsi de qualifier les utilisations de "courtes citations".

Ce projet n'est en aucun cas utilisé à des fins commerciales, il n'existe qu'à but pédagogique.

Celui-ci n'empiète d'aucune manière sur le commerce des personnes ayant produit les contenus protégés utilisés.