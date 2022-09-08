#include "menu_scene.h"

#include "tetris.h"
#include "print_color.h"
#include "global.h"


/* MenuScene global structure */
MenuScene* menu_scene = NULL;


/* MenuScene variables */
const uint16_t btn_width = 500;
const uint16_t btn_height = 75;
const uint16_t btn_space = 100;
uint16_t btn_x_start = 0;
uint16_t btn_y_start = 0;


/* MenuScene functions declaration */
bool ms_init();
uint8_t ms_loop();
void ms_event();
void ms_update();
void ms_draw();
void ms_clean();

bool ms_create_buttons();
SDL_Rect ms_get_optimal_pos(const uint8_t i);


/* Buttons callbacks */
void ms_sp_callback() { menu_scene->return_code = T_SP; }
void ms_ia_callback() { menu_scene->return_code = T_AI; }
void ms_opts_callback() { menu_scene->return_code = T_OPTIONS; }
void ms_exit_callback() { menu_scene->return_code = T_EXIT; }


/**
 * Create the menu scene of the game.
*/
MenuScene* create_menu_scene(SDL_Window* window) 
{
    MenuScene* _menu_scene = malloc(sizeof(MenuScene));
    if (_menu_scene == NULL)
        return NULL;
    _menu_scene->buttons = malloc(MS_NB_BUTTONS * sizeof(Button*));
    if(_menu_scene->buttons == NULL)
    {
        free(_menu_scene);
        return NULL;
    }
    _menu_scene->return_code = 0;
    _menu_scene->window = window;
    _menu_scene->renderer = SDL_GetRenderer(window);
    _menu_scene->ms_init = &ms_init;
    _menu_scene->ms_loop = &ms_loop;
    _menu_scene->ms_clean = &ms_clean;
    _menu_scene->nb_buttons = 0;
    menu_scene = _menu_scene;
    return _menu_scene;
}


/**
 * Initializing the menu scene.
 * 
 * \return false if there were problems 
*/
bool ms_init() 
{
    print_color("   Menu Scene init", TXT_GREEN);

    print_color("    Creating menu scene buttons", TXT_GREEN);
    if (!ms_create_buttons())
    {
        print_color("    Error Creating menu scene buttons", TXT_RED);
        return false;
    }

    print_color("   End Menu Scene init", TXT_GREEN);

    return true;
}


/**
 * Main loop for the menu scene.
 * 
 * \return one of [MS_SP, MS_IA, MS_OPTIONS].
*/
uint8_t ms_loop() 
{
    print_color("Menu Scene loop", TXT_BLUE);
    while(!menu_scene->return_code)
    {
        ms_event();
        ms_update();
        ms_draw();
    }
    print_color("End Menu Scene loop", TXT_BLUE);
    return menu_scene->return_code;
}


/**
 * Handle all events in the menu scene.
*/
void ms_event() 
{
    SDL_Event event;
    while (!menu_scene->return_code && SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                menu_scene->return_code = T_EXIT;
                break;
            case SDL_MOUSEBUTTONDOWN:
                check_buttons_click(menu_scene->buttons, menu_scene->nb_buttons, event.button);
                break;
            case SDL_MOUSEMOTION:
                check_buttons_hover(menu_scene->buttons, menu_scene->nb_buttons, event.motion);
                break;
            default: 
                break;
        }
    }
}


/**
 * Update all the objects for the menu scene.
*/
void ms_update() 
{

}


/**
 * Draw in the window the objects from the menu scene.
*/
void ms_draw() 
{
    SDL_UpdateWindowSurface(menu_scene->window);
    SDL_RenderClear(menu_scene->renderer);

    // Render all buttons 
    for (uint8_t i = 0; i < menu_scene->nb_buttons; i++)
        draw_button(menu_scene->buttons[i], menu_scene->renderer);

    SDL_RenderPresent(menu_scene->renderer);
}


/**
 * Clean the menu scene.
*/
void ms_clean() 
{
    print_color("  Menu Scene clean", TXT_GREEN);

    print_color("    Cleaning buttons", TXT_GREEN);
    for(uint8_t i = 0; i < menu_scene->nb_buttons; i++) 
        clean_button(&menu_scene->buttons[i]);
    free(menu_scene->buttons);

    free(menu_scene);
    menu_scene = NULL;

    print_color("  Menu Scene clean finished", TXT_GREEN);
}


/**
 * Create all the menu scene buttons.
*/
bool ms_create_buttons() 
{
    // Singleplayer button
    Button* btn_sp = create_button(g_btn_n_sp, ms_get_optimal_pos(menu_scene->nb_buttons), &ms_sp_callback);
    btn_sp->hovered_texture = g_btn_h_sp;
    if (btn_sp == NULL)
        return false;
    menu_scene->buttons[menu_scene->nb_buttons] = btn_sp;
    menu_scene->nb_buttons++;

    // Versus IA button
    Button* btn_ia = create_button(g_btn_n_ia, ms_get_optimal_pos(menu_scene->nb_buttons), &ms_ia_callback);
    btn_ia->hovered_texture = g_btn_h_ia;
    if (btn_ia == NULL)
        return false;
    menu_scene->buttons[menu_scene->nb_buttons] = btn_ia;
    menu_scene->nb_buttons++;

    // Options button
    Button* btn_options = create_button(g_btn_n_opts, ms_get_optimal_pos(menu_scene->nb_buttons), &ms_opts_callback);
    btn_options->hovered_texture = g_btn_h_opts;
    if (btn_options == NULL)
        return false;
    btn_options->visible = false;
    menu_scene->buttons[menu_scene->nb_buttons] = btn_options;
    menu_scene->nb_buttons++;

    // Exit button
    Button* btn_exit = create_button(g_btn_n_exit, ms_get_optimal_pos(menu_scene->nb_buttons), &ms_exit_callback);
    btn_exit->hovered_texture = g_btn_h_exit;
    if (btn_exit == NULL)
        return false;
    menu_scene->buttons[menu_scene->nb_buttons] = btn_exit;
    menu_scene->nb_buttons++;

    return true;
}


/**
 * Calculate the optimal position for the i button.
 * 
 * \param i the index of the button.
 * \return rect containing (x, y, width, height) of the button.
*/
SDL_Rect ms_get_optimal_pos(const uint8_t i) 
{
    if(btn_x_start == 0)
        btn_x_start = (WIDTH / 2) - (btn_width / 2);

    if(btn_y_start == 0) 
    {
        int total = (MS_NB_BUTTONS * btn_height) + (MS_NB_BUTTONS - 1) * btn_space;
        btn_y_start = (HEIGHT / 2) - (total / 2);
    }

    uint16_t y =  btn_y_start + ((btn_height + btn_space) * i);
    SDL_Rect rect = {btn_x_start, y, btn_width, btn_height};
    return rect;
}
