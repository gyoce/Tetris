#include "ai.h"

#include "tetromino.h"
#include "utils.h"

#include <stdio.h>
#include <math.h>
#include <limits.h>


/* AI Struct declaration */
typedef struct SquarePos {
    uint8_t x;
    uint8_t y;
} SquarePos;

typedef struct MinMax {
    uint8_t min_x;
    uint8_t max_x;
    int8_t min_y;
    int8_t max_y;
} MinMax;


/* AI functions declaration */
void calculate_best_path(AI* ai);
void generate_path(AI* ai, uint8_t rotation, int8_t move_right_left);
void set_position(Tetromino* tetromino, SquarePos** squares_pos);
void set_left_position(Tetromino* tetromino, uint8_t** map);
void set_max_position(Tetromino* tetromino, uint8_t** map);
void rotate(Tetromino* tetromino);
bool tetromino_left_blocked(Tetromino* tetromino, uint8_t** map);
void process_action(Board* board, AIAction action);

int8_t calculate_path(Tetromino* tetromino, uint8_t** map, MinMax* min_max);
uint8_t count_holes(uint8_t** map, MinMax* min_max, Tetromino* tetromino);
uint8_t count_lines_destroyed(uint8_t** map);
uint8_t get_avg_height(Tetromino* tetromino);

int8_t get_moves_right_left(uint8_t rotation, Tetromino* tetromino, SquarePos** target_position);
bool squares_pos_same_x(SquarePos** pos1, SquarePos** pos2, Tetromino* tetromino);
void add_position(SquarePos** squares_pos, int8_t x, int8_t y, Tetromino* tetromino);
uint8_t get_number_of_path(Tetromino* tetromino, uint8_t** map);
MinMax* get_min_max(Tetromino* tetromino);
void copy_tetromino_in_map(uint8_t** map, Tetromino* tetromino);
uint8_t** get_copy_of_map(Board* board);
Tetromino* get_copy_of_tetromino(Board* board);
SquarePos** get_copy_of_squares_pos(Tetromino* tetromino);

void clean_tetromino_from_map(uint8_t** map, Tetromino* tetromino);
void clean_map(uint8_t** map);
void clean_squares_pos(SquarePos** squares_pos, Tetromino* tetromino);

uint16_t get_time_to_update(AIDifficulty difficulty);


/* #################### PUBLIC METHODS #################### */

/**
 * Create an ai for this board.
 * 
 * \param board the board concerned to be played by an AI.
 * \return an AI object.
*/
AI* create_ai(Board* board)
{
	AI* ai = malloc(sizeof(AI));
	if (ai == NULL)
		return NULL;
	ai->ai_board = board;
	ai->actual_index = 0;
    ai->size_actions = 0;
    ai->actions = NULL;
    ai->path_found = false;
    ai->time_update = 0;
    ai->time_to_update = 0;
	return ai;
}


/**
 * Correctly free an ai object.
 * 
 * \param ai the pointer of pointer of the ai.
*/
void clean_ai(AI** ai) 
{
    AI* _ai = (*ai);
    if (_ai->actions != NULL)
    {
        free(_ai->actions);
    }
	free(_ai);
	*ai = NULL;
}


/**
 * Reset the ai to be to the initial state.
 * 
 * \param ai to be reseted.
*/
void reset_ai(AI* ai)
{
    if (ai->actions != NULL)
        free(ai->actions);
    ai->actions = NULL;
    ai->size_actions = 0;
    ai->actual_index = 0;
    ai->path_found = false;
    ai->time_update = 0;
}


/**
 * Set the difficulty of ai.
 * 
 * \param ai the ai concerned.
 * \param difficulty the difficulty of the ai.
*/
void set_ai_difficulty(AI* ai, AIDifficulty difficulty) 
{
    ai->ai_difficulty = difficulty;
    ai->time_to_update = get_time_to_update(difficulty);
}


/**
 * Process the AI to calculate the best movement to do and do it.
 * 
 * \param ai the ai to be processed.
 * \param delta_t the time passed.
*/
void ai_process(AI* ai, double delta_t) 
{
    ai->time_update += delta_t;

    if (!ai->path_found || ai->actual_index == ai->size_actions) 
        calculate_best_path(ai);

    if (ai->time_update > ai->time_to_update)
    {
        process_action(ai->ai_board, ai->actions[ai->actual_index]);
        ai->actual_index++;
        ai->time_update = 0;
    }
}


/* #################### AI METHODS #################### */

