#include "board.h"

#include "print_color.h"
#include "tetromino.h"
#include "utils.h"
#include "global.h"

#include <stdio.h>
#include <stdlib.h>


/* Local functions declaration */
void init_memory(Board* board);
void init_subboards(Board* board);
void init_texts(Board* board);

bool move_possible_x(Board* board, int8_t x);
bool move_possible_y(Board* board, int8_t y);
void move_tetromino(Board* board, int8_t x, int8_t y);

void summon_tetromino(Board* board);
void update_tetromino(Board* board);
void update_positions(Board* board);
void update_next_tetromino(Board* board);
void update_preview(Board* board);
void update_score(Board* board, uint16_t score_add);
void update_level(Board* board, uint8_t lines_destroyed);
void update_gravity(Board* board);
void update_text(Text* text, uint32_t add_value);
void destroy_useless_tetrominos(Board* board);
void sort_board_tetrominos(Board* board, uint8_t start);
void check_for_lines(Board* board);
void destroy_lines(Board* board, uint8_t* lines, uint8_t nb_of_lines);
Square* search_square_by_coord(Board* board, uint8_t x, uint8_t y);
void reset_map(Board* board);
void reset_mini_map(Board* board);
void add_tetromino_to_board(Board* board, Tetromino* tetromino);
void rotate_tetromino(Board* board);
bool rotation_possible(Board* board, int8_t** moves);
bool board_is_full(Board* board);
void hard_drop_tetromino(Board* board);

void draw_next_tetromino(Board* board, SDL_Renderer* renderer);
void draw_map(Board* board, SDL_Renderer* renderer);
void draw_preview(Board* board, SDL_Renderer* renderer);

uint8_t calculate_lines_before_level_up(Board* board);
uint16_t calculate_score(Board* board, uint8_t nb_of_lines);
float calculate_gravity_by_level(uint8_t level);

/* #################### PUBLIC #################### */

/**
 * Create a board for the game.
 * 
 * \param pos the position of the board.
 * \param show_right if the stats board and next tetromino is showed on the right or left.
 * \return a new board object.
*/
Board* create_board(SDL_Rect pos, bool show_right, void (*full_callback)) 
{
    Board* board = malloc(sizeof(Board));
    if (board == NULL)
        return NULL;
    board->full_callback = full_callback;
    board->pos = pos;
    board->nb_tetrominos = 0;
    board->level = 0;
    board->score = 0;
    board->time = 0;
    board->time_clock = 0;
    board->time_lock = 0;
    board->time_drop = 0;
    board->number_of_lines_blocked = 0;
    board->full = false;
    board->show_right = show_right;
    board->lock_delay_passed = true;
    board->lines_before_level_up = calculate_lines_before_level_up(board);
    board->actual_tetromino = NULL;
    board->gravity = calculate_gravity_by_level(board->level);
    board->lines_destroyed_cb = NULL;

    // Square size
    board->square_size[0] = pos.w / (MAP_WIDTH + 2);  // width
    board->square_size[1] = pos.h / (MAP_HEIGHT + 2); // height

    // Scale
    board->scale[0] = (float) pos.w / GAME_BOARD.w; // scale x
    board->scale[1] = (float) pos.h / GAME_BOARD.h; // scale y

    init_memory(board);
    init_subboards(board);
    init_texts(board);

    update_next_tetromino(board);

    return board;
}


/**
 * Init all the variables in the board which need memory allocation.
 *
 * \param board the board containing the variables.
*/
void init_memory(Board* board)
{
    // Tetrominos
    board->tetrominos = malloc(MAX_TETROMINOS * sizeof(Tetromino*));
    for (uint8_t i = 0; i < MAX_TETROMINOS; i++)
        board->tetrominos[i] = NULL;

    // Map
    board->map = malloc(sizeof(uint8_t*) * MAP_HEIGHT);
    for (uint8_t i = 0; i < MAP_HEIGHT; i++)
        board->map[i] = calloc(MAP_WIDTH, sizeof(uint8_t));

    // Map_next
    board->map_next = malloc(sizeof(uint8_t*) * MINI_MAP_HEIGHT);
    for (uint8_t i = 0; i < MINI_MAP_HEIGHT; i++)
        board->map_next[i] = calloc(MINI_MAP_WIDTH, sizeof(uint8_t));

    // Preview
    board->preview = malloc(sizeof(uint8_t*) * NB_SQUARES);
    for (uint8_t i = 0; i < NB_SQUARES; i++)
        board->preview[i] = calloc(2, sizeof(uint8_t));

    // Holes
    board->holes = calloc(MAP_HEIGHT, sizeof(uint8_t));
}


