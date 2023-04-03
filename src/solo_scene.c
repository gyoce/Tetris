#include "solo_scene.h"

#include <stdio.h>

#include "print_color.h"
#include "utils.h"
#include "board.h"
#include "global.h"
#include "tetromino.h"
#include "tetris.h"


/* SoloScene global structure */
SoloScene* solo_scene = NULL;

/* SoloScene variables */
double ss_delta_t;
uint64_t ss_start, ss_end;
bool ss_started = false;
const SDL_Rect board_pos = {100, 4, 388, 708};

/* Buttons */
Button* ss_btn_start;
Button* ss_btn_restart;
Button* ss_btn_pause;
Button* ss_btn_resume;
Button* ss_btn_back;

/* SoloScene functions declaration */
bool ss_init();
uint8_t ss_loop();
void ss_clean();

void ss_event();
void ss_keyboard_event(SDL_KeyboardEvent event);
void ss_update();
void ss_draw();

bool ss_create_buttons();
bool ss_create_texts();

/* SoloScene callbacks */
void ss_start_callback();
void ss_restart_callback();
void ss_pause_callback();
void ss_resume_callback();
void ss_full_callback();
void ss_back_callback();


/* #################### PUBLIC #################### */

/**
 * Create the solo scene.
 * 
 * \param window the main window.
 * \param board_sprite the image with all textures.
*/
SoloScene* create_solo_scene(SDL_Window* window) 
{
    SoloScene* _solo_scene = malloc(sizeof(SoloScene));
    if (_solo_scene == NULL)
        return NULL;
    _solo_scene->buttons = malloc(SS_NB_BUTTONS * sizeof(Button));
    if (_solo_scene->buttons == NULL)
    {
        free(_solo_scene);
        return NULL;
    }
    _solo_scene->key_texts = malloc(sizeof(KeyText*) * SS_NB_KEYS);
    if (_solo_scene->key_texts == NULL)
    {
        free(_solo_scene->buttons);
        free(_solo_scene);
        return NULL;
    }
    _solo_scene->window = window;
    _solo_scene->renderer = SDL_GetRenderer(window);
    _solo_scene->ss_init = &ss_init;
    _solo_scene->ss_loop = &ss_loop;
    _solo_scene->ss_clean = &ss_clean;
    _solo_scene->board = NULL;
    _solo_scene->nb_buttons = 0;
    _solo_scene->nb_keys = 0;
    _solo_scene->return_code = 0;
    _solo_scene->nb_buttons = 0;
    solo_scene = _solo_scene;
    return _solo_scene;
}


/* #################### SOLO_SCENE METHODS #################### */

/**
 * Initializing the solo scene.
 * 
 * \return true if the init worked, false else.
*/
bool ss_init() 
{
    print_color("   Solo Scene init", TXT_GREEN);

    print_color("   Creating board for solo scene", TXT_GREEN);
    solo_scene->board = create_board(board_pos, true, &ss_full_callback);
    if(solo_scene->board == NULL) 
    {
        print_color("   Error Creating board for solo scene", TXT_RED);
        return false;
    }
    
    print_color("   Creating buttons for solo scene", TXT_GREEN);
    if(!ss_create_buttons()) 
    {
        print_color("   Error Creating buttons for solo scene", TXT_RED);
        return false;
    }

    print_color("   Creating texts for solo scene", TXT_GREEN);
    if(!ss_create_texts())
    {
        print_color("   Error Creating texts for solo scene", TXT_RED);
        return false;
    }

    print_color("   End Solo Scene init", TXT_GREEN);

    return true;
}


