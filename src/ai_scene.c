#include "ai_scene.h"

#include "tetris.h"
#include "print_color.h"
#include "utils.h"
#include "global.h"
#include "test.h"

#include <stdio.h>


/* aiscene global structure */
AIScene* ai_scene = NULL;


/* aiscene varaibles */
double ais_delta_t;
uint64_t ais_start, ais_end;
bool ais_started = false;
bool ais_difficulty_choosed = false;
const SDL_Rect board_player_pos = { 230, 4, 388, 708 };
const SDL_Rect board_ai_pos = { 820, 50, 291, 531 };
const char* txt_victory_player = "Player Wins!";
const char* txt_victory_ai = "AI Wins!";

/* Buttons */
Button* ais_btn_start;
Button* ais_btn_restart;
Button* ais_btn_pause;
Button* ais_btn_resume;
Button* ais_btn_back;
Button* ais_btn_easy;
Button* ais_btn_medium;
Button* ais_btn_hard;
Button* ais_btn_extreme;

/* aiscene functions declaration */
bool ais_init();
uint8_t ais_loop();
void ais_clean();

void ais_event();
void ais_keyboard_event(SDL_KeyboardEvent event);
void ais_update();
void ais_draw();

bool ais_create_buttons();
bool ais_create_text();

void ais_show_game_ui(bool show);

/* aiscene callbacks */
void ais_lines_destroyed_player(int nb_of_lines);
void ais_lines_destroyed_ai(int nb_of_lines);
void ais_start_callback();
void ais_restart_callback();
void ais_pause_callback();
void ais_resume_callback();
void ais_back_callback();
void ais_full_player_callback();
void ais_full_ai_callback();
void ais_easy_callback();
void ais_medium_callback();
void ais_hard_callback();
void ais_extreme_callback();


/* #################### PUBLIC #################### */

/**
 * Create the ai scene of the game.
*/
AIScene* create_ai_scene(SDL_Window* window)
{
    AIScene* _ai_scene = malloc(sizeof(AIScene));
	if (_ai_scene == NULL)
		return NULL;
	_ai_scene->buttons = malloc(sizeof(Button*) * AIS_NB_BUTTONS);
	if (_ai_scene->buttons == NULL) 
	{
		free(_ai_scene);
		return NULL;
	}
    _ai_scene->key_texts = malloc(sizeof(KeyText*) * AIS_NB_KEYS);
    if (_ai_scene->key_texts == NULL)
    {
        free(_ai_scene->buttons);
        free(_ai_scene);
        return NULL;
    }
	_ai_scene->nb_buttons = 0;
	_ai_scene->nb_keys = 0;
	_ai_scene->return_code = 0;
	_ai_scene->ais_init = &ais_init;
	_ai_scene->ais_loop = &ais_loop;
	_ai_scene->ais_clean = &ais_clean;
	_ai_scene->window = window;
	_ai_scene->renderer = SDL_GetRenderer(window);
	ai_scene = _ai_scene;
	return _ai_scene;
}


/* #################### AI_SCENE METHODS #################### */

/**
 * Initializing the ai scene.
 *
 * \return true if the init worked, false else.
*/
bool ais_init() 
{
    print_color("   AI Scene init", TXT_GREEN);

    // Board player
    print_color("   Creating board player for AI scene", TXT_GREEN);
    ai_scene->board_player = create_board(board_player_pos, false, &ais_full_player_callback);
    if (ai_scene->board_player == NULL)
    {
        print_color("   Error Creating board player for AI scene", TXT_RED);
        return false;
    }
    ai_scene->board_player->lines_destroyed_cb = &ais_lines_destroyed_player;

    // Board AI
    print_color("   Creating board AI for AI scene", TXT_GREEN);
    ai_scene->board_ai = create_board(board_ai_pos, true, &ais_full_ai_callback);
    if (ai_scene->board_ai == NULL)
    {
        print_color("   Error Creating board AI for AI scene", TXT_RED);
        return false;
    }
    ai_scene->board_ai->lines_destroyed_cb = &ais_lines_destroyed_ai;

    // AI
    print_color("   Creating AI for the AI scene", TXT_GREEN);
    ai_scene->ai = create_ai(ai_scene->board_ai);
    if (ai_scene->ai == NULL) 
    {
        print_color("   Error Creating AI for the AI scene", TXT_RED);
        return false;
    }

    // Buttons
    print_color("   Creating buttons for AI scene", TXT_GREEN);
    if (!ais_create_buttons())
    {
        print_color("   Error Creating buttons for AI scene", TXT_RED);
        return false;
    }

    // Texts
    print_color("   Creating texts for AI scene", TXT_GREEN);
    if (!ais_create_text())
    {
        print_color("   Error Creating texts for AI scene", TXT_RED);
        return false;
    }

    print_color("   End AI Scene init", TXT_GREEN);

    return true;
}


