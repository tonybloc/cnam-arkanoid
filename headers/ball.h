#ifndef BALL_H
#define BALL_H

#include "./include.h"


/**
 * @brief The Ball struct
 */
typedef struct Ball{
    int isLaunch;
    double m_x;
    double m_y;
    double m_vx;
    double m_vy;
    SDL_Rect m_src;
} Ball;

/**
 * @brief initializeBall : initialize ball
 * @param b : ball
 * @param x : position x
 * @param y : position y
 * @param vx : speed x
 * @param vy : speed y
 */
void initializeBall(Ball* b);

#endif