/**
 * Create the solo scene buttons.
 * 
 * \return true if the creation works, false else.
*/
bool ss_create_buttons()
{
    // Start button
    SDL_Rect pos = { 750, 100, 224, 44 };
    Button* btn_start = create_button(g_btn_n_start, pos, &ss_start_callback);
    btn_start->hovered_texture = g_btn_h_start;
    if (btn_start == NULL)
        return false;
    solo_scene->buttons[solo_scene->nb_buttons] = btn_start;
    solo_scene->nb_buttons++;
    ss_btn_start = btn_start;

    // Restart button
    Button* btn_restart = create_button(g_btn_n_restart, pos, &ss_restart_callback);
    btn_restart->hovered_texture = g_btn_h_restart;
    if (btn_restart == NULL)
        return false;
    btn_restart->visible = false;
    solo_scene->buttons[solo_scene->nb_buttons] = btn_restart;
    solo_scene->nb_buttons++;
    ss_btn_restart = btn_restart;

    // Pause button
    SDL_Rect pos2 = { 750, 300, 224, 44 };
    Button* btn_pause = create_button(g_btn_n_pause, pos2, &ss_pause_callback);
    btn_pause->hovered_texture = g_btn_h_pause;
    if (btn_pause == NULL)
        return false;
    btn_pause->visible = false;
    solo_scene->buttons[solo_scene->nb_buttons] = btn_pause;
    solo_scene->nb_buttons++;
    ss_btn_pause = btn_pause;

    // Resume button
    Button* btn_resume = create_button(g_btn_n_resume, pos2, &ss_resume_callback);
    btn_resume->hovered_texture = g_btn_h_resume;
    if (btn_resume == NULL)
        return false;
    btn_resume->visible = false;
    solo_scene->buttons[solo_scene->nb_buttons] = btn_resume;
    solo_scene->nb_buttons++;
    ss_btn_resume = btn_resume;

    // Back button
    SDL_Rect pos3 = { 750, 500, 224, 44 };
    Button* btn_back = create_button(g_btn_n_back, pos3, &ss_back_callback);
    btn_back->hovered_texture = g_btn_h_back;
    if (btn_back == NULL)
        return false;
    solo_scene->buttons[solo_scene->nb_buttons] = btn_back;
    solo_scene->nb_buttons++;
    ss_btn_back = btn_back;

    return true;
}


/**
 * Create the solo scene texts.
 *
 * \return true if the creation works, false else.
*/
bool ss_create_texts() 
{
    const uint8_t key_size = 64;
    const uint8_t key_text_size = 24;

    // Key Space
    SDL_Rect pos1 = { 1030, 100, key_size, key_size };
    KeyText* key_text_space = create_key_text(g_key_space, "DROP", pos1, key_text_size);
    if (key_text_space == NULL)
        return false;
    solo_scene->key_texts[solo_scene->nb_keys] = key_text_space;
    solo_scene->nb_keys++;

    // Key Q
    SDL_Rect pos2 = { 1030, 175, key_size, key_size };
    KeyText* key_text_q = create_key_text(g_key_q, "LEFT", pos2, key_text_size);
    if (key_text_q == NULL)
        return false;
    solo_scene->key_texts[solo_scene->nb_keys] = key_text_q;
    solo_scene->nb_keys++;

    // Key S
    SDL_Rect pos3 = { 1030, 250, key_size, key_size };
    KeyText* key_text_s = create_key_text(g_key_s, "DOWN", pos3, key_text_size);
    if (key_text_s == NULL)
        return false;
    solo_scene->key_texts[solo_scene->nb_keys] = key_text_s;
    solo_scene->nb_keys++;

    // Key D
    SDL_Rect pos4 = { 1030, 325, key_size, key_size };
    KeyText* key_text_d = create_key_text(g_key_d, "RIGHT", pos4, key_text_size);
    if (key_text_d == NULL)
        return false;
    solo_scene->key_texts[solo_scene->nb_keys] = key_text_d;
    solo_scene->nb_keys++;

    // Key R
    SDL_Rect pos5 = { 1030, 400, key_size, key_size };
    KeyText* key_text_r = create_key_text(g_key_r, "ROTATE", pos5, key_text_size);
    if (key_text_r == NULL)
        return false;
    solo_scene->key_texts[solo_scene->nb_keys] = key_text_r;
    solo_scene->nb_keys++;

    return true;
}


/**
 * Main loop for the solo scene.
 * 
 * \return [SS_MENU, T_EXIT]
*/
uint8_t ss_loop()
{
    print_color("Solo Scene loop", TXT_BLUE);
    while(!solo_scene->return_code) 
    {
        ss_start = SDL_GetPerformanceCounter();

        ss_event();
        ss_update();
        ss_draw();

        ss_end = SDL_GetPerformanceCounter();
		
	    ss_delta_t = (double)((ss_end - ss_start) * 1000 / (double)SDL_GetPerformanceFrequency());
    }
    print_color("End Solo Scene loop", TXT_BLUE);
    return solo_scene->return_code;
}


