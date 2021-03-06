﻿# Ninja warrior

## What is this?

Ninja warrior is a platform game made by two students from Design and Development videogames degree. 

## Repository link

https://github.com/polcamacho/Platformer2D

## Team members and responsabilities

### Pol Camacho Banal

Github link: https://github.com/polcamacho

Responsability: Map 2 creation, colliders creation in Tiled, animations, state machine, audio, cleaning code, and readme.

Also enemies spritesheet, golems animations, UMLfor entities, module entities, load and save reimplementation for enemies and player and F11 debug.

And finally, UI structure, button, label, coins, hearts and score implementation, UI UML, WebPage.

### Christian Piña López

Github link: https://github.com/christianpi4

Responsability: Map 1 creation and Map 2 improved, collider implementor, load and save map, god mode, restart, change map, death animation and cleaning code.

Also pathfindig, framerate cap, module entities, checkpoint collider, player jump redo, death state for player and enemies, bat animations, parallax and kill enemy system.

And finally, UI spritesheet, image, slider, menus creation, transitions, timer, audio volume, console.

## How to play

The main goal is simple: arrive to the end of the map! But be carefull, some parts of the map are not easy and there are some angry enemies!

## Innovation

Double jump and run mechanics implemented.

Check point autosave.

Sliders move with arrows and button arrows, and with clicks in scrollbar. One more check point added in map 2, and has a fx when player pass.

## Controls

D: Walk forward.

A: Walk backward.

SPACE: Jump.

D+LSHIFT: Run forward.

A+LSHIFT: Run backward.

D+LSHIFT+SPACE: Jump while running forward.

A+LSHIFT+SPACE: Jump while running backward.

### Double jump!

SPACE+SPACE: Simple double jump.

D+SPACE+SPACE: Double jump while walk forward.

A+SPACE+SPACE: Double jump while walk backward.

D+LSHIFT+SPACE+SPACE: Double jump while running forward.

A+LSHIFT+SPACE+SPACE: Double jump while running backward.

### Debug

F1: Change to map 1.

F2: Change to map 2.

F3: Start from the beginning of the current level.

F5: Save current state (player, enemies and UI).

F6: Load current state (player, enemies and UI).

F8: Debug UI.

F9: View colliders and logic.

F10: God mode.

F11: Framerate cap ON/OFF.

### Clarifications

There are no enemies, because we had many problems to deal with them and UI.

At the beggining of the map, it doesn't appear UI lives and coins on top left corner. Press ESC key to view them.

The music and fx volume are reseted when settings is opened.

Timer sometimes is bugged. Close the game and reopen exe file until it works well.

## Credits

Spritesheed:https://craftpix.net/freebies/assassin-mage-viking-free-pixel-art-game-heroes/

Map assets: https://anokolisa.itch.io/forest-game-assets-20

Map 1 music: https://www.youtube.com/watch?v=iQIz5LmRuKE

Map 2 music: https://www.youtube.com/watch?v=vBatUCjxSrQ&list=PL06diOotXAJLPta6SgpLj9YEgp7VmGr4g

Death FX: https://audiojungle.net/item/8bit-death-sounds/6327314

Jump FX: https://opengameart.org/content/8-bit-platformer-sfx

Using the following SDL libraries:

SDL: https://www.libsdl.org/

SDL_Image: https://www.libsdl.org/projects/SDL_image/

SDL_Mixer: https://www.libsdl.org/projects/SDL_mixer/

## License

MIT License

Copyright (c) 2019 christianpi4 & polcamacho

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