/**
 * Init all the subboards needed in the board.
 *
 * \param board the board which will contains the subboards.
*/
void init_subboards(Board* board) 
{
    // Subboard
    if (board->show_right)
    {
        board->pos_subboard.x = board->pos.x + board->pos.w - board->scale[0] - board->square_size[0];
        board->pos_subboard.y = board->pos.y;
        board->pos_subboard.w = GAME_SUBBOARD_NEXT.w * board->scale[0];
        board->pos_subboard.h = GAME_SUBBOARD_NEXT.h * board->scale[1];
    }
    else
    {
        board->pos_subboard.x = board->pos.x - GAME_SUBBOARD_NEXT.w * board->scale[0] + board->square_size[0] + board->scale[0];
        board->pos_subboard.y = board->pos.y;
        board->pos_subboard.w = GAME_SUBBOARD_NEXT.w * board->scale[0];
        board->pos_subboard.h = GAME_SUBBOARD_NEXT.h * board->scale[1];
    }

    // Stats board
    board->pos_stats_board.x = board->pos_subboard.x;
    board->pos_stats_board.y = board->pos_subboard.y + board->pos_subboard.h - board->scale[0] - board->square_size[0];
    board->pos_stats_board.w = GAME_STATS.w * board->scale[0];
    board->pos_stats_board.h = GAME_STATS.h * board->scale[1];
}


/**
 * Init all the texts objects for that board.
 * 
 * \param board the board which will contains the texts.
*/
void init_texts(Board* board) 
{
    // Level text
    char* level_text = malloc(2 * sizeof(char));
    level_text[0] = '0'; level_text[1] = '\0';
    board->level_text = create_text(level_text,
        board->pos_stats_board.x + board->pos_stats_board.w / 2 - (8 * board->scale[0]) + 0.5f * board->square_size[0],
        board->pos_stats_board.y + board->scale[1] + board->square_size[1] * 3 - 0.5f * board->square_size[1],
        8 * board->scale[0]
    );

    // Score text
    char* score_text = malloc(2 * sizeof(char));
    score_text[0] = '0'; score_text[1] = '\0';
    board->score_text = create_text(score_text,
        board->pos_stats_board.x + board->pos_stats_board.w / 2 - (8 * board->scale[0]) + 0.5f * board->square_size[0],
        board->pos_stats_board.y + board->scale[1] + board->square_size[1] * 7 - 0.5f * board->square_size[1],
        8 * board->scale[0]
    );

    // Time text
    char* time_text = malloc(2 * sizeof(char));
    time_text[0] = '0'; time_text[1] = '\0';
    board->time_text = create_text(time_text,
        board->pos_stats_board.x + board->pos_stats_board.w / 2 - (8 * board->scale[0]) + 0.5f * board->square_size[0],
        board->pos_stats_board.y + board->scale[1] + board->square_size[1] * 11 - 0.5f * board->square_size[1],
        8 * board->scale[0]
    );
}


/**
 * Keyboard event for this board.
 * 
 * \param board the board concerned by the keyboard event.
 * \param key the key pressed.
*/
void keyboard_event(Board* board, SDL_Keycode key) 
{
    if (board->full || board->actual_tetromino == NULL)
        return;

    int8_t x = 0;
    int8_t y = 0;
    bool rotate = false;
    bool hard_drop = false;
    switch(key) 
    {
        case SDLK_RIGHT:
        case SDLK_d:
            x++;
            break;
        case SDLK_LEFT:
        case SDLK_q:
            x--;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            y++;
            break;
        case SDLK_r:
            rotate = true;
            break;
        case SDLK_SPACE:
            hard_drop = true;
            break;
    }

    bool need_update = false;

    if (!abs(x) || !y)
    {
        move_tetromino(board, x, y);
        need_update = true;
    }
        
    if (rotate) 
    {
        rotate_tetromino(board);
        need_update = true;
    }

    if (hard_drop)
    {
        hard_drop_tetromino(board);
        need_update = true;
    }

    if(need_update) 
    {
        update_positions(board);
        update_preview(board);
    }
}