/**
 * Clean the solo scene.
*/
void ss_clean()
{
    print_color("  Solo Scene clean", TXT_GREEN);

    print_color("    Cleaning board", TXT_GREEN);
    if (solo_scene->board != NULL) clean_board(&solo_scene->board);

    print_color("    Cleaning buttons", TXT_GREEN);
    for (uint8_t i = 0; i < solo_scene->nb_buttons; i++)
        clean_button(&solo_scene->buttons[i]);
    free(solo_scene->buttons);

    print_color("    Cleaning text", TXT_GREEN);
    for (uint8_t i = 0; i < solo_scene->nb_keys; i++)
        clean_key_text(&solo_scene->key_texts[i]);
    free(solo_scene->key_texts);

    free(solo_scene);
    solo_scene = NULL;

    print_color("  Solo Scene clean finished", TXT_GREEN);
}


/* #################### SOLO_SCENE LOOP #################### */

/**
 * Handle all events in the solo scene.
*/
void ss_event()
{
    SDL_Event event;
    while (!solo_scene->return_code && SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                solo_scene->return_code = T_EXIT;
                break;
            case SDL_MOUSEBUTTONDOWN:
                check_buttons_click(solo_scene->buttons, solo_scene->nb_buttons, event.button);
                break;
            case SDL_MOUSEMOTION:
                check_buttons_hover(solo_scene->buttons, solo_scene->nb_buttons, event.motion);
                break;
            case SDL_KEYDOWN:
                ss_keyboard_event(event.key);
                break;
            default: 
                break;
        }
    }
}


/**
 * Handle all keyboards events in the solo scene.
*/
void ss_keyboard_event(SDL_KeyboardEvent event)
{
    if(ss_started) 
    {
        SDL_Keycode key = event.keysym.sym;
        keyboard_event(solo_scene->board, key);
    }
}


/**
 * Update all the objects for the solo scene.
*/
void ss_update() 
{
    if(ss_started) 
        update_board(solo_scene->board, ss_delta_t);
}


/**
 * Draw in the window the objects from the solo scene.
*/
void ss_draw() 
{
    SDL_UpdateWindowSurface(solo_scene->window);
    SDL_RenderClear(solo_scene->renderer);

    // Draw the board
    draw_board(solo_scene->board, solo_scene->renderer);

    // Draw all the buttons
    for(uint8_t i = 0; i < solo_scene->nb_buttons; i++)
        draw_button(solo_scene->buttons[i], solo_scene->renderer);

    // Draw keys
    for (uint8_t i = 0; i < solo_scene->nb_keys; i++)
        draw_key_text(solo_scene->key_texts[i], solo_scene->renderer);

    SDL_RenderPresent(solo_scene->renderer);
}


/* #################### CALLBACK METHODS #################### */

/**
 * Callback method called when the board is full.
*/
void ss_full_callback() 
{
    ss_btn_restart->visible = true;
    ss_btn_pause->visible = false;
    ss_started = false;
}


/**
 * Callback method for the start button.
*/
void ss_start_callback()
{
    ss_started = true;
    ss_btn_start->visible = false;
    ss_btn_pause->visible = true;
}


/**
 * Callback method for the restart button.
*/
void ss_restart_callback() 
{
    ss_btn_restart->visible = false;
    reset_board(solo_scene->board);
    ss_btn_pause->visible = true;
    ss_started = true;
}


/**
 * Callback method for the pause button.
*/
void ss_pause_callback()
{
    ss_btn_pause->visible = false;
    ss_btn_resume->visible = true;
    ss_started = false;
}


/**
 * Callback method for the resume button.
*/
void ss_resume_callback()
{
    ss_btn_pause->visible = true;
    ss_btn_resume->visible = false;
    ss_started = true;
}


/**
 * Callback method for the back button.
*/
void ss_back_callback() 
{
    ss_started = false;
    reset_board(solo_scene->board);
    ss_btn_start->visible = true;
    ss_btn_pause->visible = false;
    ss_btn_resume->visible = false;
    ss_btn_restart->visible = false;
    solo_scene->return_code = T_MENU;
}
