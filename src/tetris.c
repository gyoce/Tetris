#include "tetris.h"

#include "print_color.h"
#include "global.h"

#include <time.h> 
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/* Tetris global structure */
Tetris* tetris = NULL;


/* Tetris functions declaration */
void run();
bool init();
void clean();

bool init_ui();
bool init_sprites();
bool init_sprite_board();
bool init_sprite_font();
bool init_scenes();
bool init_menu_scene();
bool init_solo_scene();
bool init_ai_scene();
bool init_icon();


/**
 * Create the tetris game. 
 * Initialize all the variables in the struct.
 * 
 * \return a new pointer of the game.
*/
Tetris* create_game() 
{
    Tetris* _tetris = malloc(sizeof(Tetris));
    if (_tetris == NULL)
    {
        print_color("Error creating tetris", TXT_RED);
        return NULL;
    }
    _tetris->width = WIDTH;
    _tetris->height = HEIGHT;
    _tetris->window = NULL;
    _tetris->renderer = NULL;
    _tetris->run = &run;
    _tetris->menu_scene = NULL;
    _tetris->solo_scene = NULL;
    tetris = _tetris;
    return _tetris;
}


/**
 * Start the game of the tetris. 
 * First initialize the SDL Context and create the window
 * before calling the main loop.
*/
void run() 
{
    // Init
    if(!init()) 
    {
        clean();
        return;
    }

    // Main loop
    uint8_t code = T_MENU;
    while(code != T_EXIT) 
    {
        switch(code) 
        {
            case T_SP:
                code = tetris->solo_scene->ss_loop();
                tetris->solo_scene->return_code = 0;
                break;
            case T_MENU:
                code = tetris->menu_scene->ms_loop();
                tetris->menu_scene->return_code = 0;
                break;
            case T_AI:
                code = tetris->ai_scene->ais_loop();
                tetris->ai_scene->return_code = 0;
                break;
        }
    }

    // Clean
    clean();
}


/**
 * Initialize the SDL Context, create the window, the renderer, load the sprite
 * and initialize the scenes.
 * 
 * \return false if there were problems.
*/
bool init() 
{
    print_color("Init", TXT_BLUE);

    srand(time(NULL));
    //srand(3);

    if (!init_ui())
        return false;
    
    if (!init_sprites())
        return false;

    if (!init_scenes())
        return false;

    if (!init_icon())
        return false;

    print_color("End Init", TXT_BLUE);

    return true;
}


/** 
 * Clean the game, destroy the textures, the renderer and the window.
 * Also call SDL_Quit and free the tetris global variable.
*/
void clean() 
{
    print_color("Cleaning the game ", TXT_BLUE);
    if(tetris->ai_scene != NULL) tetris->ai_scene->ais_clean();
    if(tetris->solo_scene != NULL) tetris->solo_scene->ss_clean();
    if(tetris->menu_scene != NULL) tetris->menu_scene->ms_clean();
    free_textures();
    if(tetris->renderer != NULL) SDL_DestroyRenderer(tetris->renderer);
    if(tetris->window != NULL) SDL_DestroyWindow(tetris->window);
    SDL_Quit();
    free(tetris);
    tetris = NULL;
    print_color("Cleaning the game finished", TXT_BLUE);
}


/**
 * Init SDL2, the window and the renderer.
 * 
 * \return true if the init did worked, false else.
*/
bool init_ui() 
{
    print_color(" Init UI", TXT_BLUE);

    // Init SDL2
    print_color("  Initializing SDL2", TXT_GREEN);
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        print_color("  Error Initializing SDL2", TXT_RED);
        printf("   %s\n", SDL_GetError());
        return false;
    }

    // Creating window
    print_color("  Creating the window", TXT_GREEN);
    tetris->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        tetris->width, tetris->height, SDL_WINDOW_SHOWN);
    if (tetris->window == NULL)
    {
        print_color("  Error creating the window", TXT_RED);
        printf("   %s\n", SDL_GetError());
        return false;
    }

    // Creating renderer
    print_color("  Creating renderer", TXT_GREEN);
    tetris->renderer = SDL_CreateRenderer(tetris->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (tetris->renderer == NULL)
    {
        print_color("  Error creating renderer", TXT_RED);
        printf("   %s\n", SDL_GetError());
        return false;
    }

    print_color(" End Init UI", TXT_BLUE);
    return true;
}


/**
 * Init the sprites needed in the game.
 * 
 * \return true if the init worked, false else.
 * \see init_sprite_board & init_sprite_font
*/
bool init_sprites() 
{
    print_color(" Init sprites", TXT_BLUE);

    if (!init_sprite_board())
        return false;

    if (!init_sprite_font())
        return false;

    if (!init_cursors())
        return false;

    print_color(" End Init sprites", TXT_BLUE);
    return true;
}


