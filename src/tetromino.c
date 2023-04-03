#include "tetromino.h"

#include "global.h"
#include "utils.h"
#include "print_color.h"


/* Local functions declaration */
Square** create_squares(BlockType block_type);
Square* create_square(int8_t x, int8_t y);

void fill_moves_for_I(uint8_t rotation, int8_t** moves);
void fill_moves_for_J(uint8_t rotation, int8_t** moves);
void fill_moves_for_L(uint8_t rotation, int8_t** moves);
//void fill_moves_for_O(uint8_t rotation, int8_t** moves);
void fill_moves_for_S(uint8_t rotation, int8_t** moves);
void fill_moves_for_T(uint8_t rotation, int8_t** moves);
void fill_moves_for_Z(uint8_t rotation, int8_t** moves);


/**
 * Create a tetromino with a certain block type and a color.
 * 
 * \param block type the block type of the tetromino.
 * \param color the color of the tetromino.
 * \return a new tetromino.
*/
Tetromino* create_tetromino(BlockType block_type) 
{
    Tetromino* tetromino = malloc(sizeof(Tetromino));
    if (tetromino == NULL)
        return NULL;
    tetromino->block_type = block_type;
    tetromino->color = get_color_for_block_type(block_type);
    tetromino->squares = create_squares(block_type);
    tetromino->nb_squares = NB_SQUARES;
    tetromino->blocked = false;
    tetromino->rotation = 0;
    return tetromino;
}


/**
 * Properly clean a tetromino.
 * 
 * \param tetromino the tetromino to be cleaned.
*/
void clean_tetromino(Tetromino** tetromino) 
{
    for(uint8_t i = 0; i < (*tetromino)->nb_squares; i++)
        free((*tetromino)->squares[i]);
    free((*tetromino)->squares);

    free((*tetromino));
    *tetromino = NULL;
}


/**
 * Create a new random tetromino with a random block type and a random color.
 *
 * \return the new random tetromino.
*/
Tetromino* get_random_tetromino()
{
    uint8_t block_type = (rand() % (B_MAX - 1)) + 1;
    return create_tetromino(block_type);
}


/**
 * Check if a square with [x, y] is inside the tetromino.
 * 
 * \param tetromino the tetromino to be checked.
 * \param x the x coordinate of the square.
 * \param y the y coordinate of the square.
 * \return true if the square is inside the tetromino, false else.
*/
bool square_in_tetromino(const Tetromino* tetromino, const int8_t x, const int8_t y)
{
    for(uint8_t i = 0; i < tetromino->nb_squares; i++)
    {
        Square* square = tetromino->squares[i];
        if(square->x == x && square->y == y)
            return true;
    }
    return false;
}


/**
 * Check if the tetromino is dead, mean that all the squares in the tetromino are dead.
 * 
 * \param tetromino the tetromino to be checked.
 * \return true if all the squares in this tetromino are dead, false else.
*/
bool tetromino_is_dead(const Tetromino* tetromino) 
{
    for(uint8_t i = 0; i < tetromino->nb_squares; i++) 
        if (tetromino->squares[i]->alive)
            return false;
    return true;
}


/**
 * Compare two tetrominos and return if there are the same.
 * 
 * \param tetromino1 the first tetromino to be compared.
 * \param tetromino2 the second tetromino to be compared.
 * \return true if the tetrominos are the same, false else.
*/
bool same_tetrominos(const Tetromino* t1, const Tetromino* t2) 
{
    if ((t1->blocked != t2->blocked) || (t1->rotation != t2->rotation) || (t1->block_type != t2->block_type))
        return false;

    for (uint8_t i = 0; i < t1->nb_squares; i++)
    {
        Square* s1 = t1->squares[i];
        Square* s2 = t2->squares[i];
        if (s1->x != s2->x || s1->y != s2->y)
            return false;
    }

    return true;
}


/**
 * Create a square with a given x and y.
 *
 * \param x the abciss of the square, the colum in the map
 * \param y the ordinate of the square, the line in the map
 * \return a new square which is per default alive.
*/
Square* create_square(int8_t x, int8_t y)
{
    Square* square = malloc(sizeof(Square));
    square->x = x;
    square->y = y;
    square->alive = true;
    return square;
}


