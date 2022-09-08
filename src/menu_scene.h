#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#define MS_NB_BUTTONS 4

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include <stdint.h>
#include <stdbool.h>

#include "button.h"

typedef struct MenuScene {
    Button** buttons;
    uint8_t nb_buttons;
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool (*ms_init)();
    uint8_t (*ms_loop)();
    void (*ms_clean)();
    uint8_t return_code;
} MenuScene;

MenuScene* create_menu_scene(SDL_Window* window);

#endif // MENU_SCENE_H
