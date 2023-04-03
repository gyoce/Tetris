#ifndef SOLO_SCENE_H
#define SOLO_SCENE_H

#define SS_NB_BUTTONS 5
#define SS_NB_KEYS 5

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include <stdint.h>
#include <stdbool.h>

#include "button.h"
#include "board.h"
#include "text.h"

typedef struct SoloScene {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Board* board;
    Button** buttons;
    uint8_t nb_buttons;
    bool (*ss_init)();
    uint8_t (*ss_loop)();
    void (*ss_clean)();
    uint8_t return_code;
    KeyText** key_texts;
    uint8_t nb_keys;
} SoloScene;

SoloScene* create_solo_scene(SDL_Window* window);

#endif //SOLO_SCENE_H