/**
 * Initializing all the buttons for the ai scene.
 * 
 * \return true if the init worked, false else.
*/
bool ais_create_buttons() 
{
    // Start button
    SDL_Rect pos = { 675, 610, 168, 33 };
    Button* btn_start = create_button(g_btn_n_start, pos, &ais_start_callback);
    btn_start->hovered_texture = g_btn_h_start;
    if (btn_start == NULL)
        return false;
    btn_start->visible = false;
    ai_scene->buttons[ai_scene->nb_buttons] = btn_start;
    ai_scene->nb_buttons++;
    ais_btn_start = btn_start;

    // Restart button
    Button* btn_restart = create_button(g_btn_n_restart, pos, &ais_restart_callback);
    btn_restart->hovered_texture = g_btn_h_restart;
    if (btn_restart == NULL)
        return false;
    btn_restart->visible = false;
    ai_scene->buttons[ai_scene->nb_buttons] = btn_restart;
    ai_scene->nb_buttons++;
    ais_btn_restart = btn_restart;

    // Pause button
    SDL_Rect pos2 = { 875, 610, 168, 33 };
    Button* btn_pause = create_button(g_btn_n_pause, pos2, &ais_pause_callback);
    btn_pause->hovered_texture = g_btn_h_pause;
    if (btn_pause == NULL)
        return false;
    btn_pause->visible = false;
    ai_scene->buttons[ai_scene->nb_buttons] = btn_pause;
    ai_scene->nb_buttons++;
    ais_btn_pause = btn_pause;

    // Resume button
    Button* btn_resume = create_button(g_btn_n_resume, pos2, &ais_resume_callback);
    btn_resume->hovered_texture = g_btn_h_resume;
    if (btn_resume == NULL)
        return false;
    btn_resume->visible = false;
    ai_scene->buttons[ai_scene->nb_buttons] = btn_resume;
    ai_scene->nb_buttons++;
    ais_btn_resume = btn_resume;

    // Back button
    SDL_Rect pos3 = { 1075, 610, 168, 33 };
    Button* btn_back = create_button(g_btn_n_back, pos3, &ais_back_callback);
    btn_back->hovered_texture = g_btn_h_back;
    if (btn_back == NULL)
        return false;
    btn_back->visible = false;
    ai_scene->buttons[ai_scene->nb_buttons] = btn_back;
    ai_scene->nb_buttons++;
    ais_btn_back = btn_back;

    // Easy button
    SDL_Rect pos4 = { 390, 200, 500, 75 };
    Button* btn_easy = create_button(g_btn_n_easy, pos4, &ais_easy_callback);
    btn_easy->hovered_texture = g_btn_h_easy;
    if (btn_easy == NULL)
        return false;
    ai_scene->buttons[ai_scene->nb_buttons] = btn_easy;
    ai_scene->nb_buttons++;
    ais_btn_easy = btn_easy;

    // Medium button
    SDL_Rect pos5 = { 390, 300, 500, 75 };
    Button* btn_medium = create_button(g_btn_n_medium, pos5, &ais_medium_callback);
    btn_medium->hovered_texture = g_btn_h_medium;
    if (btn_medium == NULL)
        return false;
    ai_scene->buttons[ai_scene->nb_buttons] = btn_medium;
    ai_scene->nb_buttons++;
    ais_btn_medium = btn_medium;

    // Hard button
    SDL_Rect pos6 = { 390, 400, 500, 75 };
    Button* btn_hard = create_button(g_btn_n_hard, pos6, &ais_hard_callback);
    btn_hard->hovered_texture = g_btn_h_hard;
    if (btn_hard == NULL)
        return false;
    ai_scene->buttons[ai_scene->nb_buttons] = btn_hard;
    ai_scene->nb_buttons++;
    ais_btn_hard = btn_hard;

    // extreme button
    SDL_Rect pos7 = { 390, 500, 500, 75 };
    Button* btn_extreme = create_button(g_btn_n_extreme, pos7, &ais_extreme_callback);
    btn_extreme->hovered_texture = g_btn_h_extreme;
    if (btn_extreme == NULL)
        return false;
    ai_scene->buttons[ai_scene->nb_buttons] = btn_extreme;
    ai_scene->nb_buttons++;
    ais_btn_extreme = btn_extreme;

    return true;
}


