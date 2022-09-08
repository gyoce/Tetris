#include "button.h"

#include "global.h"

/**
 * Create a new button which is per default visible and interactable.
 * 
 * \param texture the texture of the button.
 * \param pos the position of the button.
 * \param callback the method called when the button is pressed.
*/
Button* create_button(SDL_Texture* texture, SDL_Rect pos, void (*callback)) 
{
    Button* button = malloc(sizeof(Button));
    if (button == NULL)
        return NULL;
    button->normal_texture = texture;
    button->hovered_texture = NULL;
    button->pos = pos;
    button->callback = callback;
    button->interactable = true;
    button->visible = true;
    button->hovered = false;

    return button;
}


/**
 * Check if the mouse (x, y) is inside a button.
 * 
 * \param x the mouse x coordinate.
 * \param y the mouse y coordinate.
 * \param button the button.
 * \return true if the click is inside, false else.
*/
bool check_mouse_on_button(const Button* button, const uint16_t x, const uint16_t y)
{
    if (!button->interactable || !button->visible)
        return false;

    return 
        x >= button->pos.x && 
        y >= button->pos.y && 
        x <= button->pos.x + button->pos.w && 
        y <= button->pos.y + button->pos.h;
}


/**
 * Draw the button on a certain renderer.
 * 
 * \param button the button.
 * \param renderer the renderer of the window.
*/
void draw_button(Button* button, SDL_Renderer* renderer) 
{
    if (!button->visible)
        return;

    if (button->hovered && button->hovered_texture != NULL)
        SDL_RenderCopy(renderer, button->hovered_texture, NULL, &button->pos);
    else
        SDL_RenderCopy(renderer, button->normal_texture, NULL, &button->pos);
}


/**
 * Properly clean the button.
 * 
 * \param button the button.
*/
void clean_button(Button** button) 
{
    free((*button));
    *button = NULL;
}


/**
 * Check for every buttons if there is one that is clicked.
 *
 * \param buttons all the buttons to be checked.
 * \param nb_buttons the number of buttons.
 * \param event the event of the mouse.
 * \see check_click_on_button(button, int, int)
*/
void check_buttons_click(Button** buttons, const uint8_t nb_buttons, const SDL_MouseButtonEvent event)
{
    // Only left mouse click
    if (event.button != SDL_BUTTON_LEFT)
        return;

    const uint16_t x = event.x;
    const uint16_t y = event.y;
    for (uint8_t i = 0; i < nb_buttons; i++)
    {
        const Button* btn = buttons[i];

        if (check_mouse_on_button(btn, x, y))
        {
            btn->callback();
            break;
        }
    }
}


/**
 * Check for every buttons if there is one that is hovered.
 *
 * \param buttons all the buttons to be checked.
 * \param nb_buttons the number of buttons.
 * \see check_hover_on_button(button, int, int)
*/
void check_buttons_hover(Button** buttons, const uint8_t nb_buttons, const SDL_MouseMotionEvent event)
{
    const uint16_t x = event.x;
    const uint16_t y = event.y;
    bool has_changed = false;
    for (uint8_t i = 0; i < nb_buttons; i++)
    {
        Button* btn = buttons[i];

        if (check_mouse_on_button(btn, x, y))
        {
            btn->hovered = true;
            SDL_SetCursor(g_cursor_hovered);
            has_changed = true;
            break;
        }
        else 
        {
            btn->hovered = false;
        }
    }

    if (!has_changed)
        SDL_SetCursor(g_cursor_normal);
}