/**
 * Create all squares for a tetromino for a certain block type.
 *
 * \param block_type the block type of the tetromino.
 * \return all the squares for a tetromino.
*/
Square** create_squares(BlockType block_type)
{
    Square** squares = malloc(NB_SQUARES * sizeof(Square*));
    switch (block_type)
    {
    case I:
        squares[0] = create_square(3, -2); squares[1] = create_square(4, -2); // [0][1][2][3]
        squares[2] = create_square(5, -2); squares[3] = create_square(6, -2);
        break;
    case J:
        squares[0] = create_square(3, -3); squares[1] = create_square(4, -3); // [0][1][2]
        squares[2] = create_square(5, -3); squares[3] = create_square(5, -2); //       [3]
        break;
    case L:
        squares[0] = create_square(3, -3); squares[1] = create_square(4, -3); // [0][1][2]
        squares[2] = create_square(5, -3); squares[3] = create_square(3, -2); // [3]
        break;
    case O:
        squares[0] = create_square(4, -3); squares[1] = create_square(5, -3); // [0][1]
        squares[2] = create_square(4, -2); squares[3] = create_square(5, -2); // [2][3]
        break;
    case S:
        squares[0] = create_square(4, -3); squares[1] = create_square(5, -3); //    [0][1]
        squares[2] = create_square(3, -2); squares[3] = create_square(4, -2); // [2][3]
        break;
    case T:
        squares[0] = create_square(3, -3); squares[1] = create_square(4, -3); // [0][1][2]
        squares[2] = create_square(5, -3); squares[3] = create_square(4, -2); //    [3]   
        break;
    case Z:
        squares[0] = create_square(3, -3); squares[1] = create_square(4, -3); // [0][1]
        squares[2] = create_square(4, -2); squares[3] = create_square(5, -2); //    [2][3]
        break;
    case B_NONE:
    case B_MAX:
    default:
        print_color("Blocktype not implemented", TXT_RED);
        break;
    }
    return squares;
}


/**
 * Get the right color for the block type.
 * 
 * \param block_type the block_type of the tetromino.
 * \return the right Color for that block type.
*/
Color get_color_for_block_type(BlockType block_type)
{
    Color color = C_NONE;
    switch(block_type) 
    {
        case I:
            color = LIGHT_BLUE;
            break;
        case J:
            color = DARK_BLUE;
            break;
        case L:
            color = ORANGE;
            break;
        case O:
            color = YELLOW;
            break;
        case S:
            color = GREEN;
            break;
        case T:
            color = PURPLE;
            break;
        case Z:
            color = RED;
            break;
        case B_NONE:
        case B_MAX:
        default:
            print_color("BlockType not implemented", TXT_RED);
            break;
    }
    return color;
}


/**
 * Get the number of rotation for that particular block type.
 * 
 * \return the number of rotation for that block type.
*/
uint8_t get_number_of_rotation(BlockType block_type) 
{
    switch(block_type) 
    {
        case Z:
        case S:
        case I:
            return 2;
        case J:
        case L:
        case T:
            return 4;
        case O:
            return 1;
        case B_NONE:
        case B_MAX:
        default:
            print_color("get_number_of_rotation => block_type undefined", TXT_RED);
            return 0;
    }
}


/**
 * Get the next rotation additions to all the squares for that tetromino.
 * 
 * \param tetromino the tetromino to get the next rotation.
 * \return [nb_squares][2] array that contains the moves for all the squares.
*/
int8_t** get_next_rotation_moves(Tetromino* tetromino)
{
    int8_t** moves = malloc(sizeof(int8_t*) * NB_SQUARES);
    for (uint8_t i = 0; i < NB_SQUARES; i++)
        moves[i] = calloc(2, sizeof(int8_t));

    switch(tetromino->block_type) 
    {
        case I:
            fill_moves_for_I(tetromino->rotation, moves);
            break;
        case J:
            fill_moves_for_J(tetromino->rotation, moves);
            break;
        case L:
            fill_moves_for_L(tetromino->rotation, moves);
            break;
        case O:
            //fill_moves_for_O(tetromino->rotation, moves);
            break;
        case S:
            fill_moves_for_S(tetromino->rotation, moves);
            break;
        case T:
            fill_moves_for_T(tetromino->rotation, moves);
            break;
        case Z:
            fill_moves_for_Z(tetromino->rotation, moves);
            break;
        case B_NONE:
        case B_MAX:
        default:
            print_color("get_next_rotation_moves => block_type undefined", TXT_RED);
            break;
    }

    return moves;
}


/**
 * Update the actual rotation of the tetromino to the next rotation.
*/
void update_rotation(Tetromino* tetromino) 
{
    switch (tetromino->block_type)
    {
        case Z:
        case S:
        case I:
            tetromino->rotation = (tetromino->rotation + 1) % 2; // 2 states
            break;
        case J:
        case L:
        case T:
            tetromino->rotation = (tetromino->rotation + 1) % 4; // 4 states
            break;
        case O:
            break;
        case B_NONE:
        case B_MAX:
        default:
            print_color("update_rotation => block_type undefined", TXT_RED);
            break;
    }
}


void fill_moves_for_I(uint8_t rotation, int8_t** moves)
{
    if (rotation == 0)
    {																		   // [0]
        moves[0][0] = 1; moves[0][1] = -1; // 2nd square stay				   // [1]
        moves[2][0] = -1; moves[2][1] = 1; moves[3][0] = -2; moves[3][1] = 2;  // [2]
    }																		   // [3]
    else 
    {
        moves[0][0] = -1; moves[0][1] = 1; // 2nd square stay				   // [0][1][2][3]
        moves[2][0] = 1; moves[2][1] = -1; moves[3][0] = 2; moves[3][1] = -2; 
    }
}

