# Tetris in C with SDL2

---
## Game
This is the famous Game created by Alekseï Pajitnov called Tetris. 

In this game you have to move the pieces call "Tetrominoes" to make entire line. When a 
line is full, all the pieces above goes down. The goal is of course to not fill the 
entire board and survive as long as possible.

I created an other game mode in which you play versus an AI. You can choose the difficulty
of the AI but the main things that will change is the speed of the AI.

The AI is capable of picking the best place for the actual piece and move it.

Good luck ! :)

PS : if you wanna know how the AI works it is just [below](#how-the-ai-works).

![Tetris](https://github.com/gyoce/Tetris/blob/main/imgs/Tetris.png)

---
## How to play
You can play with the keys :  
**Q** for going left.  
**S** for going down (soft drop).  
**D** for goind right.  
**R** for rotating the piece.  
**Space** for instant drop of the piece.

---
## How to compile the project with Makefile
1. You must have SDL2 lib installed
2. Download or clone the projet with `git clone https://github.com/gyoce/Tetris-SDL-C.git`.
3. Go to src with `cd tetris/src/`.
4. Make with `make`.
5. Execute with `./main` (Linux) or `main.exe` (Windows).

---
## How to compile the project with Visual studio
1. Open the solution (Tetris.sln)
2. Select x86
3. Launch

---
## How the AI works
So the AI evaluates all the possibilities with its actual piece and give for each possibilities
a score. Then it picks the best score and do the actions for it.

For example if you have a tetromino with a T-Shape, the ai compute all the possibilities left
to right and for every rotation possible, (T-Shape got 4 rotations for example (trust me :) ).

For every rotations and positions of the tetromino the AI is giving a score. The score is calculate
with few parameters :
1. If the tetromino will leave holes
2. The average height of each cells of the tetromino
3. If the tetromino will destroy lines

Then after the AI calculate every score possible for every rotations and positions it picks the 
best one and register some actions to achieve its goals and that's it !.
