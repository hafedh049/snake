#include <stdio.h>
//Game Engine written in C
#include <SDL2/SDL.h>
#include <time.h>
#include "linkedlist.c"

//Initializing CONSTANTS
#define WIDTH 800
#define HEIGHT 600
#define true 1
#define false 0

//Game Loop condition variable
int isRunning;

//Entry Point of the program or the game it accepts number of arguments and array of strings which means the arguments
int main(int argc, char **argv)
{
    //Initialize the SDL Library
    SDL_Init(SDL_INIT_EVERYTHING /*ALL THE ATTRIBUTES (AUDIO, MIC, VIDEO, SENSORS, JOYSTICKS, ...)*/);

    //This is the main window of the game, the SDL_CreateWindow returns an pointer instance of the SDL_Window it accepts the title, the positions (x, y) and the width and height. 
    SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

    //If an error happens the SDL_GetError() function will returns the last error message and terminates the thread.
    if (!window)
        fprintf(stderr, "%s\n", SDL_GetError()); //fprintf : File Printer to the standard error output 'stderr' 

    //Responsible for the 2D "context", this guy is the one who draws. 
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    //Safety checks
    if (!renderer)
        fprintf(stderr, "%s\n", SDL_GetError());

    //Turning the draw color to black (the background color)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    //Its good to always clear the rendered at the start
    SDL_RenderClear(renderer);
    //Make the rendered visible on the window
    SDL_RenderPresent(renderer);

    //The starting x and y of the Snake
    int startingx = WIDTH / 2;
    int startingy = HEIGHT / 2;

    //Creating the Snake
    Snake *snake = initializeSnake();

    //Inserting 5 parts of the snake at the beginning and each step is adding/substracting 25px
    insertBeginning(snake, startingx, startingy);
    insertEnd(snake, startingx + 25, startingy);
    insertBeginning(snake, startingx - 25, startingy);
    insertBeginning(snake, startingx - 50, startingy);
    insertBeginning(snake, startingx - 75, startingy);

    //Creating the snake's food with (x, y) = (100, 100) and dimension of 25x25
    SDL_Rect food;
    food.x = 100;
    food.y = 100;
    food.w = 25;
    food.h = 25;

    //Changing the color to red for the food
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    //Drawing the food's border
    SDL_RenderDrawRect(renderer, &food);
    //Fills the food with red color
    SDL_RenderFillRect(renderer, &food);
    //Showing the food
    SDL_RenderPresent(renderer);

    //Initializing the seed's generator for pseudo-random numbers
    srand(time(0));

    //Setting the GLCV to true the 
    isRunning = 1;
    int done = false;
    int isStarting = true;

    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    while (isRunning)
    {
        SDL_Event e;

        while (SDL_PollEvent(&e))
        {

            switch (e.type)
            {
            case SDL_QUIT:
                isRunning = 0;
                break;

            case SDL_KEYDOWN:
                // case SDL_KEYUP:
                switch (e.key.keysym.scancode)
                {
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_W:
                    if (down == 1)
                        break;
                    up = 1;
                    down = 0;
                    left = 0;
                    right = 0;
                    isStarting = false;
                    break;
                case SDL_SCANCODE_DOWN:
                case SDL_SCANCODE_S:
                    if (up == 1)
                        break;
                    up = 0;
                    down = 1;
                    left = 0;
                    right = 0;
                    isStarting = false;
                    break;
                case SDL_SCANCODE_LEFT:
                case SDL_SCANCODE_A:
                    if (right == 1 || isStarting == true)
                        break;
                    up = 0;
                    down = 0;
                    left = 1;
                    right = 0;
                    break;
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

        int headx = snake->tail->body.x;
        int heady = snake->tail->body.y;

        if (up && !down)
        {
            insertEnd(snake, headx, heady - 25);

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

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderDrawRect(renderer, &food);
        SDL_RenderFillRect(renderer, &food);

        if (snake->tail->body.x == food.x && snake->tail->body.y == food.y)
        {
            int foodx = rand() % 31;
            int foody = rand() % 23;

            food.x = foodx * 25;
            food.y = foody * 25;

            int tailx = snake->head->body.x;
            int taily = snake->head->body.y;

            insertBeginning(snake, tailx, taily);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SnakeNode *temp = snake->head;

        while (temp->next != snake->tail)
        {
            //If the snake touches itself
            if (snake->tail->body.x == temp->body.x && snake->tail->body.y == temp->body.y)
            {
                //Wait before doing something
                SDL_Delay(1500);
                //End of game
                done = 1;
                //Exiting the loop
                break;
            }
            //Moving forward the traverse all the snake
            temp = temp->next;
        }

        if (done == 1)
            break;

        temp = snake->head;

        while (temp != NULL)
        {
            SDL_RenderDrawRect(renderer, &temp->body);
            SDL_RenderFillRect(renderer, &temp->body);
            temp = temp->next;
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(200);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
