#ifndef PRINT_COLOR_H
#define PRINT_COLOR_H

typedef enum TextColor {
    TXT_RED = 31,
    TXT_GREEN = 32,
    TXT_BLUE = 34,
    TXT_WHITE = 37
} TextColor;

void print_color(const char* message, const TextColor color);

#endif // PRINT_COLOR_H
