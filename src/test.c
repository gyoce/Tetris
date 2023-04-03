#include "test.h"

#include <stdint.h>

#include "tetromino.h"
#include "print_color.h"

void place_tetromino(Tetromino* tetromino, uint8_t x, uint8_t y, uint8_t rotation);

/**
 * Fill the board for a test.
 * Test:
 * Destroy lines * 8
*/
void test_map1(Board* board) 
{
	uint8_t i = 0;

	// Right
	Tetromino* tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 19, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 18, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 17, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 16, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 15, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 14, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 13, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 12, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 11, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 10, 0);
	board->tetrominos[i] = tetromino;
	i++;

	// Left
	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 0, 19, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 0, 18, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 0, 17, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 0, 16, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 0, 15, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 0, 14, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 0, 13, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 0, 12, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 0, 11, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 0, 10, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 4, 16, 1);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 4, 12, 1);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 4, 8, 1);
	board->tetrominos[i] = tetromino;
	i++;

	/*tetromino = create_tetromino(T);
	place_tetromino(tetromino, 1, 14, 0);
	board->tetrominos[i] = tetromino;
	i++;*/

	board->nb_tetrominos = i;
	board->next_tetromino = create_tetromino(I);
}


/**
 * Fill the board for a test.
 * Test:
 * Destroy lines * 1
*/
void test_map2(Board* board) 
{
	uint8_t i = 0;

	// Left
	Tetromino* tetromino = create_tetromino(I);
	place_tetromino(tetromino, 3, 16, 1);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(J);
	place_tetromino(tetromino, 2, 19, 2);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(O);
	place_tetromino(tetromino, 1, 17, 0);
	board->tetrominos[i] = tetromino;
	i++;

	// Right
	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 9, 16, 1);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(O);
	place_tetromino(tetromino, 7, 18, 0);
	board->tetrominos[i] = tetromino;
	i++;

	board->nb_tetrominos = i;

	clean_tetromino(&board->next_tetromino);
	board->next_tetromino = create_tetromino(J);
	update_board(board, 500);
}


/**
 * 
*/
void test_map3(Board* board) 
{
	uint8_t i = 0;

	// Left
	Tetromino* tetromino = create_tetromino(I);
	place_tetromino(tetromino, 0, 19, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 4, 16, 1);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(Z);
	place_tetromino(tetromino, 1, 17, 1);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(J);
	place_tetromino(tetromino, 0, 18, 3);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 3, 14, 1);
	board->tetrominos[i] = tetromino;
	i++;

	// Right
	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 19, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 18, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 17, 0);
	board->tetrominos[i] = tetromino;
	i++;

	tetromino = create_tetromino(I);
	place_tetromino(tetromino, 6, 16, 0);
	board->tetrominos[i] = tetromino;
	i++;

	board->nb_tetrominos = i;

	clean_tetromino(&board->next_tetromino);
	board->next_tetromino = create_tetromino(I);
}


void test_map4(Board* board) 
{
	uint8_t i = 0;

	// Left
	Tetromino* tetromino = create_tetromino(I);
	place_tetromino(tetromino, 0, 19, 0);
	board->tetrominos[i] = tetromino;
	i++;

	board->nb_tetrominos = i;

	clean_tetromino(&board->next_tetromino);
	board->next_tetromino = create_tetromino(S);
}

/**
 * Place a tetromino at certain coordinates
*/
void place_tetromino(Tetromino* tetromino, uint8_t x, uint8_t y, uint8_t rotation) 
{
	switch(tetromino->block_type) 
	{
		case I:
			if(rotation == 0)
			{
				tetromino->squares[0]->x = x;
				tetromino->squares[0]->y = y;
				tetromino->squares[1]->x = x + 1;
				tetromino->squares[1]->y = y;
				tetromino->squares[2]->x = x + 2;
				tetromino->squares[2]->y = y;
				tetromino->squares[3]->x = x + 3;
				tetromino->squares[3]->y = y;
			}
			else 
			{
				tetromino->squares[0]->x = x;
				tetromino->squares[0]->y = y;
				tetromino->squares[1]->x = x;
				tetromino->squares[1]->y = y + 1;
				tetromino->squares[2]->x = x;
				tetromino->squares[2]->y = y + 2;
				tetromino->squares[3]->x = x;
				tetromino->squares[3]->y = y + 3;
			}
			break;
		case T:
			tetromino->squares[0]->x = x;
			tetromino->squares[0]->y = y;
			tetromino->squares[1]->x = x + 1;
			tetromino->squares[1]->y = y;
			tetromino->squares[2]->x = x + 2;
			tetromino->squares[2]->y = y;
			tetromino->squares[3]->x = x + 1;
			tetromino->squares[3]->y = y + 1;
			break;
		case J:
			if (rotation == 2) 
			{
				tetromino->squares[0]->x = x;
				tetromino->squares[0]->y = y;
				tetromino->squares[1]->x = x - 1;
				tetromino->squares[1]->y = y;
				tetromino->squares[2]->x = x - 2;
				tetromino->squares[2]->y = y;
				tetromino->squares[3]->x = x - 2;
				tetromino->squares[3]->y = y - 1;
			}
			else if (rotation == 3) 
			{
				tetromino->squares[0]->x = x;
				tetromino->squares[0]->y = y;
				tetromino->squares[1]->x = x;
				tetromino->squares[1]->y = y - 1;
				tetromino->squares[2]->x = x;
				tetromino->squares[2]->y = y - 2;
				tetromino->squares[3]->x = x + 1;
				tetromino->squares[3]->y = y - 2;
			}
			break;
		case O:
			tetromino->squares[0]->x = x;
			tetromino->squares[0]->y = y;
			tetromino->squares[1]->x = x + 1;
			tetromino->squares[1]->y = y;
			tetromino->squares[2]->x = x;
			tetromino->squares[2]->y = y + 1;
			tetromino->squares[3]->x = x + 1;
			tetromino->squares[3]->y = y + 1;
			break;
		case Z:
			tetromino->squares[0]->x = x;
			tetromino->squares[0]->y = y;
			tetromino->squares[1]->x = x + 1;
			tetromino->squares[1]->y = y;
			tetromino->squares[2]->x = x + 1;
			tetromino->squares[2]->y = y + 1;
			tetromino->squares[3]->x = x + 2;
			tetromino->squares[3]->y = y + 1;
			break;
		case S:
		case B_NONE:
		case B_MAX:
		default:
			print_color("[Test](place_tetromino) block type not supported", TXT_RED);
			break;
	}
}
