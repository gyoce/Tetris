#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdint.h>

typedef struct Button {
    SDL_Texture* normal_texture;
    SDL_Texture* hovered_texture;
    SDL_Rect pos;
    bool interactable;
    bool visible;
    bool hovered;
    void (*callback)();
} Button;

Button* create_button(SDL_Texture* texture, SDL_Rect pos, void (*callback));
bool check_mouse_on_button(const Button* button, const uint16_t x, const uint16_t y);

void draw_button(Button* button, SDL_Renderer* renderer);
void clean_button(Button** button);

void check_buttons_click(Button** buttons, const uint8_t nb_buttons, const SDL_MouseButtonEvent event);
void check_buttons_hover(Button** buttons, const uint8_t nb_buttons, const SDL_MouseMotionEvent event);

#endif // BUTTON_H
