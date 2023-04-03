#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include <stdint.h>

#include "ai.h"
#include "button.h"
#include "tetromino.h"
#include "board.h"

SDL_Texture* get_area_texture(const SDL_Rect rect, SDL_Renderer* renderer, SDL_Texture* source);

uint8_t number_of_digits(uint32_t n);

void print_map(uint8_t** map);
void print_tetromino(Tetromino* tetromino);
void print_all_tetrominos(Board* board);
void print_path(AIAction* actions, uint8_t size_action);
void print_holes(uint8_t* holes);
char* get_string_for_action(AIAction action);
char* get_string_for_block_type(BlockType block_type);

// Working directory for VS : tetris\Tetris

#endif // UTILS_H
