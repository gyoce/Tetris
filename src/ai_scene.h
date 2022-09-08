#ifndef AI_SCENE_H
#define AI_SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include <stdint.h>
#include <stdbool.h>

#include "button.h"
#include "board.h"
#include "text.h"
#include "ai.h"

#define AIS_NB_BUTTONS 9
#define AIS_NB_KEYS 5

typedef struct AIScene {
    Button** buttons;
    uint8_t nb_buttons;
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool (*ais_init)();
    uint8_t (*ais_loop)();
    void (*ais_clean)();
    uint8_t return_code;
    Board* board_player;
    Board* board_ai;
    AI* ai;
    Text* text_ai;
    Text* text_victory;
    Text* text_choose_difficulty;
    KeyText** key_texts;
    uint8_t nb_keys;
} AIScene;

AIScene* create_ai_scene(SDL_Window* window);

#endif // AI_SCENE_H
