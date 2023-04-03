#include "global.h"

#include "utils.h"
#include "tetromino.h"

#include <stdio.h>

/* Path to board sprite image */
const char* PATH_BOARD_SPRITE = "../imgs/tetris.bmp";
const char* PATH_FONT_SPRITE = "../imgs/font.bmp";
const char* PATH_ICON_SPRITE = "../imgs/icon.bmp";

/* Cursors */
SDL_Cursor* g_cursor_normal;
SDL_Cursor* g_cursor_hovered;

/* ############## SDL_Rect ############## */
// Tetrominos
const SDL_Rect T_LIGHT_BLUE = {154, 161, 7, 7};
const SDL_Rect T_YELLOW = {163, 161, 7, 7};
const SDL_Rect T_PURPLE = {172, 161, 7, 7};
const SDL_Rect T_ORANGE = {181, 161, 7, 7};
const SDL_Rect T_DARK_BLUE = {190, 161, 7, 7};
const SDL_Rect T_RED = {199, 161, 7, 7};
const SDL_Rect T_GREEN = {208, 161, 7, 7};
const SDL_Rect T_GRAY = {217, 161, 7, 7};

// Frames
const SDL_Rect F_LIGHT_BLUE = {154, 169, 7, 7};
const SDL_Rect F_YELLOW = {163, 169, 7, 7};
const SDL_Rect F_PURPLE = {172, 169, 7, 7};
const SDL_Rect F_ORANGE = {181, 169, 7, 7};
const SDL_Rect F_DARK_BLUE = {190, 169, 7, 7};
const SDL_Rect F_RED = {199, 169, 7, 7};
const SDL_Rect F_GREEN = {208, 169, 7, 7};
const SDL_Rect F_GRAY = {216, 169, 7, 7};

// Buttons (normal)
const SDL_Rect BTN_N_SP = {153, 0, 56, 11};
const SDL_Rect BTN_N_IA = {153, 11, 56, 11};
const SDL_Rect BTN_N_OPTS = {153, 22, 56, 11};
const SDL_Rect BTN_N_EXIT = {153, 33, 56, 11};
const SDL_Rect BTN_N_START = {153, 44, 56, 11};
const SDL_Rect BTN_N_PAUSE = {153, 55, 56, 11};
const SDL_Rect BTN_N_RESUME = {153, 66, 56, 11};
const SDL_Rect BTN_N_RESTART = {153, 77, 56, 11};
const SDL_Rect BTN_N_BACK = {153, 88, 56, 11};
const SDL_Rect BTN_N_EASY = {153, 99, 56, 11};
const SDL_Rect BTN_N_MEDIUM = {153, 110, 56, 11};
const SDL_Rect BTN_N_HARD = {153, 121, 56, 11};
const SDL_Rect BTN_N_EXTREME = {153, 132, 56, 11};

// Buttons (hovered)
const SDL_Rect BTN_H_SP = {209, 0, 56, 11};
const SDL_Rect BTN_H_IA = {209, 11, 56, 11};
const SDL_Rect BTN_H_OPTS = {209, 22, 56, 11};
const SDL_Rect BTN_H_EXIT = {209, 33, 56, 11};
const SDL_Rect BTN_H_START = {209, 44, 56, 11};
const SDL_Rect BTN_H_PAUSE = {209, 55, 56, 11};
const SDL_Rect BTN_H_RESUME = {209, 66, 56, 11};
const SDL_Rect BTN_H_RESTART = {209, 77, 56, 11};
const SDL_Rect BTN_H_BACK = {209, 88, 56, 11};
const SDL_Rect BTN_H_EASY = {209, 99, 56, 11};
const SDL_Rect BTN_H_MEDIUM = {209, 110, 56, 11};
const SDL_Rect BTN_H_HARD = {209, 121, 56, 11};
const SDL_Rect BTN_H_EXTREME = {209, 132, 56, 11};

// Game variables
const SDL_Rect GAME_BOARD = {0, 0, 97, 177};
const SDL_Rect GAME_SUBBOARD_NEXT = {88, 0, 65, 57};
const SDL_Rect GAME_STATS = {88, 48, 65, 128};

// Keys variables
const SDL_Rect KEY_SPACE = {153, 143, 16, 16};
const SDL_Rect KEY_Q = {170, 143, 16, 16};
const SDL_Rect KEY_S = {187, 143, 16, 16};
const SDL_Rect KEY_D = {204, 143, 16, 16};
const SDL_Rect KEY_R = {221, 143, 16, 16};

/* ############## SDL_Texture ############## */
SDL_Texture* g_board_sprite; // The main board with all sprites in it
SDL_Texture* g_font_sprite;  // The font sprite with all the chars

