#include "./../headers/ship.h"


void initializeShip(Ship* s){
    s->m_x = 0.0;
    s->m_y = 0.0;
    s->m_vx = 0.0;
    s->m_dir = 0;
    s->m_src.h = 0;
    s->m_src.w = 0;
    s->m_src.x = 0;
    s->m_src.y = 0;
}

