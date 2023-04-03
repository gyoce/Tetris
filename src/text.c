#include "text.h"

#include "global.h"

#include <string.h>
#include <stdio.h>


/**
 * Create a new text sprite with a certain size.
 * 
 * \param text the string to be converted to sprite.
 * \param x the x position of the sprite.
 * \param y the y position of the sprite.
 * \param size the size of each char in pixel (width and height).
 * \return a new Text struct.
*/
Text* create_text(char* text, int16_t x, int16_t y, uint8_t size)
{
	Text* text_sprite = malloc(sizeof(Text));
	if (text_sprite == NULL)
		return NULL;
	text_sprite->size = size;
	text_sprite->text = text;
	text_sprite->visible = true;
	text_sprite->correction = TEXT_CORRECTION * size;
	size_t str_size = strlen(text);
	SDL_Rect position = { 
		x, 
		y, 
		(str_size * size) - (str_size * text_sprite->correction),
		size 
	};
	text_sprite->pos = position;
	return text_sprite;
}


/**
 * Correctly clean a Text object.
*/
void clean_text(Text** text) 
{
	free((*text));
	*text = NULL;
}


/**
 * Draw in the renderer the text object.
 * 
 * \param text the text to be drawed.
 * \param renderer the renderer of the window.
*/
void draw_text(Text* text, SDL_Renderer* renderer) 
{
	if (!text->visible)
		return;

	SDL_Rect position = text->pos;
	position.w = position.h = text->size;

	size_t size = strlen(text->text);
	for(size_t i = 0; i < size; i++)
	{
		if(text->text[i] == '\n')
		{
			position.x = text->pos.x;
			position.y += text->size;
			continue;
		}
		SDL_RenderCopy(renderer, g_chars[(int)text->text[i] - CHAR_OFFSET], NULL, &position);
		position.x += text->size - text->correction;
	}
}


/**
 * Create a new key_text object. A key text object represent a texture of a key
 * with a text associate with it.
 * 
 * \param key_texture the texture of the key.
 * \param text the text associate with the texture.
 * \param pos_texture the position of the texture.
 * \return a new key_text object.
*/
KeyText* create_key_text(SDL_Texture* key_texture, char* text, SDL_Rect pos_texture, uint8_t text_size) 
{
	KeyText* key_text = malloc(sizeof(KeyText));
	if (key_text == NULL)
		return NULL;
	key_text->pos = pos_texture;
	key_text->texture = key_texture;
	key_text->visible = true;
	uint16_t x = key_text->pos.x + key_text->pos.w + SPACE_BETWEEN_KEY_TEXT;
	uint16_t y = key_text->pos.y + (key_text->pos.h / 2) - (text_size / 2);
	key_text->text = create_text(text, x, y, text_size);
	return key_text;
}


/**
 * Correctly clean a key_text object.
 * \param key_text the pointer of the key text.
*/
void clean_key_text(KeyText** key_text) 
{
	clean_text(&((*key_text)->text));
	free((*key_text));
	*key_text = NULL;
}


/**
 * Draw in the renderer the key_text object.
 * 
 * \param key_text the key_text object to be drawed.
 * \param renderer the renderer associate with the window.
*/
void draw_key_text(KeyText* key_text, SDL_Renderer* renderer)
{
	if (!key_text->visible)
		return;

	draw_text(key_text->text, renderer);
	SDL_RenderCopy(renderer, key_text->texture, NULL, &key_text->pos);
}
