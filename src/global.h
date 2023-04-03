#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL2/SDL.h>

#include <stdbool.h>

#define CHAR_WIDTH 32
#define CHAR_HEIGHT 32
#define CHAR_OFFSET 32

/* Path to board sprite image */
extern const char* PATH_BOARD_SPRITE;
extern const char* PATH_FONT_SPRITE;
extern const char* PATH_ICON_SPRITE;

/* Cursors */
extern SDL_Cursor* g_cursor_normal;
extern SDL_Cursor* g_cursor_hovered;

/* ############## SDL_Rect ############## */
// Game variables //
extern const SDL_Rect GAME_BOARD;
extern const SDL_Rect GAME_SUBBOARD_NEXT;
extern const SDL_Rect GAME_STATS;

/* ############## SDL_Texture ############## */
extern SDL_Texture* g_board_sprite; // The main board with all sprites in it
extern SDL_Texture* g_font_sprite;  // The font sprite with all the chars

// Tetrominos
extern SDL_Texture* g_t_light_blue;
extern SDL_Texture* g_t_yellow;
extern SDL_Texture* g_t_purple;
extern SDL_Texture* g_t_orange;
extern SDL_Texture* g_t_dark_blue;
extern SDL_Texture* g_t_red;
extern SDL_Texture* g_t_green;
extern SDL_Texture* g_t_gray;

// Frames
extern SDL_Texture* g_f_light_blue;
extern SDL_Texture* g_f_yellow;
extern SDL_Texture* g_f_purple;
extern SDL_Texture* g_f_orange;
extern SDL_Texture* g_f_dark_blue;
extern SDL_Texture* g_f_red;
extern SDL_Texture* g_f_green;
extern SDL_Texture* g_f_gray;

// Buttons (normal)
extern SDL_Texture* g_btn_n_sp;
extern SDL_Texture* g_btn_n_ia;
extern SDL_Texture* g_btn_n_opts;
extern SDL_Texture* g_btn_n_exit;
extern SDL_Texture* g_btn_n_start;
extern SDL_Texture* g_btn_n_pause;
extern SDL_Texture* g_btn_n_resume;
extern SDL_Texture* g_btn_n_restart;
extern SDL_Texture* g_btn_n_back;
extern SDL_Texture* g_btn_n_easy;
extern SDL_Texture* g_btn_n_medium;
extern SDL_Texture* g_btn_n_hard;
extern SDL_Texture* g_btn_n_extreme;

// Buttons (hovered)
extern SDL_Texture* g_btn_h_sp;
extern SDL_Texture* g_btn_h_ia;
extern SDL_Texture* g_btn_h_opts;
extern SDL_Texture* g_btn_h_exit;
extern SDL_Texture* g_btn_h_start;
extern SDL_Texture* g_btn_h_pause;
extern SDL_Texture* g_btn_h_resume;
extern SDL_Texture* g_btn_h_restart;
extern SDL_Texture* g_btn_h_back;
extern SDL_Texture* g_btn_h_easy;
extern SDL_Texture* g_btn_h_medium;
extern SDL_Texture* g_btn_h_hard;
extern SDL_Texture* g_btn_h_extreme;

// Game variables
extern SDL_Texture* g_game_board;
extern SDL_Texture* g_game_subboard_next;
extern SDL_Texture* g_game_stats;

// Keys variables
extern SDL_Texture* g_key_space;
extern SDL_Texture* g_key_q;
extern SDL_Texture* g_key_s;
extern SDL_Texture* g_key_d;
extern SDL_Texture* g_key_r;

// Board variables
extern SDL_Texture** g_squares;
extern SDL_Texture** g_frames;

// Chars
extern SDL_Texture** g_chars;
extern uint8_t nb_chars;

bool init_sprite_textures(SDL_Renderer* renderer, SDL_Texture* board_sprite);
bool init_font_textures(SDL_Renderer* renderer, SDL_Texture* font_sprite);
bool init_cursors();
void free_textures();

#endif // GLOBAL_H