// Tetrominos
SDL_Texture* g_t_light_blue;
SDL_Texture* g_t_yellow;
SDL_Texture* g_t_purple;
SDL_Texture* g_t_orange;
SDL_Texture* g_t_dark_blue;
SDL_Texture* g_t_red;
SDL_Texture* g_t_green;
SDL_Texture* g_t_gray;

// Frames
SDL_Texture* g_f_light_blue;
SDL_Texture* g_f_yellow;
SDL_Texture* g_f_purple;
SDL_Texture* g_f_orange;
SDL_Texture* g_f_dark_blue;
SDL_Texture* g_f_red;
SDL_Texture* g_f_green;
SDL_Texture* g_f_gray;

// Buttons (normal)
SDL_Texture* g_btn_n_sp;
SDL_Texture* g_btn_n_ia;
SDL_Texture* g_btn_n_opts;
SDL_Texture* g_btn_n_exit;
SDL_Texture* g_btn_n_start;
SDL_Texture* g_btn_n_pause;
SDL_Texture* g_btn_n_resume;
SDL_Texture* g_btn_n_restart;
SDL_Texture* g_btn_n_back;
SDL_Texture* g_btn_n_easy;
SDL_Texture* g_btn_n_medium;
SDL_Texture* g_btn_n_hard;
SDL_Texture* g_btn_n_extreme;

// Buttons (hovered)
SDL_Texture* g_btn_h_sp;
SDL_Texture* g_btn_h_ia;
SDL_Texture* g_btn_h_opts;
SDL_Texture* g_btn_h_exit;
SDL_Texture* g_btn_h_start;
SDL_Texture* g_btn_h_pause;
SDL_Texture* g_btn_h_resume;
SDL_Texture* g_btn_h_restart;
SDL_Texture* g_btn_h_back;
SDL_Texture* g_btn_h_easy;
SDL_Texture* g_btn_h_medium;
SDL_Texture* g_btn_h_hard;
SDL_Texture* g_btn_h_extreme;

// Game variables
SDL_Texture* g_game_board;
SDL_Texture* g_game_subboard_next;
SDL_Texture* g_game_stats;

// Keys variables
SDL_Texture* g_key_space;
SDL_Texture* g_key_q;
SDL_Texture* g_key_s;
SDL_Texture* g_key_d;
SDL_Texture* g_key_r;

// Board variables
SDL_Texture** g_squares;
SDL_Texture** g_frames;

// Chars
SDL_Texture** g_chars;
uint8_t nb_chars;


