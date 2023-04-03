#include "utils.h"

#include <stdio.h>
#include <stdbool.h>

/**
 * Get a new texture extract from an parent texture.
 * 
 * \param rect the (x, y, w, h) of the subtexture.
 * \param renderer the renderer of the window.
 * \param source the source texture.
 * \return the new texture at the given position of the source texture.
*/
SDL_Texture* get_area_texture(const SDL_Rect rect, SDL_Renderer* renderer, SDL_Texture* source)
{
    SDL_Texture* result = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    if (result == NULL)
        return NULL;
    SDL_SetRenderTarget(renderer, result);
    SDL_RenderCopy(renderer, source, &rect, NULL);
    SDL_SetRenderTarget(renderer, NULL);
    return result;
}


/**
 * Get the number of digits for a number. 
 * 
 * \param n the number to be examined.
 * \return the number of digits of n.
*/
uint8_t number_of_digits(uint32_t n) 
{
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    return 10;
}


/**
 * Print correctly the map of a board.
 * 
 * \param map the map to be printed.
*/
void print_map(uint8_t** map) 
{
    printf("[");
    for(uint8_t i = 0; i < MAP_HEIGHT; i++)
    {
        printf("[");
        for(uint8_t j = 0; j < MAP_WIDTH; j++)
        {
            if(j == MAP_WIDTH - 1)
                printf("%d]", map[i][j]);
            else 
                printf("%d, ", map[i][j]);
        }
        if(i == MAP_HEIGHT - 1)
            printf("]");
        else 
            printf("],\n");
    }
    printf("]\n\n");
}


/**
 * Print correctly the squares of the tetromino.
 * 
 * \param tetromino the tetromino containing the squares to be printed.
*/
void print_tetromino(Tetromino* tetromino) 
{
    printf("Tetromino : (%s)[", get_string_for_block_type(tetromino->block_type));
    for(uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        if(i == tetromino->nb_squares - 1)
            printf("[%d, %d](%d)", tetromino->squares[i]->x, tetromino->squares[i]->y, tetromino->squares[i]->alive);
        else 
            printf("[%d, %d](%d), ", tetromino->squares[i]->x, tetromino->squares[i]->y, tetromino->squares[i]->alive);
    }
    printf("]\n");
}


/**
 * Print correctly all the tetrominos for a certain board.
 * 
 * \param board the board containing the tetrominos.
*/
void print_all_tetrominos(Board* board) 
{
    for(uint8_t i = 0; i < board->nb_tetrominos; i++) 
    {
        Tetromino* tetromino = board->tetrominos[i];
        if (tetromino == NULL)
            continue;
        print_tetromino(tetromino);
    }
    printf("\n");
}


/**
 * Print correctly the path found by the ai.
 * 
 * \param actions the array of actions.
 * \param size_action the size of the array of actions.
*/
void print_path(AIAction* actions, uint8_t size_action) 
{
    printf("Actions : [");
    for (uint8_t i = 0; i < size_action; i++) 
    {
        if (i == size_action - 1)
            printf("%s]\n", get_string_for_action(actions[i]));
        else
            printf("%s, ", get_string_for_action(actions[i]));
    }
}


/**
 * Print correctly the holes of a board.
 * 
 * \param holes the holes to be printed.
*/
void print_holes(uint8_t* holes) 
{
    printf("Holes : [");
    for (uint8_t i = 0; i < MAP_HEIGHT; i++)
    {
        if (i == MAP_HEIGHT - 1)
            printf("%d]\n", holes[i]);
        else
            printf("%d, ", holes[i]);
    }
}


/**
 * Return the corresponding string for this action.
 * 
 * \param action the action.
 * \return string associated with the action.
*/
char* get_string_for_action(AIAction action) 
{
    switch (action) 
    {
        case ROTATE:
            return "ROTATE";
        case MOVE_LEFT:
            return "MOVE_LEFT";
        case MOVE_RIGHT:
            return "MOVE_RIGHT";
        case HARD_DROP:
            return "HARD_DROP";
        default:
            return "UNKNOWN";
    }
}


/**
 * Return the string for a block type.
 * 
 * \param block_type of the tetromino.
 * \return the string representation.
*/
char* get_string_for_block_type(BlockType block_type) 
{
    switch(block_type)
    {
        case I:
            return "I";
        case J:
            return "J";
        case L:
            return "L";
        case O:
            return "O";
        case S:
            return "S";
        case T:
            return "T";
        case Z:
            return "Z";
        case B_NONE:
        case B_MAX:
        default:
            return "UKKNOWN";
    }
}