void fill_moves_for_J(uint8_t rotation, int8_t** moves)
{
    if(rotation == 0)
    {																		   //    [0]
        moves[0][0] = 1; moves[0][1] = -1; // 2nd square stay				   //    [1] 
        moves[2][0] = -1; moves[2][1] = 1; moves[3][0] = -2; moves[3][1] = 0;  // [3][2]
    }
    else if(rotation == 1)
    {
        moves[0][0] = 1; moves[0][1] = 1; // 2nd square stay				   // [3]
        moves[2][0] = -1; moves[2][1] = -1; moves[3][0] = 0; moves[3][1] = -2; // [2][1][0]
    }
    else if(rotation == 2) 
    {																		   // [2][3]
        moves[0][0] = -1; moves[0][1] = 1; // 2nd square stay				   // [1]
        moves[2][0] = 1; moves[2][1] = -1; moves[3][0] = 2; moves[3][1] = 0;   // [0]
    }
    else 
    {
        moves[0][0] = -1; moves[0][1] = -1; // 2nd square stay				   // [0][1][2]
        moves[2][0] = 1; moves[2][1] = 1; moves[3][0] = 0; moves[3][1] = 2;    //       [3]
    }
}

void fill_moves_for_L(uint8_t rotation, int8_t** moves)
{
    if(rotation == 0) 
    {																		   // [3][0]
        moves[0][0] = 1; moves[0][1] = -1; // 2nd square stay				   //    [1] 
        moves[2][0] = -1; moves[2][1] = 1; moves[3][0] = 0; moves[3][1] = -2;  //    [2]
    }
    else if(rotation == 1)
    {
        moves[0][0] = 1; moves[0][1] = 1; // 2nd square stay				   //       [3] 
        moves[2][0] = -1; moves[2][1] = -1; moves[3][0] = 2; moves[3][1] = 0;  // [2][1][0]
    }
    else if(rotation == 2) 
    {																		   // [2]
        moves[0][0] = -1; moves[0][1] = 1; // 2nd square stay				   // [1] 
        moves[2][0] = 1; moves[2][1] = -1; moves[3][0] = 0; moves[3][1] = 2;   // [0][3]
    }
    else 
    {
        moves[0][0] = -1; moves[0][1] = -1; // 2nd square stay				   // [0][1][2]
        moves[2][0] = 1; moves[2][1] = 1; moves[3][0] = -2; moves[3][1] = 0;   // [3]
    }
}

/*void fill_moves_for_O(uint8_t rotation, int8_t** moves)
{  
    // Nothing to do lol
}*/

void fill_moves_for_S(uint8_t rotation, int8_t** moves)
{
    if (rotation == 0)
    {																		   // [2]
        moves[0][0] = 1; moves[0][1] = 0; moves[1][0] = 0; moves[1][1] = 1;    // [3][0] 
        moves[2][0] = 1; moves[2][1] = -2; moves[3][0] = 0; moves[3][1] = -1;  //    [1]
    }
    else
    {
        moves[0][0] = -1; moves[0][1] = 0; moves[1][0] = 0; moves[1][1] = -1;  //    [0][1] 
        moves[2][0] = -1; moves[2][1] = 2; moves[3][0] = 0; moves[3][1] = 1;  // [2][3]
    }
}

void fill_moves_for_T(uint8_t rotation, int8_t** moves)
{
    if (rotation == 0)
    {																		   //    [0]
        moves[0][0] = 2; moves[0][1] = 0; moves[1][0] = 1; moves[1][1] = 1;    // [3][1] 
        moves[2][0] = 0; moves[2][1] = 2; // 3rd square stay				   //    [2]
    }
    else if (rotation == 1)
    {
        moves[0][0] = 0; moves[0][1] = 2; moves[1][0] = -1; moves[1][1] = 1;   //    [3] 
        moves[2][0] = -2; moves[2][1] = 0; // 3rd square stay				   // [2][1][0]
    }
    else if (rotation == 2)
    {																		   // [0]
        moves[0][0] = -2; moves[0][1] = 0; moves[1][0] = -1; moves[1][1] = -1; // [1][3] 
        moves[2][0] = 0; moves[2][1] = -2; // 3rd square stay				   // [2]
    }
    else
    {
        moves[0][0] = 0; moves[0][1] = -2; moves[1][0] = 1; moves[1][1] = -1;   // [0][1][2]
        moves[2][0] = 2; moves[2][1] = 0; // 3rd square stay				    //    [3]
    }
}

void fill_moves_for_Z(uint8_t rotation, int8_t** moves)
{
    if (rotation == 0)
    {																		   //    [0]
        moves[0][0] = 1; moves[0][1] = -1; // 2nd square stay				   // [2][1] 
        moves[2][0] = -1; moves[2][1] = -1; moves[3][0] = -2; moves[3][1] = 0; // [3]
    }
    else
    {
        moves[0][0] = -1; moves[0][1] = 1; // 2nd square stay                  // [0][1] 
        moves[2][0] = 1; moves[2][1] = 1; moves[3][0] = 2; moves[3][1] = 0;    //    [2][3]
    }
}