/**
 * Init all the sprite textures for the entire game.
 * 
 * \param renderer the renderer of the window.
 * \param board_sprite the board with all sprites in it.
*/
bool init_sprite_textures(SDL_Renderer* renderer, SDL_Texture* board_sprite) 
{
    // Main board
    g_board_sprite = board_sprite;

    // Tetrominos
    g_t_light_blue = get_area_texture(T_LIGHT_BLUE, renderer, board_sprite);
    g_t_yellow = get_area_texture(T_YELLOW, renderer, board_sprite);
    g_t_purple = get_area_texture(T_PURPLE, renderer, board_sprite);
    g_t_orange = get_area_texture(T_ORANGE, renderer, board_sprite);
    g_t_dark_blue = get_area_texture(T_DARK_BLUE, renderer, board_sprite);
    g_t_red = get_area_texture(T_RED, renderer, board_sprite);
    g_t_green = get_area_texture(T_GREEN, renderer, board_sprite);
    g_t_gray = get_area_texture(T_GRAY, renderer, board_sprite);

    // Frames
    g_f_light_blue = get_area_texture(F_LIGHT_BLUE, renderer, board_sprite);
    g_f_yellow = get_area_texture(F_YELLOW, renderer, board_sprite);
    g_f_purple = get_area_texture(F_PURPLE, renderer, board_sprite);
    g_f_orange = get_area_texture(F_ORANGE, renderer, board_sprite);
    g_f_dark_blue = get_area_texture(F_DARK_BLUE, renderer, board_sprite);
    g_f_red = get_area_texture(F_RED, renderer, board_sprite);
    g_f_green = get_area_texture(F_GREEN, renderer, board_sprite);
    g_f_gray = get_area_texture(F_GRAY, renderer, board_sprite);

    // Buttons (normal)
    g_btn_n_sp = get_area_texture(BTN_N_SP, renderer, board_sprite);
    g_btn_n_ia = get_area_texture(BTN_N_IA, renderer, board_sprite);
    g_btn_n_opts = get_area_texture(BTN_N_OPTS, renderer, board_sprite);
    g_btn_n_exit = get_area_texture(BTN_N_EXIT, renderer, board_sprite);
    g_btn_n_start = get_area_texture(BTN_N_START, renderer, board_sprite);
    g_btn_n_pause = get_area_texture(BTN_N_PAUSE, renderer, board_sprite);
    g_btn_n_resume = get_area_texture(BTN_N_RESUME, renderer, board_sprite);
    g_btn_n_restart = get_area_texture(BTN_N_RESTART, renderer, board_sprite);
    g_btn_n_back = get_area_texture(BTN_N_BACK, renderer, board_sprite);
    g_btn_n_easy = get_area_texture(BTN_N_EASY, renderer, board_sprite);
    g_btn_n_medium = get_area_texture(BTN_N_MEDIUM, renderer, board_sprite);
    g_btn_n_hard = get_area_texture(BTN_N_HARD, renderer, board_sprite);
    g_btn_n_extreme = get_area_texture(BTN_N_EXTREME, renderer, board_sprite);

    // Buttons (hovered)
    g_btn_h_sp = get_area_texture(BTN_H_SP, renderer, board_sprite);
    g_btn_h_ia = get_area_texture(BTN_H_IA, renderer, board_sprite);
    g_btn_h_opts = get_area_texture(BTN_H_OPTS, renderer, board_sprite);
    g_btn_h_exit = get_area_texture(BTN_H_EXIT, renderer, board_sprite);
    g_btn_h_start = get_area_texture(BTN_H_START, renderer, board_sprite);
    g_btn_h_pause = get_area_texture(BTN_H_PAUSE, renderer, board_sprite);
    g_btn_h_resume = get_area_texture(BTN_H_RESUME, renderer, board_sprite);
    g_btn_h_restart = get_area_texture(BTN_H_RESTART, renderer, board_sprite);
    g_btn_h_back = get_area_texture(BTN_H_BACK, renderer, board_sprite);
    g_btn_h_easy = get_area_texture(BTN_H_EASY, renderer, board_sprite);
    g_btn_h_medium = get_area_texture(BTN_H_MEDIUM, renderer, board_sprite);
    g_btn_h_hard = get_area_texture(BTN_H_HARD, renderer, board_sprite);
    g_btn_h_extreme = get_area_texture(BTN_H_EXTREME, renderer, board_sprite);

    // Game variables
    g_game_board = get_area_texture(GAME_BOARD, renderer, board_sprite);
    g_game_subboard_next = get_area_texture(GAME_SUBBOARD_NEXT, renderer, board_sprite);
    g_game_stats = get_area_texture(GAME_STATS, renderer, board_sprite);

    // Keys variables
    g_key_space = get_area_texture(KEY_SPACE, renderer, board_sprite);
    g_key_q = get_area_texture(KEY_Q, renderer, board_sprite);
    g_key_s = get_area_texture(KEY_S, renderer, board_sprite);
    g_key_d = get_area_texture(KEY_D, renderer, board_sprite);
    g_key_r = get_area_texture(KEY_R, renderer, board_sprite);

    // Board variables
    g_squares = malloc((enum Color) C_MAX * sizeof(SDL_Texture*));
    if (g_squares == NULL)
        return false;
    g_squares[(enum Color) LIGHT_BLUE] = g_t_light_blue;
    g_squares[(enum Color) YELLOW] = g_t_yellow;
    g_squares[(enum Color) PURPLE] = g_t_purple;
    g_squares[(enum Color) ORANGE] = g_t_orange;
    g_squares[(enum Color) DARK_BLUE] = g_t_dark_blue;
    g_squares[(enum Color) RED] = g_t_red; 
    g_squares[(enum Color) GREEN] = g_t_green;
    g_squares[(enum Color) GRAY] = g_t_gray;

    // Frames
    g_frames = malloc((enum Color) C_MAX * sizeof(SDL_Texture*));
    if (g_frames == NULL)
        return false;
    g_frames[(enum Color) LIGHT_BLUE] = g_f_light_blue;
    g_frames[(enum Color) YELLOW] = g_f_yellow;
    g_frames[(enum Color) PURPLE] = g_f_purple;
    g_frames[(enum Color) ORANGE] = g_f_orange;
    g_frames[(enum Color) DARK_BLUE] = g_f_dark_blue;
    g_frames[(enum Color) RED] = g_f_red; 
    g_frames[(enum Color) GREEN] = g_f_green;
    g_frames[(enum Color) GRAY] = g_f_gray;

    return true;
}