/**
 * Main update function for the board.
 * 
 * \param board the board concerned.
 * \param delta_t the time passed for a frame (in ms).
*/
void update_board(Board* board, double delta_t) 
{
    if(board->full)
        return;

    board->time_clock += delta_t;
    board->time_drop += delta_t;

    // New tetromino
    if((board->actual_tetromino == NULL || board->actual_tetromino->blocked) && board->lock_delay_passed)
    {
        check_for_lines(board);
        destroy_useless_tetrominos(board);
        board->full = board_is_full(board);
        if (board->full)
        {
            board->full_callback();
            return;
        }
        summon_tetromino(board);
        update_preview(board);
        update_next_tetromino(board);
    }

    // Time update for the actual tetromino
    if(board->time_drop * board->gravity >= TIME_UPDATE)
    {
        update_tetromino(board);
        update_positions(board);
        update_preview(board);
        board->time_drop = 0;
    }

    // Time clock
    if(board->time_clock >= MS_SECOND)
    {
        board->time++;
        update_text(board->time_text, board->time);
        board->time_clock = 0;
    }

    // Lock 
    if (!board->lock_delay_passed)
        board->time_lock += delta_t;

    if (board->time_lock >= LOCK_DELAY)
    {
        board->lock_delay_passed = tetromino_blocked(board->map, board->actual_tetromino);
        board->time_lock = 0;
    }
}


/**
 * Draw a board on the screen with the renderer.
 * 
 * \param board the board to draw.
 * \param renderer the renderer of the window
*/
void draw_board(Board* board, SDL_Renderer* renderer) 
{
    // Draw the board
    SDL_RenderCopy(renderer, g_game_board, NULL, &board->pos);

    // Draw the preview
    draw_preview(board, renderer);

    // Draw the map inside the board
    draw_map(board, renderer);

    // Draw the subboard next tetromino
    SDL_RenderCopy(renderer, g_game_subboard_next, NULL, &board->pos_subboard);

    // Draw next tetromino
    draw_next_tetromino(board, renderer);

    // Draw the stats board
    SDL_RenderCopy(renderer, g_game_stats, NULL, &board->pos_stats_board);

    // Draw the texts
    draw_text(board->level_text, renderer);
    draw_text(board->score_text, renderer);
    draw_text(board->time_text, renderer);
}


/**
 * Correctly clean a board.
 * 
 * \param board the pointer of the board to be cleaned.
*/
void clean_board(Board** board) 
{
    Board* _board = (*board);

    free(_board->level_text->text);
    clean_text(&_board->level_text);
    free(_board->score_text->text);
    clean_text(&_board->score_text);
    free(_board->time_text->text);
    clean_text(&_board->time_text);

    for(uint8_t i = 0; i < MAP_HEIGHT; i++)
        free(_board->map[i]);
    free(_board->map);

    for(uint8_t i = 0; i < MAX_TETROMINOS; i++)
        if(_board->tetrominos[i] != NULL)
            clean_tetromino(&_board->tetrominos[i]);
            
    for(uint8_t i = 0; i < MINI_MAP_HEIGHT; i++)
        free(_board->map_next[i]);
    free(_board->map_next);

    for(uint8_t i = 0; i < NB_SQUARES; i++)
        free(_board->preview[i]);
    free(_board->preview);
    
    free(_board->holes);
    free(_board->tetrominos);
    free(_board);
    *board = NULL;
}


/**
 * Reset the board to the initial state.
*/
void reset_board(Board* board) 
{
    for (uint8_t i = 0; i < MAX_TETROMINOS; i++)
        if (board->tetrominos[i] != NULL)
            clean_tetromino(&board->tetrominos[i]);

    free(board->level_text->text);
    clean_text(&board->level_text);
    free(board->score_text->text);
    clean_text(&board->score_text);
    free(board->time_text->text);
    clean_text(&board->time_text);

    init_texts(board);

    board->nb_tetrominos = 0;
    board->level = 0;
    board->score = 0;
    board->time = 0;
    board->time_clock = 0;
    board->time_lock = 0;
    board->number_of_lines_blocked = 0;
    board->full = false;
    board->lock_delay_passed = true;
    board->lines_before_level_up = calculate_lines_before_level_up(board);
    board->actual_tetromino = NULL;

    update_next_tetromino(board);
    update_positions(board);

    update_gravity(board);

    reset_map(board);
}


/**
 * Check if a square [x, y] is blocked in the map.
 * Check if it is outside the map, if there is another tetromino just below.
 *
 * \param map the map associated with the board.
 * \param x the x coordinate of the square.
 * \param y the y coordinate of the square.
 * \param tetromino the actual tetromino.
 * \return true if the square is blocked, false else.
*/
bool square_blocked(uint8_t** map, int8_t x, int8_t y, Tetromino* tetromino)
{
    // If outside the map
    if (y < -1)
        return false;

    // Last line
    if (y + 1 >= MAP_HEIGHT)
        return true;

    // There is possibly another tetromino below with the same color
    if (map[y + 1][x] == (uint8_t)tetromino->color)
        return !square_in_tetromino(tetromino, x, y + 1);

    // There is another tetromino below
    if (map[y + 1][x])
        return true;

    return false;
}


