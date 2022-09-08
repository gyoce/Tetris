#ifndef AI_H
#define AI_H

#include "board.h"

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>

#define TIME_EASY 600
#define TIME_MEDIUM 400
#define TIME_HARD 200
#define TIME_EXTREME 100

typedef enum AIDifficulty {
    EASY,
    MEDIUM,
    HARD,
    EXTREME,
} AIDifficulty;

typedef enum AIAction {
    NONE,
    HARD_DROP,
    ROTATE,
    MOVE_RIGHT,
    MOVE_LEFT
} AIAction;

typedef struct AI {
	Board* ai_board;            // The board associated with the AI
    bool path_found;            // If there is a path found for the tetromino
    AIAction* actions;          // The array of actions
    uint8_t actual_index;       // The actual index for the actions
    uint8_t size_actions;       // The size of the actions
    double time_update;         // The counter time for update
    uint16_t time_to_update;    // The number of ms before updating
    AIDifficulty ai_difficulty; // The AI difficulty
} AI;

AI* create_ai(Board* board);
void clean_ai(AI** ai);
void reset_ai(AI* ai);
void set_ai_difficulty(AI* ai, AIDifficulty difficulty);
void ai_process(AI* ai, double delta_t);

#endif