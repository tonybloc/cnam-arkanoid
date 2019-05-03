#ifndef SHIP_H
#define SHIP_H

#include "./include.h"

/**
 * @brief The Ship struct
 */
typedef struct Ship
{
    double m_x;
    double m_y;
    double m_vx;
    double m_vy;
    SDL_Rect m_src;
} Ship;

/**
 * @brief initializeShip : initialize new ship
 * @param ship
 */
void initializeShip(Ship* ship);


#endif