/**
 * Add lines to the board with one random hole.
 * The new lines will be added at the bottom of the board.
 * 
 * \param board the board concerned.
 * \param number_of_lines the number of line to be added.
*/
void add_random_lines(Board* board, uint8_t number_of_lines) 
{
    // Move all tetrominos up
    for (uint8_t i = 0; i < board->nb_tetrominos; i++)
    {
        Tetromino* tetromino = board->tetrominos[i];
        if (tetromino == NULL)
            continue;

        if (same_tetrominos(tetromino, board->actual_tetromino))
            continue;

        for (uint8_t j = 0; j < tetromino->nb_squares; j++)
        {
            Square* square = tetromino->squares[j];
            square->y -= number_of_lines;
        }
    }

    if (tetromino_blocked(board->map, board->actual_tetromino))
    {
        for (uint8_t j = 0; j < board->actual_tetromino->nb_squares; j++)
        {
            Square* square = board->actual_tetromino->squares[j];
            square->y -= number_of_lines;
        }
    }

    // Move holes up
    if (board->number_of_lines_blocked > 0) 
    {
        for (uint8_t i = 0; i < board->number_of_lines_blocked; i++) 
        {
            int8_t new_index = MAP_HEIGHT - board->number_of_lines_blocked - number_of_lines + i;
            if (new_index < 0)
                continue;
            board->holes[new_index] = board->holes[MAP_HEIGHT - board->number_of_lines_blocked + i];
        }
    }    

    if (board->number_of_lines_blocked + number_of_lines >= MAP_HEIGHT - 1)
    {
        board->full = true;
        board->full_callback();
        return;
    }

    // Add new random holes
    for (uint8_t i = 0; i < number_of_lines; i++)
    {
        uint8_t random = rand() % MAP_WIDTH;
        board->holes[MAP_HEIGHT - 1 - i] = random;
    }
    board->number_of_lines_blocked = fmin(board->number_of_lines_blocked + number_of_lines, MAP_HEIGHT - 1);
    update_positions(board);
    update_preview(board);
    board->full = board_is_full(board);
    if (board->full)
        board->full_callback();
}


/**
 * Check with the movement of x amount is possible.
 * 
 * \param board the board concerned.
 * \param x the amount of x coordinate to be added to all the squares of the tetromino.
 * \return true if the move is possible, false else.
*/
bool move_possible_x(Board* board, int8_t x) 
{
    Tetromino* tetromino = board->actual_tetromino;
    for(uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        Square* square = tetromino->squares[i];

        if (square->x + x > MAP_WIDTH - 1 || square->x + x < 0)
            return false;

        if (square->y < 0)
            continue;

        if (board->map[square->y][square->x + x] == (int)tetromino->color)
        {
            if (!square_in_tetromino(tetromino, square->x + x, square->y))
                return false;
            else
                continue;
        }

        if (board->map[square->y][square->x + x])
            return false;
    }
    return true;
}


/**
 * Check with the movement of y amount is possible.
 * 
 * \param board the board concerned.
 * \param y the amount of y coordinate to be added to all the squares of the tetromino.
 * \return true if the move is possible, false else.
*/
bool move_possible_y(Board* board, int8_t y) 
{
    Tetromino* tetromino = board->actual_tetromino;
    for(uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        Square* square = tetromino->squares[i];
        if(square_blocked(board->map, square->x, square->y + y, tetromino)) 
            return false;
    }
    return true;
}


/**
 * Try to move the actual tetromino of the board.
 * First, check if the move with the position of the tetromino plus x and y is possible.
 * If it isn't just do nothing, else update the position.
 * 
 * \param board the board concerned by the move.
 * \param x the amount of x coordinate to be added to all the squares of the tetromino.
 * \param y the amount of y coordinate to be added to all the squares of the tetromino. 
*/
void move_tetromino(Board* board, int8_t x, int8_t y)
{
    // If there is a movement with x
    if(x != 0)
        if(!move_possible_x(board, x))
            return;

    // If there is a movement with y
    if(y != 0)
        if(!move_possible_y(board, y))
            return;

    for(uint8_t i = 0; i < board->actual_tetromino->nb_squares; i++)
    {
        Square* square = board->actual_tetromino->squares[i];
        
        square->x += x;
        square->y += y;
    }

    if(y)
        update_score(board, SCORE_PER_SOFT_DROP);
}


/* #################### UPDATE #################### */

/**
 * Summon a new tetromino to the board of the game.
 * 
 * \param board the board which summon a new tetromino.
*/
void summon_tetromino(Board* board) 
{
    add_tetromino_to_board(board, board->next_tetromino);
    board->actual_tetromino = board->next_tetromino;
}


