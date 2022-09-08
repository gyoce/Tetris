#include "print_color.h"

#include <stdio.h>

void print_color(const char* message, const enum TextColor color) 
{
    printf("\033[%d;1m%s \033[0m\n", color, message);
}
