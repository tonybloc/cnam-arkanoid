#ifndef BONUS_H
#define BONUS_H

#include "./include.h"

typedef struct Bonus
{
    int m_key;
    int m_x;
    int m_y;
    int m_vy;
    SDL_Rect m_src;

} Bonus;

#endif // BONUS_H

void initializeBonus(Bonus* b);