/**
 * Update the position of the actual tetromino.
 * 
 * \param board the board containing the tetromino to update.
*/
void update_tetromino(Board* board) 
{
    Tetromino* tetromino = board->actual_tetromino;

    if(tetromino_blocked(board->map, tetromino))
    {
        tetromino->blocked = true;
        board->lock_delay_passed = false;
        return;
    }

    for(uint8_t i = 0; i < tetromino->nb_squares; i++)
        tetromino->squares[i]->y++;
}


/**
 * Update all the tetrominos position to the map.
 * 
 * \param board to board to update the positions.
*/
void update_positions(Board* board) 
{
    reset_map(board);

    // Foreach tetrominos, copy position in the map
    for(uint8_t i = 0; i < board->nb_tetrominos; i++)
    {
        Tetromino* tetromino = board->tetrominos[i];
        if (tetromino == NULL)
            continue;

        for(uint8_t j = 0; j < tetromino->nb_squares; j++)
        {
            Square* square = tetromino->squares[j];
            if(square->x < 0 || square->y < 0 || square->x >= MAP_WIDTH || square->y >= MAP_HEIGHT || !square->alive)
                continue;
            board->map[square->y][square->x] = tetromino->color;
        }
    }

    // Foreach lines blocked
    for (uint8_t i = 0; i < board->number_of_lines_blocked; i++) 
    {
        for (uint8_t j = 0; j < MAP_WIDTH; j++)
        {
            if (j == board->holes[MAP_HEIGHT - i - 1])
                continue;
            board->map[MAP_HEIGHT - i - 1][j] = GRAY;
        }
    }
}


/**
 * Update the next tetromino for the board.
 * Get a new random tetromino.
 * 
 * \param board the board concerned.
*/
void update_next_tetromino(Board* board) 
{
    reset_mini_map(board);

    board->next_tetromino = get_random_tetromino();

    for(uint8_t i = 0; i < board->next_tetromino->nb_squares; i++)
    {
        Square* square = board->next_tetromino->squares[i];
        board->map_next[square->y + 3][square->x - 2] = (uint8_t) board->next_tetromino->color;
    }
}


/**
 * Update the position of all the squares coordinate for the preview.
 * 
 * \param board the board concerned.
*/
void update_preview(Board* board) 
{
    Tetromino* tetromino = board->actual_tetromino;

    // Set the preview coordinates for all squares
    for(uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        Square* square = tetromino->squares[i];
        board->preview[i][0] = square->x;
        board->preview[i][1] = square->y;
    }

    bool blocked = false;
    while(!blocked) 
    {
        // Check if blocked
        for(uint8_t i = 0; i < tetromino->nb_squares; i++)
        {
            Square* square = tetromino->squares[i];

            if(square_blocked(board->map, square->x, board->preview[i][1], tetromino))
            {
                blocked = true;
                break;
            }
        }

        // Move down
        if(!blocked)
            for(uint8_t i = 0; i < tetromino->nb_squares; i++)
                board->preview[i][1]++;
    }
}


/**
 * Update the score of the board with score_add to add.
 * 
 * \param board the board concerned.
 * \param score_add the score to be added to the board score.
*/
void update_score(Board* board, uint16_t score_add)
{
    board->score += score_add;
    update_text(board->score_text, board->score);
}


/**
 * Update the level of the board
 *
 * \param board the board concerned.
 * \param lines_destroyed the number of lines destroyed.
*/
void update_level(Board* board, uint8_t lines_destroyed) 
{
    board->lines_destroyed += lines_destroyed;
    if (board->lines_before_level_up <= lines_destroyed && board->level < MAX_LEVEL)
    {
        board->level++;
        lines_destroyed -= board->lines_before_level_up;
        board->lines_before_level_up = calculate_lines_before_level_up(board);
        board->lines_before_level_up -= lines_destroyed;
        update_text(board->level_text, board->level);        
    }
    else
        board->lines_before_level_up -= lines_destroyed;
    /*printf("Level : %d | Lines to be destroyed : %d\n", board->level, board->lines_before_level_up);*/
}


/**
 * Update the gravity of the board from its current level.
 * 
 * \param board the board to update.
 **/
void update_gravity(Board* board)
{
    board->gravity = calculate_gravity_by_level(board->level);
}