/**
 * Creating the text for the ai scene.
 *
 * \return true if the creation worked, false else.
*/
bool ais_create_text() 
{
    const uint8_t text_size = 32;
    const uint8_t key_text_size = 24;
    const uint8_t key_size = 64;
    const uint16_t text_ai_x = ai_scene->board_ai->pos.x + ((GAME_BOARD.w * ai_scene->board_ai->scale[0]) / 2) - text_size;
    const uint16_t text_ai_y = ai_scene->board_ai->pos.y - text_size - ai_scene->board_ai->scale[1];

    ai_scene->text_ai = create_text("AI", text_ai_x, text_ai_y, text_size);
    if (ai_scene->text_ai == NULL)
        return false;

    ai_scene->text_victory = create_text((char*)txt_victory_player, 650, 660, text_size);
    if (ai_scene->text_victory == NULL)
        return false;
    ai_scene->text_victory->visible = false;
    
    ai_scene->text_choose_difficulty = create_text("Chose difficulty :", 433, 92, text_size);
    if (ai_scene->text_choose_difficulty == NULL)
        return false;

    // Key Space
    SDL_Rect pos1 = {630, 50, key_size, key_size };
    KeyText* key_text_space = create_key_text(g_key_space, "DROP", pos1, key_text_size);
    if (key_text_space == NULL)
        return false;
    ai_scene->key_texts[ai_scene->nb_keys] = key_text_space;
    ai_scene->nb_keys++;

    // Key Q
    SDL_Rect pos2 = { 630, 125, key_size, key_size };
    KeyText* key_text_q = create_key_text(g_key_q, "LEFT", pos2, key_text_size);
    if (key_text_q == NULL)
        return false;
    ai_scene->key_texts[ai_scene->nb_keys] = key_text_q;
    ai_scene->nb_keys++;

    // Key S
    SDL_Rect pos3 = { 630, 200, key_size, key_size };
    KeyText* key_text_s = create_key_text(g_key_s, "DOWN", pos3, key_text_size);
    if (key_text_s == NULL)
        return false;
    ai_scene->key_texts[ai_scene->nb_keys] = key_text_s;
    ai_scene->nb_keys++;

    // Key D
    SDL_Rect pos4 = { 630, 275, key_size, key_size };
    KeyText* key_text_d = create_key_text(g_key_d, "RIGHT", pos4, key_text_size);
    if (key_text_d == NULL)
        return false;
    ai_scene->key_texts[ai_scene->nb_keys] = key_text_d;
    ai_scene->nb_keys++;

    // Key R
    SDL_Rect pos5 = { 630, 350, key_size, key_size };
    KeyText* key_text_r = create_key_text(g_key_r, "ROTATE", pos5, key_text_size);
    if (key_text_r == NULL)
        return false;
    ai_scene->key_texts[ai_scene->nb_keys] = key_text_r;
    ai_scene->nb_keys++;

    return true;
}


