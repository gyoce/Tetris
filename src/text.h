#ifndef TEXT_H
#define TEXT_H

#define TEXT_CORRECTION 0.3f // More is less space between letters
#define SPACE_BETWEEN_KEY_TEXT 10

#include <SDL2/SDL.h>

#include <stdint.h>
#include <stdbool.h>

typedef struct Text {
	SDL_Rect pos;
	char* text;
	uint8_t size;
	uint8_t correction;
	bool visible;
} Text;

typedef struct KeyText {
	SDL_Rect pos;
	SDL_Texture* texture;
	Text* text;
	bool visible;
} KeyText;

Text* create_text(char* text, int16_t x, int16_t y, uint8_t size);
void clean_text(Text** text);
void draw_text(Text* text, SDL_Renderer* renderer);

KeyText* create_key_text(SDL_Texture* key_texture, char* text, SDL_Rect pos, uint8_t text_size);
void clean_key_text(KeyText** key_text);
void draw_key_text(KeyText* key_text, SDL_Renderer* renderer);

#endif