/**
 * Update a certain text for a certain board with a new value.
 * 
 * \param text the text object to be updated.
 * \param add_value the value to be added to the value of the text.
*/
void update_text(Text* text, uint32_t new_value) 
{
    uint32_t value = atoi(text->text);
    uint8_t digits_before = number_of_digits(value);
    uint8_t digits_after = number_of_digits(new_value);
    if(digits_after != digits_before) 
    {
        text->pos.x -= (text->size * 0.32f) * (digits_after - digits_before);
        text->text = realloc(text->text, (digits_after + 1) * sizeof(char));
    }
    int size = sprintf(text->text, "%d", new_value);
    text->text[size] = '\0';
}


/**
 * Destroy the tetrominos in the board that aren't draw anymore.
 *
 * \param board the board concerned.
*/
void destroy_useless_tetrominos(Board* board) 
{
    uint8_t size = board->nb_tetrominos;
    for(uint8_t i = 0; i < size; i++) 
    {
        Tetromino* tetromino = board->tetrominos[i];
        if (tetromino == NULL)
            continue;

        if (tetromino_is_dead(tetromino))
        {
            clean_tetromino(&tetromino);
            sort_board_tetrominos(board, i);
            board->nb_tetrominos--;
        }
    }
}


/**
 * Take all the tetrominos after the ith position and move them one position back.
 * 
 * \param board the board concerned.
 * \param start the starting position.
*/
void sort_board_tetrominos(Board* board, uint8_t start)
{
    for(uint8_t i = start; i < board->nb_tetrominos; i++) 
        if(i + 1 < MAX_TETROMINOS)
            board->tetrominos[i] = board->tetrominos[i + 1];
}


/**
 * Check if there are some lines to be destroyed (full).
 * 
 * \param board the board concerned.
*/
void check_for_lines(Board* board)
{
    uint8_t* lines = malloc(sizeof(uint8_t) * 4); // 4 = Max of line to be destroyed at once
    uint8_t nb_of_lines = 0;
    for(uint8_t i = 0; i < MAP_HEIGHT; i++)
    {
        uint8_t nb_of_squares = 0;
        for(uint8_t j = 0; j < MAP_WIDTH; j++)
        {
            if(board->map[i][j])
                nb_of_squares++;
        }
        // There is a full line
        if (nb_of_squares == MAP_WIDTH)
        {
            lines[nb_of_lines] = i;
            nb_of_lines++;
        }
    }

    // If there is at least one line to be destroyed
    if (nb_of_lines) 
    {
        destroy_lines(board, lines, nb_of_lines);
        if (board->lines_destroyed_cb != NULL)
            board->lines_destroyed_cb(nb_of_lines);
    }
    free(lines);
}


/**
 * Destroy the lines for the board.
 * Lower the upper line of the.
 * The lines parameter contains the index of the lines from up to down.
 * 
 * \param board the board containing the lines to be destroyed.
 * \param lines the index of the line to be destroyed.
*/
void destroy_lines(Board* board, uint8_t* lines, uint8_t nb_of_lines)
{
    /*printf("Lines [");
    for (uint8_t i = 0; i < nb_of_lines; i++)
    {
        if (i == nb_of_lines - 1)
            printf("%d", lines[i]);
        else
            printf("%d, ", lines[i]);
    }
    printf("] (%d) need to be destroyed\n", nb_of_lines);*/

    uint8_t lines_blocked = 0;
    for (uint8_t i = 0; i < nb_of_lines; i++)
        if (lines[i] >= MAP_HEIGHT - board->number_of_lines_blocked)
            lines_blocked++;

    board->number_of_lines_blocked -= lines_blocked;

    // First: delete the lines completed
    for (uint8_t i = 0; i < nb_of_lines; i++)
    {
        uint8_t line = lines[i];
        for (uint8_t j = 0; j < MAP_WIDTH; j++)
        {
            Square* square = search_square_by_coord(board, j, line);
            if (square == NULL)
                continue;
            square->alive = false;
        }
    }

    // Second: lower the upper lines
    for (uint8_t i = 0; i < nb_of_lines; i++)
    {
        uint8_t line_destroyed = lines[i];
        for(uint8_t line = 0; line < line_destroyed; line++)
        {
            for (uint8_t column = 0; column < MAP_WIDTH; column++)
            {
                if (!board->map[line][column])
                    continue;
                Square* square = search_square_by_coord(board, column, line);
                if (square == NULL)
                    continue;
                square->y += 1;
            }
        }
        update_positions(board);
    }

    update_level(board, nb_of_lines);
    update_gravity(board);
    update_score(board, calculate_score(board, nb_of_lines));
}