/**
 * Main loop for the ai scene.
 *
 * \return [T_MENU, T_EXIT]
*/
uint8_t ais_loop() 
{
    print_color("AI Scene loop", TXT_BLUE);
    while (!ai_scene->return_code)
    {
        ais_start = SDL_GetPerformanceCounter();

        ais_event();
        ais_update();
        ais_draw();

        ais_end = SDL_GetPerformanceCounter();

        ais_delta_t = (double)((ais_end - ais_start) * 1000 / (double)SDL_GetPerformanceFrequency());
    }
    print_color("End AI Scene loop", TXT_BLUE);
    return ai_scene->return_code;
}


/**
 * Clean the ai scene.
*/
void ais_clean() 
{
    print_color("  AI Scene clean", TXT_GREEN);

    print_color("    Cleaning board player", TXT_GREEN);
    if (ai_scene->board_player != NULL) clean_board(&ai_scene->board_player);

    print_color("    Cleaning board ai", TXT_GREEN);
    if (ai_scene->board_ai != NULL) clean_board(&ai_scene->board_ai);

    print_color("    Cleaning AI", TXT_GREEN);
    if (ai_scene->ai != NULL) clean_ai(&ai_scene->ai);

    print_color("    Cleaning buttons", TXT_GREEN);
    for (uint8_t i = 0; i < ai_scene->nb_buttons; i++)
        clean_button(&ai_scene->buttons[i]);
    free(ai_scene->buttons);

    print_color("    Cleaning text", TXT_GREEN);
    clean_text(&ai_scene->text_ai);
    clean_text(&ai_scene->text_victory);
    clean_text(&ai_scene->text_choose_difficulty);
    for (uint8_t i = 0; i < ai_scene->nb_keys; i++)
        clean_key_text(&ai_scene->key_texts[i]);
    free(ai_scene->key_texts);

    free(ai_scene);
    ai_scene = NULL;

    print_color("  AI Scene clean finished", TXT_GREEN);
}


/* #################### AI_SCENE LOOP #################### */

/**
 * Handle all events in the ai scene.
*/
void ais_event()
{
    SDL_Event event;
    while (!ai_scene->return_code && SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                ai_scene->return_code = T_EXIT;
                break;
            case SDL_MOUSEBUTTONDOWN:
                check_buttons_click(ai_scene->buttons, ai_scene->nb_buttons, event.button);
                break;
            case SDL_MOUSEMOTION:
                check_buttons_hover(ai_scene->buttons, ai_scene->nb_buttons, event.motion);
                break;
            case SDL_KEYDOWN:
                ais_keyboard_event(event.key);
                break;
            default:
                break;
        }
    }
}


/**
 * Handle all keyboards events in the solo scene.
*/
void ais_keyboard_event(SDL_KeyboardEvent event)
{
    if(ais_started) 
    {
        SDL_Keycode key = event.keysym.sym;
        keyboard_event(ai_scene->board_player, key);
    }
}


/**
 * Update all the objects for the ai scene.
*/
void ais_update() 
{
    if (ais_started) 
    {
        update_board(ai_scene->board_player, ais_delta_t);
        update_board(ai_scene->board_ai, ais_delta_t);
        ai_process(ai_scene->ai, ais_delta_t);
    }
}


/**
 * Draw in the window the objects from the solo scene.
*/
void ais_draw() 
{
    SDL_UpdateWindowSurface(ai_scene->window);
    SDL_RenderClear(ai_scene->renderer);

    if (ais_difficulty_choosed)
    {
        // Draw the boards
        draw_board(ai_scene->board_player, ai_scene->renderer);
        draw_board(ai_scene->board_ai, ai_scene->renderer);

        // Draw texts
        draw_text(ai_scene->text_ai, ai_scene->renderer);
        draw_text(ai_scene->text_victory, ai_scene->renderer);

        // Draw keys
        for (uint8_t i = 0; i < ai_scene->nb_keys; i++)
            draw_key_text(ai_scene->key_texts[i], ai_scene->renderer);
    }
    else 
    {
        // Draw text
        draw_text(ai_scene->text_choose_difficulty, ai_scene->renderer);
    }

    // Draw all the buttons
    for (uint8_t i = 0; i < ai_scene->nb_buttons; i++)
        draw_button(ai_scene->buttons[i], ai_scene->renderer);

    SDL_RenderPresent(ai_scene->renderer);
}