/**
 * Init all the font textures.
 *
 * \param renderer the renderer of the window.
 * \param font_sprite the font sprite with all chars in it.
*/
bool init_font_textures(SDL_Renderer* renderer, SDL_Texture* font_sprite) 
{
    g_font_sprite = font_sprite;

    int width, height;
    SDL_QueryTexture(font_sprite, NULL, NULL, &width, &height);
    
    uint8_t columns = width / CHAR_WIDTH;
    uint8_t lines = height / CHAR_HEIGHT;
    nb_chars = lines * columns;

    g_chars = malloc(sizeof(SDL_Texture*) * (nb_chars));
    if (g_chars == NULL)
        return false;

    SDL_Rect rect = { 0, 0, CHAR_WIDTH, CHAR_HEIGHT };
    for(uint8_t i = 0; i < lines; i++) 
    {
        for(uint8_t j = 0; j < columns; j++) 
        {
            rect.x = j * CHAR_WIDTH;
            rect.y = i * CHAR_HEIGHT;
            g_chars[i * columns + j] = get_area_texture(rect, renderer, g_font_sprite);
        }
    }

    return true;
}


/**
 * Init all the cursors need for the game.
 * 
 * \return true if the init worked, false else.
*/
bool init_cursors() 
{
    // Cursors
    g_cursor_normal = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    g_cursor_hovered = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    return true;
}


/**
 * Free / Destroy all the textures used.
*/
void free_textures() 
{
    // Main board
    SDL_DestroyTexture(g_board_sprite);

    // Tetrominos
    SDL_DestroyTexture(g_t_light_blue);
    SDL_DestroyTexture(g_t_yellow);
    SDL_DestroyTexture(g_t_purple);
    SDL_DestroyTexture(g_t_orange);
    SDL_DestroyTexture(g_t_dark_blue);
    SDL_DestroyTexture(g_t_red);
    SDL_DestroyTexture(g_t_green);
    SDL_DestroyTexture(g_t_gray);

    // Frames
    SDL_DestroyTexture(g_f_light_blue);
    SDL_DestroyTexture(g_f_yellow);
    SDL_DestroyTexture(g_f_purple);
    SDL_DestroyTexture(g_f_orange);
    SDL_DestroyTexture(g_f_dark_blue);
    SDL_DestroyTexture(g_f_red);
    SDL_DestroyTexture(g_f_green);
    SDL_DestroyTexture(g_f_gray);

    // Buttons (normal)
    SDL_DestroyTexture(g_btn_n_sp);
    SDL_DestroyTexture(g_btn_n_ia);
    SDL_DestroyTexture(g_btn_n_opts);
    SDL_DestroyTexture(g_btn_n_exit);
    SDL_DestroyTexture(g_btn_n_start);
    SDL_DestroyTexture(g_btn_n_pause);
    SDL_DestroyTexture(g_btn_n_resume);
    SDL_DestroyTexture(g_btn_n_restart);
    SDL_DestroyTexture(g_btn_n_back);
    SDL_DestroyTexture(g_btn_n_easy);
    SDL_DestroyTexture(g_btn_n_medium);
    SDL_DestroyTexture(g_btn_n_hard);
    SDL_DestroyTexture(g_btn_n_extreme);

    // Buttons (hovered)
    SDL_DestroyTexture(g_btn_h_sp);
    SDL_DestroyTexture(g_btn_h_ia);
    SDL_DestroyTexture(g_btn_h_opts);
    SDL_DestroyTexture(g_btn_h_exit);
    SDL_DestroyTexture(g_btn_h_start);
    SDL_DestroyTexture(g_btn_h_pause);
    SDL_DestroyTexture(g_btn_h_resume);
    SDL_DestroyTexture(g_btn_h_restart);
    SDL_DestroyTexture(g_btn_h_back);
    SDL_DestroyTexture(g_btn_h_easy);
    SDL_DestroyTexture(g_btn_h_medium);
    SDL_DestroyTexture(g_btn_h_hard);
    SDL_DestroyTexture(g_btn_h_extreme);

    // Game variables
    SDL_DestroyTexture(g_game_board);
    SDL_DestroyTexture(g_game_subboard_next);
    SDL_DestroyTexture(g_game_stats);

    // Keys variables
    SDL_DestroyTexture(g_key_space);
    SDL_DestroyTexture(g_key_q);
    SDL_DestroyTexture(g_key_s);
    SDL_DestroyTexture(g_key_d);
    SDL_DestroyTexture(g_key_r);

    // Board variables
    free(g_squares);
    g_squares = NULL;
    free(g_frames);
    g_frames = NULL;

    // Chars
    SDL_DestroyTexture(g_font_sprite);
    for (uint8_t i = 0; i < nb_chars; i++)
        SDL_DestroyTexture(g_chars[i]);
    free(g_chars);
    g_chars = NULL;

    // Cursors
    SDL_FreeCursor(g_cursor_normal);
    SDL_FreeCursor(g_cursor_hovered);
}