/**
 * Search a square with its coordinates.
 * 
 * \param board the board concerned.
 * \param x the x coordinate of the square (the column).
 * \param y the y coordinate of the square (the line)
 * \return the square if found, NULL else.
*/
Square* search_square_by_coord(Board* board, uint8_t x, uint8_t y) 
{
    for(uint8_t i = 0; i < board->nb_tetrominos; i++) 
    {   
        Tetromino* tetromino = board->tetrominos[i];
        if (tetromino == NULL)
            continue;

        for (uint8_t j = 0; j < tetromino->nb_squares; j++)
        {
            Square* square = tetromino->squares[j];
            if (square->x == x && square->y == y && square->alive)
                return square;
        }
    }
    return NULL;
}


/**
 * Reset all the map of the board to the initial state.
 * 
 * \param board the board containing the map to be reseted.
*/
void reset_map(Board* board) 
{
    for(uint8_t i = 0; i < MAP_HEIGHT; i++)
        for (uint8_t j = 0; j < MAP_WIDTH; j++)
            board->map[i][j] = 0;
}


/**
 * Reset all the mini-map (map for next tetromino) of the board to the initial state.
 * 
 * \param board the board containing the mini-map to be reseted.
*/
void reset_mini_map(Board* board) 
{
    for(uint8_t i = 0; i < MINI_MAP_HEIGHT; i++)
        for (uint8_t j = 0; j < MINI_MAP_WIDTH; j++)
            board->map_next[i][j] = 0;
}


/**
 * Add a tetromino to the board.
 * 
 * \param board the board.
*/
void add_tetromino_to_board(Board* board, Tetromino* tetromino) 
{
    for(uint8_t i = 0; i < MAX_TETROMINOS; i++)
    {
        if (board->tetrominos[i] == NULL)
        {
            board->tetrominos[i] = tetromino;
            board->nb_tetrominos++;
            return;
        }
    }
}


/**
 * Check if the tetromino is on the ground on the board.
 * 
 * \param board the board of the tetromino.
 * \param tetromino the tetromino to search if it is on ground.
 * \return true if the tetromino is blocked, false else.
*/
bool tetromino_blocked(uint8_t** map, Tetromino* tetromino) 
{
    for(uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        Square* square = tetromino->squares[i];

        if (square_blocked(map, square->x, square->y, tetromino))
            return true;
    }
    return false;
}


/**
 * Try to rotate the actual tetromino of the board.
 * 
 * \param board the board concerned.
*/
void rotate_tetromino(Board* board) 
{
    Tetromino* tetromino = board->actual_tetromino;
    
    int8_t** moves = get_next_rotation_moves(tetromino);
    if (rotation_possible(board, moves)) 
    {
        for(uint8_t i = 0; i < tetromino->nb_squares; i++)
        {
            tetromino->squares[i]->x += moves[i][0];
            tetromino->squares[i]->y += moves[i][1];
        }
        update_rotation(tetromino);
    }

    for (uint8_t i = 0; i < tetromino->nb_squares; i++)
        free(moves[i]);
    free(moves);
}


/**
 * Check if the rotation of the actual tetromino for this board
 * is possible with theses moves.
 * 
 * \param board the board concerned.
 * \param moves the array of moves [nb_squares][x & y].
 * \return true if the rotation is possible, false else.
*/
bool rotation_possible(Board* board, int8_t** moves)
{
    Tetromino* tetromino = board->actual_tetromino;
    for(uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        Square* square = tetromino->squares[i];

        if (square_blocked(board->map, square->x + moves[i][0], square->y + moves[i][1], tetromino))
            return false;
    }
    return true;
}


/**
 * Check if the board is full i.e. that the game is over.
 * 
 * \param board the board concerned.
*/
bool board_is_full(Board* board) 
{
    for (uint8_t i = 0; i < MAP_WIDTH; i++)
        if (board->map[0][i] && (!square_in_tetromino(board->actual_tetromino, i, 0) || tetromino_blocked(board->map, board->actual_tetromino)))
            return true;
    return false;
}


/**
 * Do a hard drop for that board.
 * "Teleport" the actual tetromino to the preview.
 * 
 * \param board the board concerned.
*/
void hard_drop_tetromino(Board* board) 
{
    Tetromino* tetromino = board->actual_tetromino;
    uint8_t score = 0;
    for (uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        Square* square = tetromino->squares[i];
        score += (board->preview[i][1] - square->y) * 2;
        square->x = board->preview[i][0];
        square->y = board->preview[i][1];
    }
    tetromino->blocked = true;
    board->lock_delay_passed = true;
    update_score(board, score);
}


/* #################### DRAW #################### */

