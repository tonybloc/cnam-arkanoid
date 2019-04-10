#ifndef SHIP_H
#define SHIP_H

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Ship
 */
typedef struct Ship Ship;

/**
 * @brief The Ship struct
 */
struct Ship
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
void initializeShip(Ship* ship);

#endif
