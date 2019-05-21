#include "./../headers/bonus.h"


void initializeBonus(Bonus* b)
{
    b->m_key = 0;
    b->m_x = 0;
    b->m_y = 0;
    b->m_vy = 5;
    b->m_src.h =0;
    b->m_src.w = 0;
    b->m_src.x = 0;
    b->m_src.y = 0;
}