/**
 * Draw the map of the board.
 * 
 * \param board the board concerned.
 * \param renderer the renderer of the window.
*/
void draw_map(Board* board, SDL_Renderer* renderer) 
{
    SDL_Rect pos;
    for(uint8_t i = 0; i < MAP_HEIGHT; i++)
    {
        for(uint8_t j = 0; j < MAP_WIDTH; j++)
        {
            if(!board->map[i][j])
                continue;

            pos.x = (board->pos.x + board->square_size[0] * (j + 1)) + board->scale[0];
            pos.y = (board->pos.y + board->square_size[1] * (i + 1)) + board->scale[1];
            pos.w = board->square_size[0] - board->scale[0];
            pos.h = board->square_size[1] - board->scale[1];
            SDL_RenderCopy(renderer, g_squares[board->map[i][j]], NULL, &pos);
        }
    }
}


/**
 * Draw the next tetromino in the subboard.
 * 
 * \param board the board concerned.
 * \param renderer the renderer of the window.
*/
void draw_next_tetromino(Board* board, SDL_Renderer* renderer)
{
    uint8_t correction = 0;
    switch(board->next_tetromino->block_type) 
    {
        case T:
        case L:
        case J:
        case Z:
        case S:
            correction = board->square_size[0] * 0.5;
            break;
        case I:
        case O:
        case B_NONE:
        case B_MAX:
        default:
            break;
    }

    SDL_Rect pos;
    for(uint8_t i = 0; i < MINI_MAP_HEIGHT; i++)
    {
        for(uint8_t j = 0; j < MINI_MAP_WIDTH; j++)
        {
            if(!board->map_next[i][j])
                continue;

            pos.x = (board->pos_subboard.x + board->square_size[0] * (j + 1)) + board->scale[0] - 1 + correction;
            pos.y = (board->pos_subboard.y + board->square_size[1] * (i + 3)) + board->scale[1] - 1;
            pos.w = board->square_size[0] - board->scale[0];
            pos.h = board->square_size[1] - board->scale[1];
            SDL_RenderCopy(renderer, g_squares[board->map_next[i][j]], NULL, &pos);
        }
    }
}


/**
 * Draw the preview of the actual tetromino.
 * 
 * \param board the board concerned.
 * \param renderer the renderer of the window.
*/
void draw_preview(Board* board, SDL_Renderer* renderer) 
{
    if (board->actual_tetromino == NULL)
        return;

    SDL_Rect pos;
    for(uint8_t i = 0; i < NB_SQUARES; i++)
    {
        pos.x = (board->pos.x + board->square_size[0] * (board->preview[i][0] + 1)) + board->scale[0];
        pos.y = (board->pos.y + board->square_size[1] * (board->preview[i][1] + 1)) + board->scale[1];
        pos.w = board->square_size[0] - board->scale[0];
        pos.h = board->square_size[1] - board->scale[1];
        SDL_RenderCopy(renderer, g_frames[board->actual_tetromino->color], NULL, &pos);
    }
}


/* #################### UTILS #################### */

/**
 * Calculate the number of lines before leveling up.
 *
 * \param board the board concerned.
 * \return the number of lines to be destroyed before leveling up.
*/
uint8_t calculate_lines_before_level_up(Board* board) 
{
    if (board->level <= 9)
        return (board->level + 1) * 10;
    if (board->level <= 15)
        return 100;
    if (board->level <= 25)
        return (board->level * 10) - 50;
    return 200;
}


/**
 * Calculate the score for that board level with that number of
 * lines destroyed.
 *
 * \param board the board concerned.
 * \param nb_of_lines the number of lines destroyed.
 * \return the score calculated to be added to the score of the board.
*/
uint16_t calculate_score(Board* board, uint8_t nb_of_lines) 
{
    uint16_t base = 0;
    switch(nb_of_lines) 
    {
        case 1:
            base = 40;
            break;
        case 2:
            base = 100;
            break;
        case 3:
            base = 300;
            break;
        case 4:
            base = 1200;
            break;
        default:
            base = 0;
            break;
    }
    return base * (board->level + 1);
}


/**
 * Calculate the gravity corresponding to the desired level.
 *
 * \param level the level number to calculate the gravity from.
 * \return the gravity calculated to be updated on the board.
*/
float calculate_gravity_by_level(uint8_t level) 
{
    float gravity = 1;

    if (level <= 8)
    {
        gravity = 48 - (5 * level);
    }
    else
    {
        if (level == 9)
            gravity = 6;
        else if (level <= 12)
            gravity = 5;
        else if (level <= 15)
            gravity = 4;
        else if (level <= 18)
            gravity = 3;
        else if (level <= 28)
            gravity = 2;
        else
            gravity = 1;
    }    
    
    return 48/gravity;
}
