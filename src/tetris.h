#ifndef TETRIS_H
#define TETRIS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_render.h>

#include <stdint.h>

#include "menu_scene.h"
#include "solo_scene.h"
#include "ai_scene.h"

#define T_EXIT 255   // Exit the game
#define T_SP 1       // Go to singleplayer
#define T_AI 2       // Go to ai
#define T_OPTIONS 3  // Go to options
#define T_MENU 4     // Go to menu

#define WIDTH 1280
#define HEIGHT 720
#define TITLE "Tetris"

typedef struct Tetris {
    uint16_t width;
    uint16_t height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    MenuScene* menu_scene;
    SoloScene* solo_scene;
    AIScene* ai_scene;
    void (*run)();
} Tetris;

Tetris* create_game();

#endif // TETRIS_H
