#ifndef TETROMINO_H
#define TETROMINO_H

#include <SDL2/SDL.h>

#include <stdint.h>
#include <stdbool.h>

#define NB_SQUARES 4

typedef enum Color {
	C_NONE,
	LIGHT_BLUE,
	YELLOW,
	PURPLE,
	ORANGE,
	DARK_BLUE,
	RED,
	GREEN,
	GRAY,
	C_MAX
} Color;

typedef enum BlockType {
	B_NONE,
	I,
	J,
	L,
	O,
	S,
	T,
	Z,
	B_MAX
} BlockType;

typedef struct Square {
	int8_t x, y;
	bool alive;
} Square;

typedef struct Tetromino {
	Square** squares;
	uint8_t nb_squares;
	Color color;
	BlockType block_type;
	bool blocked;
	uint8_t rotation;
} Tetromino;

Tetromino* create_tetromino(BlockType block_type);
void clean_tetromino(Tetromino** tetromino);

Tetromino* get_random_tetromino();
bool square_in_tetromino(const Tetromino* tetromino, const int8_t x, const int8_t y);
bool tetromino_is_dead(const Tetromino* tetromino);
bool same_tetrominos(const Tetromino* tetromino1, const Tetromino* tetromino2);
Color get_color_for_block_type(BlockType block_type);
uint8_t get_number_of_rotation(BlockType block_type);
int8_t** get_next_rotation_moves(Tetromino* tetromino);
void update_rotation(Tetromino* tetromino);

#endif // TETROMINO_H
