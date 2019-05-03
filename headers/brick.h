#ifndef BRICK_H
#define BRICK_H

#include "./include.h"

/**
 * @brief The Ship struct
 */
typedef struct Brick
{
    double m_x;
    double m_y;
    double m_vx;
    double m_vy;
    SDL_Rect m_src;
} Brick;

/**
 * @brief initializeShip : initialize new ship
 * @param ship
 */
void initializeBrick(Brick* brick);

#endif