/**
 * Calculate the best path for the actual tetromino of the board.
 * 
 * \param ai the AI concerned.
*/
void calculate_best_path(AI* ai) 
{
    ai->actual_index = 0;
    uint8_t** map_copy = get_copy_of_map(ai->ai_board);
    Tetromino* tetromino_copy = get_copy_of_tetromino(ai->ai_board);
    SquarePos** base_squares_pos = get_copy_of_squares_pos(tetromino_copy);
    SquarePos** best_pos = NULL;

    int8_t score = CHAR_MIN;
    uint8_t number_of_rotations = get_number_of_rotation(tetromino_copy->block_type);
    uint8_t best_rotation = 0;

    // Foreach rotation of the tetromino
    for (uint8_t rotation = 0; rotation < number_of_rotations; rotation++)
    {
        set_left_position(tetromino_copy, map_copy);
        uint8_t number_of_path = get_number_of_path(tetromino_copy, map_copy);
        SquarePos** start_position = get_copy_of_squares_pos(tetromino_copy);

        // Foreach path for that rotation
        for (uint8_t path = 0; path < number_of_path; path++) 
        {
            set_max_position(tetromino_copy, map_copy);

            MinMax* min_max = get_min_max(tetromino_copy);
            if (min_max->min_y < 0)
            {
                free(min_max);
                continue;
            }
            copy_tetromino_in_map(map_copy, tetromino_copy);
            int8_t score_path = calculate_path(tetromino_copy, map_copy, min_max);
            if (score_path > score)
            {
                score = score_path;
                best_rotation = rotation;
                if (best_pos != NULL)
                    clean_squares_pos(best_pos, tetromino_copy);
                best_pos = get_copy_of_squares_pos(tetromino_copy);
            }
            clean_tetromino_from_map(map_copy, tetromino_copy);

            add_position(start_position, 1, 0, tetromino_copy);
            set_position(tetromino_copy, start_position);
            free(min_max);
        }
        rotate(tetromino_copy);
        clean_squares_pos(start_position, tetromino_copy);
    }

    set_position(tetromino_copy, base_squares_pos);
    tetromino_copy->rotation = 0;
    
    if (best_pos != NULL)
    {
        int8_t moves_right_left = get_moves_right_left(best_rotation, tetromino_copy, best_pos);
        generate_path(ai, best_rotation, moves_right_left);
    }

    clean_squares_pos(base_squares_pos, tetromino_copy);
    clean_map(map_copy);
    clean_tetromino(&tetromino_copy);
    ai->path_found = true;
}


/**
 * Generate the path for the ai which will be store in actions of ai.
 * 
 * \param ai the ai concerned.
 * \param rotation the index of the rotation of the tetromino.
 * \param move_right_left the amount of MOVE_RIGHT or MOVE_LEFT.
*/
void generate_path(AI* ai, uint8_t rotation, int8_t move_right_left) 
{
    if (ai->actions != NULL)
        free(ai->actions);

    uint8_t number_of_actions = rotation + abs(move_right_left) + 1;
    ai->actions = malloc(sizeof(AIAction) * (number_of_actions));
    ai->size_actions = number_of_actions;

    uint8_t index = 0;
    for (uint8_t i = 0; i < rotation; i++) 
    {
        ai->actions[index] = ROTATE;
        index++;
    }

    for (uint8_t i = 0; i < abs(move_right_left); i++) 
    {
        ai->actions[index] = move_right_left < 0 ? MOVE_LEFT : MOVE_RIGHT;
        index++;
    }

    ai->actions[ai->size_actions - 1] = HARD_DROP;
}


/**
 * Set the tetromino position to the first possible position in the map on the left.
 * 
 * \param tetromino the tetromino which will be set to the first position.
 * \param map the map of the board.
*/
void set_left_position(Tetromino* tetromino, uint8_t** map)
{
    bool blocked = tetromino_left_blocked(tetromino, map);
    while (!blocked) 
    {
        for (uint8_t i = 0; i < tetromino->nb_squares; i++)
            tetromino->squares[i]->x--;
        blocked = tetromino_left_blocked(tetromino, map);
    }
}


/**
 * Set the max position in depth for that tetromino.
 * 
 * \param tetromino the tetromino to be set to the max position.
 * \param map the map associated with the board.
*/
void set_max_position(Tetromino* tetromino, uint8_t** map) 
{
    bool blocked = tetromino_blocked(map, tetromino);
    while (!blocked)
    {
        for (uint8_t i = 0; i < tetromino->nb_squares; i++)
            tetromino->squares[i]->y++;
        blocked = tetromino_blocked(map, tetromino);
    }
}


