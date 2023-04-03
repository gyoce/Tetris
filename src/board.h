#ifndef BOARD_H
#define BOARD_H

#define MAP_WIDTH 10
#define MAP_HEIGHT 20
#define MINI_MAP_WIDTH 6
#define MINI_MAP_HEIGHT 3
#define MAX_TETROMINOS 255
#define TIME_UPDATE 500
#define MAX_LEVEL 29
#define SCORE_PER_SOFT_DROP 4
#define MS_SECOND 1000
#define LOCK_DELAY 500 // ms

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdint.h>

#include "tetromino.h"
#include "text.h"

typedef struct Board {
    void (*full_callback)();         // The method called when the board is full
    void (*lines_destroyed_cb)(int); // The method called when there are lines destroyed
    SDL_Rect pos;				     // The position of the board {x, y, w, h}
    SDL_Rect pos_subboard;		     // The position of the subboard (left or right)
    SDL_Rect pos_stats_board;        // The position of the stats board (left or right)
    Tetromino* actual_tetromino;     // The actual tetromino moving
    Tetromino* next_tetromino;       // The next tetromino
    Tetromino** tetrominos;		     // All tetrominos in the board
    uint8_t nb_tetrominos;		     // The number of tetrominos
    uint8_t** map;				     // The array of array of int, it is the map [line][column]
    uint8_t** map_next;			     // The array of array of int, for the "mini-map" of the next tetromino
    uint8_t** preview;			     // The preview coordinates for the actual tetromino
    uint8_t number_of_lines_blocked; // The number of lines blocked.
    uint8_t* holes;                  // The index of holes in a line blocked.
    float scale[2];			         // The scale x and y factor for the size of the board
    uint8_t square_size[2];		     // The square w and h of all squares
    bool show_right;			     // If the next tetromino is showed on right or not
    bool full;				 	     // If the board is full or not
    bool lock_delay_passed;          // If the lock delay passed or not
    double time_clock;			     // The counter time for the clock
    double time_lock;                // The counter time for the time lock
    double time_drop;                // The counter time for the standard drop
    Text* level_text;			     // The level text associated with the board
    Text* score_text;                // The score text associated with the board
    Text* time_text;                 // The time text associated with the board
    uint32_t score;			         // The score of the board
    uint16_t time;                   // The time of the board (in seconds)
    uint8_t level;                   // The level of the board
    float gravity;                   // The gravity multiplier for standard drop speed
    uint16_t lines_destroyed;        // The number of lines destroyed
    uint8_t lines_before_level_up;   // The number of lines before the level up
} Board;

Board* create_board(SDL_Rect pos, bool show_right, void (*full_callback));
void keyboard_event(Board* board, SDL_Keycode key);
void update_board(Board* board, double delta_t);
void draw_board(Board* board, SDL_Renderer* renderer);
void clean_board(Board** board);
void reset_board(Board* board);

void add_random_lines(Board* board, uint8_t number_of_lines);

bool square_blocked(uint8_t** map, int8_t x, int8_t y, Tetromino* tetromino);
bool tetromino_blocked(uint8_t** map, Tetromino* tetromino);

#endif // BOARD_H
