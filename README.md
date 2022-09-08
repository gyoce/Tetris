# Tetris in C with SDL2

![Tetris](https://github.com/gyoce/Tetris-SDL-C/blob/master/doc/Tetris.png)

---
## How to play
You can play with the keys :  
**Q** for going left.  
**S** for going down (soft drop).  
**D** for goind right.  
**R** for rotating the piece.  
**Space** for instant drop of the piece.

---
## How to compile the project
1. Download or clone the projet with `git clone https://github.com/gyoce/Tetris-SDL-C.git`.
1. Go to src with `cd tetris/src/`.
1. Make with `make`.
1. Execute with `./main` (Linux) or `main.exe` (Windows).

---
## Code explanation
### ai_scene
The AI scene allow you to play with an AI. You can choose a difficulty. This difficulty
change the speed of every movements of the ai.

### ai
It contains all the logic of the ai. The ai simulates every movements for the 
actual tetromino and give a score foreach possibility. The best score is taken 
and it will simulate the movements.  
The best score is calculate with the number of holes that the tetromino will let,
the average height of each square and if it will destroy lines.

### board
It contains all the logic for the tetris : generate tetromino, update tetromino,
update score and level ...  
When you press a key this will be process by this board and try the movements for
the tetromino.

### button
It contains all the needs for a button in the game. A button is an area clickable
and hoverable.  
When it's clicked, it call a callback method which is defined in the creation 
of the button.

### global
It contains all the textures needed in the game and some global variables like 
pathes to files (sprite, font ...).  
For exemple it contains the array of textures representing the squares of the 
tetrominoes.

## main
The main file of the project. It create a tetris "object" and run the main function
of it.

### menu_scene
The menu scene is the first scene of the game. You will have to choose between
the **solo** and the **AI** scene with some buttons.  

### print_color
It contains just a function to print in the console with colors (Only use in 
debug mode).

### solo_scene
The solo scene is the singleplayer game of Tetris. The objectiv of this game is
to have the best score possible.

### test
It contains some functions to fill the map of the board to test (Only use in 
debug mode).

### tetris
The tetris will initialize **window**, **font**, **sprites** and **scenes**.
It is also the main loop of the game.  
It contains the logic to pass from a scene
to another.

### tetromino
It contains the logic to spawn a new tetromino and to correctly initialize
one.  
There are 7 types of tetrominoes possible (I, O, T, L, J, Z and S).

### text
It contains all the needs for drawing text in the window.  
It also contains the needs to draw a "KeyText". It is a texture showing the key
to press and a Text associate with it.

### utils
It contains some random utilities functions. Some are for debug only, others have
real utility.