/**
 * Set the tetromino position to the squares pos.
 * 
 * \param tetromino the tetromino to be changed.
 * \param squares_pos all the squares pos.
*/
void set_position(Tetromino* tetromino, SquarePos** squares_pos) 
{
    for (uint8_t i = 0; i < tetromino->nb_squares; i++) 
    {
        SquarePos* square_pos = squares_pos[i];
        Square* square = tetromino->squares[i];
        square->x = square_pos->x;
        square->y = square_pos->y;
    }
}


/**
 * Check if a tetromino is left blocked.
 * 
 * \param tetromino the tetromino to be checked.
 * \param map the map of the board.
 * \return true if the tetromino is left blocked, false else.
*/
bool tetromino_left_blocked(Tetromino* tetromino, uint8_t** map) 
{
    for (uint8_t i = 0; i < tetromino->nb_squares; i++) 
    {
        Square* square = tetromino->squares[i];

        if (square->x == 0)
            return true;

        if (square->y >= 0 && map[square->y][square->x - 1])
        {
            if (square_in_tetromino(tetromino, square->x - 1, square->y))
                continue;
            else
                return true;
        }
    }
    return false;
}


/**
 * Check if a tetromino is right blocked.
 *
 * \param tetromino the tetromino to be checked.
 * \param map the map of the board.
 * \return true if the tetromino is right blocked, false else.
*/
bool tetromino_right_blocked(Tetromino* tetromino, uint8_t** map) 
{
    for (uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        Square* square = tetromino->squares[i];

        if (square->x == MAP_WIDTH - 1)
            return true;

        if (square->y >= 0 && map[square->y][square->x + 1])
        {
            if (square_in_tetromino(tetromino, square->x + 1, square->y))
                continue;
            else
                return true;
        }
    }
    return false;
}


/**
 * Rotate the tetromino.
 * Assume that we can rotate it.
 * 
 * \param tetromino the tetromino to be rotated.
*/
void rotate(Tetromino* tetromino) 
{
    int8_t** moves = get_next_rotation_moves(tetromino);
    for (uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        tetromino->squares[i]->x += moves[i][0];
        tetromino->squares[i]->y += moves[i][1];
    }
    update_rotation(tetromino);

    for (uint8_t i = 0; i < tetromino->nb_squares; i++)
        free(moves[i]);
    free(moves);
}


/**
 * Calculate the score of the actual path.
 * 
 * \param tetromino the tetromino which is placed in the map.
 * \param map the map associated with the board.
 * \param min_max the min and max for the tetromino.
 * \return the score for that path.
*/
int8_t calculate_path(Tetromino* tetromino, uint8_t** map, MinMax* min_max) 
{
    int8_t score = 0;
    uint8_t number_of_holes = count_holes(map, min_max, tetromino);
    uint8_t number_of_lines = count_lines_destroyed(map);
    uint8_t avg_max_height = get_avg_height(tetromino);
    score += avg_max_height;        // High height = more score
    score += number_of_lines * 2;   // Lines destroyed = more score
    score -= (number_of_holes * 3); // Less holes = more score
    return score;
}


/**
 * Count the number of holes below the tetromino.
 * WARNING : The tetromino has to be in the map.
 *
 * \param map the associated map of the board.
 * \param min_max the min max object of the tetromino.
 * \param tetromino the tetromino associated.
 * \return the number of holes below the tetromino.
*/
uint8_t count_holes(uint8_t** map, MinMax* min_max, Tetromino* tetromino)
{
    uint8_t count = 0;

    for (uint8_t x = min_max->min_x; x <= min_max->max_x; x++) 
    {
        uint8_t y = min_max->min_y;

        // Go to the nearest square in tetromino
        if (map[y][x] == 0)
        {
            while (!square_in_tetromino(tetromino, x, y))
                y++;
        }
        
        // Go below the square
        while (y < MAP_HEIGHT && square_in_tetromino(tetromino, x, y))
            y++;
        
        // Count the hole while there is another square
        while (y < MAP_HEIGHT && !map[y][x])
        {
            y++;
            count++;
        }
    }

    return count;
}


/**
 * Count the number of lines which will be destroyed.
 * 
 * \param map the map associated with the board.
 * \return the number of lines which will be destroyed.
*/
uint8_t count_lines_destroyed(uint8_t** map)
{
    uint8_t count = 0;
    for (uint8_t y = 0; y < MAP_HEIGHT; y++) 
    {
        uint8_t temp = 0;
        for (uint8_t x = 0; x < MAP_WIDTH; x++)
        {
            if (map[y][x])
                temp++;
        }
        if (temp == MAP_WIDTH)
            count++;
    }
    return count;
}


