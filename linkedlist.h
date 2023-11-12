#include <stdlib.h>
#include <SDL2/SDL.h>

//Snake Part
typedef struct SnakeNode
{
    SDL_Rect body;
    struct SnakeNode *next;
} SnakeNode;

//Entire Snake
typedef struct Snake
{
    SnakeNode *head, *tail;
} Snake;

Snake *initializeSnake();
void insertBeginning(Snake *snake, int x, int y);
void insertEnd(Snake *snake, int x, int y);
void deleteBeginning(Snake *snake);
void deleteEnd(Snake *snake);