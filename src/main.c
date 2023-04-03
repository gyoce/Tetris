#include "tetris.h"

//int main(int argc, char **argv) 
int main()
{
    Tetris* tetris = create_game();
    if (!tetris)
        return 1;

    tetris->run();

    return 0;
}
