#include "./../headers/ball.h"


void initializeBall(Ball* b){
    b->isLaunch = 0;
    b->m_x = 0.0;
    b->m_y = 0.0;
    b->m_vx = 0.0;
    b->m_vy = 0.0;
    b->m_src.h = 0;
    b->m_src.w = 0;
    b->m_src.x = 0;
    b->m_src.y = 0;
}