/**
 * Get the average height of each square in the tetromino.
 * 
 * \param tetromino the tetromino to be examined.
 * \return the average height.
*/
uint8_t get_avg_height(Tetromino* tetromino) 
{
    uint8_t total = 0;
    for (uint8_t i = 0; i < tetromino->nb_squares; i++)
        total += tetromino->squares[i]->y;
    return round((float)total / (float)tetromino->nb_squares);
}


/**
 * Process the action for the board.
 * Simulate a keyboard event.
 * 
 * \param board the board receiving an keyboard event.
 * \param action the action to process to the board.
*/
void process_action(Board* board, AIAction action) 
{
    SDL_Keycode key;
    switch (action) 
    {
        case ROTATE:
            key = SDLK_r;
            break;
        case HARD_DROP:
            key = SDLK_SPACE;
            break;
        case MOVE_LEFT:
            key = SDLK_q;
            break;
        case MOVE_RIGHT:
            key = SDLK_d;
            break;
        case NONE:
        default:
            break;
    }
    keyboard_event(board, key);
}


/**
 * Return the movement to do left or right for the tetromino.
 * WARNING : the tetromino has to be in the initial state and position.
 * 
 * \param rotation the rotation of the tetromino.
 * \param tetromino the tetromino concerned.
 * \param position the position of the tetromino.
 * \return the amount to add to x to get to the target position (can be negative).
*/
int8_t get_moves_right_left(uint8_t rotation, Tetromino* tetromino, SquarePos** target_position) 
{
    int8_t moves = 0;
    for (uint8_t i = 0; i < rotation; i++)
        rotate(tetromino);
    
    SquarePos** squares_pos = get_copy_of_squares_pos(tetromino);

    int8_t x_amount = squares_pos[0]->x < target_position[0]->x ? 1 : -1;
    while (!squares_pos_same_x(squares_pos, target_position, tetromino))
    {    
        add_position(squares_pos, x_amount, 0, tetromino);
        moves += x_amount;
    }
    clean_squares_pos(squares_pos, tetromino);
    return moves;
}


/**
 * Add an x and y amount to all the squares positions.
 * 
 * \param squares_pos the array of square pos.
 * \param x the amount to be added to x of all squares pos.
 * \param y the amount to be added to y of all squares pos.
 * \param tetromino the tetromino associated with the square pos.
*/
void add_position(SquarePos** squares_pos, int8_t x, int8_t y, Tetromino* tetromino) 
{
    for (uint8_t i = 0; i < tetromino->nb_squares; i++) 
    {
        SquarePos* square_pos = squares_pos[i];
        square_pos->x += x;
        square_pos->y += y;
    }
}


/**
 * Get the number of path possible.
 * WARNING : tetromino has to be on the max left position.
 * 
 * \param tetromino the tetromino to count the path possible.
 * \param map the map associated with the board.
 * \return the number of path possible.
*/
uint8_t get_number_of_path(Tetromino* tetromino, uint8_t** map) 
{
    SquarePos** base_pos = get_copy_of_squares_pos(tetromino);
    uint8_t count = 1;
    bool blocked = tetromino_right_blocked(tetromino, map);
    while (!blocked)
    {
        for (uint8_t i = 0; i < tetromino->nb_squares; i++)
            tetromino->squares[i]->x++;
        count++;
        blocked = tetromino_right_blocked(tetromino, map);
    }
    set_position(tetromino, base_pos);
    clean_squares_pos(base_pos, tetromino);
    return count;
}


/**
 * Copy the tetromino into the map. i.e. copy all the squares
 * from the tetromino inside the map with the corresponding color.
 * 
 * \param map the map which will contain the tetromino.
 * \param tetromino the tetromino to copied inside the map.
*/
void copy_tetromino_in_map(uint8_t** map, Tetromino* tetromino) 
{
    for(uint8_t i = 0; i < tetromino->nb_squares; i++) 
    {
        Square* square = tetromino->squares[i];
        if (square->y > 0)
            map[square->y][square->x] = tetromino->color;
    }
}


/**
 * Clean the tetromino from the map. i.e. remove all the squares
 * from the tetromino from the map.
 *
 * \param map the map to process.
 * \param tetromino the tetromino to remove from the map.
*/
void clean_tetromino_from_map(uint8_t** map, Tetromino* tetromino)
{
    for (uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        Square* square = tetromino->squares[i];
        if (square->y > 0)
            map[square->y][square->x] = 0;
    }
}