/**
 * Shortcut to show or hide all the games ui and hide the difficulty choice.
 * 
 * \param show if we show the game ui or hide it.
*/
void ais_show_game_ui(bool show) 
{
    ais_btn_easy->visible = !show;
    ais_btn_medium->visible = !show;
    ais_btn_hard->visible = !show;
    ais_btn_extreme->visible = !show;    
    ais_btn_pause->visible = !show;
    ais_btn_start->visible = show;
    ais_btn_back->visible = show;
}


/* #################### CALLBACK METHODS #################### */

/**
 * Callback method for the lines destroyed by the player.
 * 
 * \param nb_of_lines the number of lines destroyed.
*/
void ais_lines_destroyed_player(int nb_of_lines)
{
    add_random_lines(ai_scene->board_ai, nb_of_lines);
}


/**
 * Callback method for the lines destroyed by the ai.
 * 
 * \param nb_of_lines the number of lines destroyed.
*/
void ais_lines_destroyed_ai(int nb_of_lines) 
{
    add_random_lines(ai_scene->board_player, nb_of_lines);
}


/**
 * Callback method for the start button.
*/
void ais_start_callback() 
{
    ais_started = true;
    ais_btn_start->visible = false;
    ais_btn_pause->visible = true;
}


/**
 * Callback method for the restart button.
*/
void ais_restart_callback() 
{
    ais_btn_restart->visible = false;
    reset_board(ai_scene->board_player);
    reset_board(ai_scene->board_ai);
    reset_ai(ai_scene->ai);
    ais_btn_pause->visible = true;
    ais_started = true;
    ai_scene->text_victory->visible = false;
}


/**
 * Callback method for the pause button.
*/
void ais_pause_callback() 
{
    ais_started = false;
    ais_btn_resume->visible = true;
    ais_btn_pause->visible = false;
}


/**
 * Callback method for the resume button.
*/
void ais_resume_callback() 
{
    ais_started = true;
    ais_btn_resume->visible = false;
    ais_btn_pause->visible = true;
}


/**
 * Callback method for the back button.
*/
void ais_back_callback() 
{
    ais_started = false;
    reset_board(ai_scene->board_player);
    reset_board(ai_scene->board_ai);
    reset_ai(ai_scene->ai);
    ais_btn_start->visible = true;
    ais_btn_pause->visible = false;
    ais_btn_resume->visible = false;
    ais_btn_restart->visible = false;
    ai_scene->text_victory->visible = false;
    ai_scene->return_code = T_MENU;
    ais_show_game_ui(false);
    ais_difficulty_choosed = false;
}


/**
 * Callback method called when the board player is full.
*/
void ais_full_player_callback() 
{
    ais_btn_restart->visible = true;
    ais_btn_pause->visible = false;
    ais_started = false;
    ai_scene->text_victory->text = (char*)txt_victory_ai;
    ai_scene->text_victory->visible = true;
}


/**
 * Callback method called when the board ai is full.
*/
void ais_full_ai_callback() 
{
    ais_btn_restart->visible = true;
    ais_btn_pause->visible = false;
    ais_started = false;
    ai_scene->text_victory->text = (char*)txt_victory_player;
    ai_scene->text_victory->visible = true;
}


/**
 * Callback method for the easy button
*/
void ais_easy_callback() 
{
    set_ai_difficulty(ai_scene->ai, EASY);
    ais_difficulty_choosed = true;
    ais_show_game_ui(true);
}


/**
 * Callback method for the medium button
*/
void ais_medium_callback() 
{
    set_ai_difficulty(ai_scene->ai, MEDIUM);
    ais_difficulty_choosed = true;
    ais_show_game_ui(true);
}


/**
 * Callback method for the hard button
*/
void ais_hard_callback() 
{
    set_ai_difficulty(ai_scene->ai, HARD);
    ais_difficulty_choosed = true;
    ais_show_game_ui(true);
}


/**
 * Callback method for the extreme button
*/
void ais_extreme_callback()
{
    set_ai_difficulty(ai_scene->ai, EXTREME);
    ais_difficulty_choosed = true;
    ais_show_game_ui(true);
}
