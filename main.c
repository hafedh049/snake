#include <stdio.h>
// Game Engine written in C
#include <SDL2/SDL.h>
#include <time.h>
#include "linkedlist.c"

// Initializing CONSTANTS
#define WIDTH 800
#define HEIGHT 600
#define true 1
#define false 0

// Game Loop condition variable
int isRunning;

// Entry Point of the program or the game it accepts number of arguments and array of strings which means the arguments
int main(int argc, char **argv)
{
    // Initialize the SDL Library
    SDL_Init(SDL_INIT_EVERYTHING); /*ALL THE ATTRIBUTES (AUDIO, MIC, VIDEO, SENSORS, JOYSTICKS, ...)*/

    // This is the main window of the game, the SDL_CreateWindow returns an pointer instance of the SDL_Window it accepts the title, the positions (x, y) and the width and height.
    SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

    // Responsible for the 2D "context", this guy is the one who draws.
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    // Turning the draw color to black (the background color)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    // Its good to always clear the rendered at the start
    SDL_RenderClear(renderer);
    // Make the rendered visible on the window
    SDL_RenderPresent(renderer);

    // The starting x and y of the Snake
    int startingx = WIDTH / 2;
    int startingy = HEIGHT / 2;

    // Creating the Snake
    Snake *snake = initializeSnake();

    // Inserting 5 parts of the snake at the beginning and each step is adding/substracting 25px
    insertBeginning(snake, startingx, startingy);
    insertEnd(snake, startingx + 25, startingy);
    insertBeginning(snake, startingx - 25, startingy);
    insertBeginning(snake, startingx - 50, startingy);
    insertBeginning(snake, startingx - 75, startingy);

    // Creating the snake's food with (x, y) = (100, 100) and dimension of 25x25
    SDL_Rect food;
    food.x = 100;
    food.y = 100;
    food.w = 25;
    food.h = 25;

    // Changing the color to red for the food
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    // Drawing the food's border
    SDL_RenderDrawRect(renderer, &food);
    // Fills the food with red color
    SDL_RenderFillRect(renderer, &food);
    // Showing the food
    SDL_RenderPresent(renderer);

    // Initializing the seed's generator for pseudo-random numbers
    srand(time(0));

    // Setting the GLCV to true
    isRunning = 1;
    // The done variable is the responsible for checking if the snake touches itself or touches the boundaries
    int done = false;
    // This guy is responsible for making the snake freez at the beginning or not
    int isStarting = true;

    // Initializing the directions
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    // Equivalent for while True in Python
    while (isRunning)
    {
        // This variable e contains the all the events inside a queue
        SDL_Event e;

        // We should treate all the pending events at the beggining
        while (SDL_PollEvent(&e))
        {
            // SDL_PollEvent return and event from the queue

            // Testing the types of the events (QUIT, KEY_PRESS, ...)
            switch (e.type)
            {
                // QUIT THE MAIN LOOP
            case SDL_QUIT:
                isRunning = 0;
                break;

            case SDL_KEYDOWN:
                // e.key is the key object that containst the keysym which is the simulation of the scancode tat is the actual physical key code
                switch (e.key.keysym.scancode)
                {
                // ARROW UP || W
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_W:
                    // IF THE SNAKE IS GOING FORWARD WE CAN'T REVERSE IT THATS THE REALITY
                    if (down == 1)
                        break;
                    up = 1;
                    down = 0;
                    left = 0;
                    right = 0;
                    isStarting = false;
                    break;

                // ARROW DOWN || S
                case SDL_SCANCODE_DOWN:
                case SDL_SCANCODE_S:
                    // IF THE SNAKE IS GOING DOWN WE CAN'T REVERSE IT THATS THE REALITY
                    if (up == 1)
                        break;
                    up = 0;
                    down = 1;
                    left = 0;
                    right = 0;
                    isStarting = false;
                    break;

                // ARROW LEFT || A
                case SDL_SCANCODE_LEFT:
                case SDL_SCANCODE_A:
                    // SAME THING + IF THE SNAKE IS FROZEN
                    if (right == 1 || isStarting == true)
                        break;
                    up = 0;
                    down = 0;
                    left = 1;
                    right = 0;
                    break;

                // ARROW RIGHT || D
                case SDL_SCANCODE_RIGHT:
                case SDL_SCANCODE_D:
                    if (left == 1)
                        break;
                    up = 0;
                    down = 0;
                    left = 0;
                    right = 1;
                    isStarting = false;
                    break;
                }
                break;

            default:
                break;
            }
        }

        // aSSUME THAT THE TAIL IS THE HEAD BECAUSE WE ROTATED THE SNAKE AT THE BEGINNING
        int headx = snake->tail->body.x;
        int heady = snake->tail->body.y;

        if (up && !down)
        { // FOR EACH FRAME WE INSERT AT THE END AND RETRAIN FROM THE BEGINNING
            // DESCRIMENTING THE Y BY -25 BECAUSE IN THE GRID OF XY WE SHOULD REDUCE Y TO GO ABOVE
            insertEnd(snake, headx, heady - 25);

            // IF SURPASSES THE BORDERS
            if (heady <= 0)
            {
                SDL_Delay(1500);
                isRunning = false;
                break;
            }

            deleteBeginning(snake);
        }

        if (!up && down)
        {
            insertEnd(snake, headx, heady + 25);

            if ((heady + 25) >= HEIGHT)
            {
                SDL_Delay(1500);
                isRunning = false;
                break;
            }

            deleteBeginning(snake);
        }

        if (left && !right)
        {
            insertEnd(snake, headx - 25, heady);

            if (headx <= 0)
            {
                SDL_Delay(1500);
                isRunning = false;
                break;
            }

            deleteBeginning(snake);
        }

        if (!left && right)
        {
            insertEnd(snake, headx + 25, heady);

            if ((headx + 25) >= WIDTH)
            {
                SDL_Delay(1500);
                isRunning = false;
                break;
            }

            deleteBeginning(snake);
        }

        // SETTING EVERYTHING TO BLACK AND CLEARING THE RENDERER
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // RE-DRAWING THE FOOD
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderDrawRect(renderer, &food);
        SDL_RenderFillRect(renderer, &food);

        // CHECK IF THE SNAKE ATE ITS FOOD IF YES INCRESE THE SNAKE BODY AND RE-POINT THE FOOD
        if (snake->tail->body.x == food.x && snake->tail->body.y == food.y)
        {
            // rand() return a number between 0 and 20 millions
            int foodx = rand() % 31;
            int foody = rand() % 23;

            food.x = foodx * 25;
            food.y = foody * 25;

            int tailx = snake->head->body.x;
            int taily = snake->head->body.y;

            insertBeginning(snake, tailx, taily);
        }

        // SET THE SNAKE COLOR TO WHITE
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SnakeNode *temp = snake->head;

        while (temp->next != snake->tail)
        {
            // If the snake touches itself
            if (snake->tail->body.x == temp->body.x && snake->tail->body.y == temp->body.y)
            {
                // Wait before doing something
                SDL_Delay(1500);
                // End of game
                done = 1;
                // Exiting the loop of traversing
                break;
            }
            // Moving forward the traverse all the snake
            temp = temp->next;
        }

        // If we are done we should exit the loop
        if (done == 1)
            break;

        /*Redrawing the snake after each frame (200ms)*/
        temp = snake->head;

        while (temp != NULL)
        {
            SDL_RenderDrawRect(renderer, &temp->body);
            SDL_RenderFillRect(renderer, &temp->body);
            temp = temp->next;
        }

        // Re-representing the snake again
        SDL_RenderPresent(renderer);

        SDL_Delay(200);
    }

    // Destroying and liberating memory for both the renderer and the window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Closing the window
    SDL_Quit();

    return 0;
}