/**
 * Init the sprite board, i.e. the image containing all the sprites
 * of the game.
 * 
 * \return true if the init worked, false else.
 * \see global::init_sprite_textures
*/
bool init_sprite_board() 
{
    // Loading sprite board
    print_color("  Loading sprite board", TXT_GREEN);
    SDL_Surface* surface = SDL_LoadBMP(PATH_BOARD_SPRITE);
    if (surface == NULL)
    {
        print_color("  Error loading sprite board", TXT_RED);
        printf("   %s\n", SDL_GetError());
        return false;
    }

    // Creating texture sprite from surface
    print_color("  Creating texture sprite from surface", TXT_GREEN);
    SDL_Texture* board_sprite = SDL_CreateTextureFromSurface(tetris->renderer, surface);
    if (board_sprite == NULL)
    {
        print_color("  Error Creating texture sprite from surface", TXT_RED);
        printf("   %s\n", SDL_GetError());
        return false;
    }
    SDL_FreeSurface(surface);

    // Initializing sprite board textures
    print_color("  Initializing sprite board textures", TXT_GREEN);
    if (!init_sprite_textures(tetris->renderer, board_sprite))
    {
        print_color("  Error Initializing sprite board textures", TXT_RED);
        return false;
    }

    return true;
}


/**
 * Init the sprite font, i.e. the image containing all the chars
 * of the font of the game.
 *
 * \return true if the init worked, false else.
 * \see global::init_font_textures
*/
bool init_sprite_font() 
{
    // Loading sprite font
    print_color("  Loading sprite font", TXT_GREEN);
    SDL_Surface* surface = SDL_LoadBMP(PATH_FONT_SPRITE);
    if (surface == NULL)
    {
        print_color("  Error loading sprite font", TXT_RED);
        printf("   %s\n", SDL_GetError());
        return false;
    }

    // Creating texture font from surface
    print_color("  Creating texture font from surface", TXT_GREEN);
    SDL_Texture* font_sprite = SDL_CreateTextureFromSurface(tetris->renderer, surface);
    if (font_sprite == NULL)
    {
        print_color("  Error Creating texture font from surface", TXT_RED);
        printf("   %s\n", SDL_GetError());
        return false;
    }
    SDL_FreeSurface(surface);

    // Initializing all the font textures
    print_color("  Initializing sprite font textures", TXT_GREEN);
    if(!init_font_textures(tetris->renderer, font_sprite)) 
    {
        print_color("  Error Initializing sprite font textures", TXT_RED);
        return false;
    }

    return true;
}


/**
 * Init the scenes of the game.
 * 
 * \return true if the init worked, false else.
 * \see init_menu_scene & init_solo_scene
*/
bool init_scenes() 
{
    print_color(" Init scenes", TXT_BLUE);

    if (!init_menu_scene())
        return false;

    if (!init_solo_scene())
        return false;

    if (!init_ai_scene())
        return false;
    
    print_color(" End Init scenes", TXT_BLUE);
    return true;
}


/**
 * Init the menu scene.
 * 
 * \return true if the init worked, false else.
 * \see menu_scene::create_menu_scene & menu_scene::ms_init.
*/
bool init_menu_scene() 
{
    print_color("  Creating menu scene", TXT_GREEN);
    tetris->menu_scene = create_menu_scene(tetris->window);
    if (tetris->menu_scene == NULL)
    {
        print_color("  Error creating menu scene", TXT_RED);
        return false;
    }

    print_color("  Initializing menu scene", TXT_GREEN);
    if (!tetris->menu_scene->ms_init())
    {
        print_color("  Error initializing menu scene", TXT_RED);
        return false;
    }

    return true;
}


/**
 * Init the solo scene.
 *
 * \return true if the init worked, false else.
 * \see solo_scene::create_solo_scene & solo_scene::ss_init.
*/
bool init_solo_scene() 
{
    print_color("  Creating solo scene", TXT_GREEN);
    tetris->solo_scene = create_solo_scene(tetris->window);
    if (tetris->solo_scene == NULL)
    {
        print_color("  Error creating solo scene", TXT_RED);
        return false;
    }

    print_color("  Initializing solo scene", TXT_GREEN);
    if (!tetris->solo_scene->ss_init())
    {
        print_color("  Error initializing solo scene", TXT_RED);
        return false;
    }

    return true;
}


/**
 * Init the ai scene.
 *
 * \return true if the init worked, false else.
 * \see ai_scene::create_ai_scene & ai_scene::ias_init.
*/
bool init_ai_scene() 
{
    print_color("  Creating ai scene", TXT_GREEN);
    tetris->ai_scene = create_ai_scene(tetris->window);
    if (tetris->ai_scene == NULL)
    {
        print_color("  Error creating ai scene", TXT_RED);
        return false;
    }

    print_color("  Initializing ai scene", TXT_GREEN);
    if (!tetris->ai_scene->ais_init())
    {
        print_color("  Error initializing ai scene", TXT_RED);
        return false;
    }

    return true;
}


/**
 * Init the icon of the game.
 * 
 * \return true if the init worked, false else.
*/
bool init_icon() 
{
    print_color("  Initializing icon", TXT_GREEN);

    SDL_Surface* surface = SDL_LoadBMP(PATH_ICON_SPRITE);
    if (surface == NULL)
    {
        print_color("  Error loading sprite icon", TXT_RED);
        printf("   %s\n", SDL_GetError());
        return false;
    }
    SDL_SetWindowIcon(tetris->window, surface);
    SDL_FreeSurface(surface);

    print_color("  End Initializing icon", TXT_GREEN);
    return true;
}
