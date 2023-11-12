#include "linkedlist.h"

Snake *initializeSnake()
{
    Snake *snake = (Snake *)malloc(sizeof(snake));
    if (snake != NULL)
    {
        snake->head = NULL;
        snake->tail = NULL;
    }
    return snake;
}

void insertBeginning(Snake *snake, int x, int y)
{

    if (snake->head == NULL)
    {
        SnakeNode *node = (SnakeNode *)malloc(sizeof(SnakeNode));
        if (node != NULL)
        {
            node->next = snake->tail;
            node->body.x = x;
            node->body.y = y;
            node->body.w = 25;
            node->body.h = 25;
            snake->head = node;
        }
        return;
    }

    else
    {
        SnakeNode *temp = snake->head;
        SnakeNode *node = (SnakeNode *)malloc(sizeof(SnakeNode));
        if (node != NULL)
        {
            node->body.x = x;
            node->body.y = y;
            node->body.w = 25;
            node->body.h = 25;
            node->next = snake->head;
            snake->head = node;
        }
    }
}

void insertEnd(Snake *snake, int x, int y)
{

    if (snake->tail == NULL)
    {
        SnakeNode *node = (SnakeNode *)malloc(sizeof(SnakeNode));
        node->next = NULL;
        snake->head->next = node;
        node->body.x = x;
        node->body.y = y;
        node->body.w = 25;
        node->body.h = 25;
        snake->tail = node;
    }

    else
    {
        SnakeNode *node = (SnakeNode *)malloc(sizeof(SnakeNode));
        if (node != NULL)
        {
            node->next = NULL;
            node->body.x = x;
            node->body.y = y;
            node->body.w = 25;
            node->body.h = 25;
            snake->tail->next = node;
            snake->tail = node;
        }
    }
}

void deleteBeginning(Snake *snake)
{

    if (snake->head != NULL)
    {
        SnakeNode *temp = snake->head;
        snake->head = snake->head->next;
        free(temp);
    }
}

void deleteEnd(Snake *snake)
{

    if (snake->tail != NULL)
    {
        SnakeNode *temp = snake->head;
        while (temp->next != snake->tail)
            temp = temp->next;
        snake->tail = temp;
        free(temp);
    }
}