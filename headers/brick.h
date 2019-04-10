#ifndef BRICK_H
#define BRICK_H

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Ship
 */
typedef struct Brick Brick;

/**
 * @brief The Ship struct
 */
struct Brick
{
    double m_x;
    double m_y;
    double m_vx;
    double m_vy;
    SDL_Rect m_src;
};

/**
 * @brief initializeShip : initialize new ship
 * @param ship
 */
void initializeBrick(Brick* brick);

#endif
