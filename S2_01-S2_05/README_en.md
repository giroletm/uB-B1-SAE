# SAÉ 2.01 & 2.05 - Developing an application & Managing a project

[Version française](https://github.com/giroletm/uB-B1-SAE/tree/master/S2_01-S2_05/README.md)

This version was translated from french.

![The game's logo](https://github.com/giroletm/uB-B1-SAE/tree/master/S2_01-S2_05/game/common/assets/UI/Logo.png?raw=true)

# Subject

Constraints:
- The game's main UI is a grid of [at least] 10 by 10 squares on which most actions happen.
- Squares can contain elements, charactersn monsters, buildings, objects, chests, ...
- The keyboard's four directional keys must perform an action on the grid (move a character, choose a square, ...)
- Outside of the grid, you need to put additional information about the game's state (health points, inventory, available resources, ...)
- Outside of the grid, you must also add one or more buttons that will act on the game (drink a potion, build a building, ...)
- You need to include a drag-and-drop system somewhere in the app.
- The game will have rules that needs to be explained to the player, a goal and an end screen. You are free to make up your own rules.

To deliver:
- You need to host your code on GitHub and plan your tasks on Trello.
- You must make a UML diagram.
- No need to write a report, but there will be a presentation to make.

[...]

# Structure

The repo is structured as follows:
* **game**: This folder contains the game's code
* **editor**: This folder contains the map editor's code

# Credits

*Full names have voluntarely been truncated to first names only. If you want more information about the people involved, check out their profile.*

- Subject: [IUT de Dijon / Auxerre / Nevers](https://iutdijon.u-bourgogne.fr/www)
- Core game mechanics, actor system, state machine, map system, ports, scenario : [Matthieu](https://github.com/giroletm) (me)
- Health system, NPCs, dialogues, textboxes, scenario : [Agathe](https://github.com/D4cta)
- Inventories, maps, character design, scenario : [Tahar](https://github.com/TaharChtioui)
- Original soundtrack : Étienne
- Graphical rendering engine [SDL2](https://github.com/libsdl-org/SDL) and submodules : [Sam LANTINGA](https://github.com/slouken) & [Contributors](https://github.com/libsdl-org/SDL/graphs/contributors)
- Sprites : [RPG Maker](https://www.rpgmakerweb.com/)
- Sound effects : [Undertale](https://undertale.com/), [Minecraft](https://www.minecraft.net/)

# Copyright exception

```
Lorsque l'oeuvre a été divulguée, l'auteur ne peut interdire :

3° Sous réserve que soient indiqués clairement le nom de l'auteur et la source :

a) Les analyses et courtes citations justifiées par le caractère critique, polémique, pédagogique, scientifique ou d'information de l'oeuvre à laquelle elles sont incorporées ;
```

Source: [Article L. 122-5](https://www.legifrance.gouv.fr/codes/section_lc/LEGITEXT000006069414/LEGISCTA000006161637/?anchor=LEGIARTI000048603495#LEGIARTI000048603495) of the french copyright law

This project fulfills the quoted exception, because:
- It is a university project, therefore educational
- The protected content we used are small snippets compared to the full works they come from, allowing them to be qualified as "small uses"

This project is by no means used for commercial purposes, it only exists for educational purposes.

It does not do anything that could cause any problem to the sale of the products the protected content comes from in any way, shape or form.