/**
 * Check if all the squares from pos1 have the same x with pos2.
 * 
 * \param pos1 the first position to be compared.
 * \param pos2 the second position to be compared.
 * \param tetromino the tetromino associated with the positions.
 * \return true if the squares have the same x, false else.
*/
bool squares_pos_same_x(SquarePos** pos1, SquarePos** pos2, Tetromino* tetromino)
{
    for (uint8_t i = 0; i < tetromino->nb_squares; i++) 
        if (pos1[i]->x != pos2[i]->x)
            return false;
    return true;
}


/**
 * Get the max min object from the tetromino
 * 
 * \param tetromino the tetromino containing all its squares.
 * \return the min max object.
*/
MinMax* get_min_max(Tetromino* tetromino)
{
    MinMax* min_max = malloc(sizeof(MinMax));
    uint8_t max_x = 0;
    int8_t max_y = CHAR_MIN;
    uint8_t min_x = UCHAR_MAX;
    int8_t min_y = CHAR_MAX;
    for (uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        Square* square = tetromino->squares[i];

        if (square->x > max_x)
            max_x = square->x;
        if (square->x < min_x)
            min_x = square->x;

        if (square->y > max_y)
            max_y = square->y;
        if (square->y < min_y)
            min_y = square->y;
    }
    min_max->min_x = min_x;
    min_max->min_y = min_y;
    min_max->max_y = max_y;
    min_max->max_x = max_x;
    return min_max;
}


/* #################### COPY METHODS #################### */

/**
 * Copy the board map to a new one, without the actual tetromino.
 * 
 * \param board the board containing the map.
 * \return the map of the board copied.
*/
uint8_t** get_copy_of_map(Board* board) 
{
    uint8_t** map = malloc(sizeof(uint8_t*) * MAP_HEIGHT);
    for (uint8_t i = 0; i < MAP_HEIGHT; i++) 
        map[i] = malloc(sizeof(uint8_t) * MAP_WIDTH);

    for (uint8_t i = 0; i < MAP_HEIGHT; i++)
        for (uint8_t j = 0; j < MAP_WIDTH; j++)
            map[i][j] = board->map[i][j];

    Tetromino* tetromino = board->actual_tetromino;
    for (uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        Square* square = tetromino->squares[i];
        if (square->y < 0)
            continue;
        map[square->y][square->x] = 0;
    }

    return map;
}


/**
 * Copy the actual tetromino of the board.
 * 
 * \param board the board containing the actual tetromino.
 * \return the tetromino copied from the board.
*/
Tetromino* get_copy_of_tetromino(Board* board) 
{
    Tetromino* tetromino = create_tetromino(board->actual_tetromino->block_type);
    return tetromino;
}


/**
 * Copy the position of the tetromino into a array of SquarePos.
 * 
 * \param tetromino the tetromino to copy its position.
 * \return an array of SquarePos object containing the positions.
*/
SquarePos** get_copy_of_squares_pos(Tetromino* tetromino)
{
    SquarePos** squares_pos = malloc(sizeof(SquarePos*) * (tetromino->nb_squares));
    for (uint8_t i = 0; i < tetromino->nb_squares; i++) 
    {
        squares_pos[i] = malloc(sizeof(SquarePos));
        squares_pos[i]->x = tetromino->squares[i]->x;
        squares_pos[i]->y = tetromino->squares[i]->y;
    }
    return squares_pos;
}


/* #################### CLEAN METHODS #################### */

/**
 * Correctly clean a map of a board.
 * 
 * \param map the map to be cleaned.
*/
void clean_map(uint8_t** map) 
{
    for (uint8_t i = 0; i < MAP_HEIGHT; i++)
        free(map[i]);
    free(map);
}


/**
 * Correctly clean a SquarePos array.
 * 
 * \param squares_pos the array to be cleaned.
*/
void clean_squares_pos(SquarePos** squares_pos, Tetromino* tetromino)
{
    for (uint8_t i = 0; i < tetromino->nb_squares; i++)
        free(squares_pos[i]);
    free(squares_pos);
}


/* #################### UTILS METHODS #################### */

/**
 * Get the time to update for a difficulty.
 * 
 * \param difficulty the difficulty of the ai.
 * \return the number of ms before updating.
*/
uint16_t get_time_to_update(AIDifficulty difficulty) 
{
    uint16_t time_to_update = 0;
    switch (difficulty) 
    {
        case EASY:
            time_to_update = TIME_EASY;
            break;
        case MEDIUM:
            time_to_update = TIME_MEDIUM;
            break;
        case HARD:
            time_to_update = TIME_HARD;
            break;
        case EXTREME:
            time_to_update = TIME_EXTREME;
            break;
    }
    return time_to_update;
}
