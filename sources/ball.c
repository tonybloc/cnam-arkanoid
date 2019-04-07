#include "./../headers/ball.h"


void initializeBall(Ball* b, double x, double y, double vx, double vy){
    b->m_x = x;
    b->m_y = y;
    b->m_vx = vx;
    b->m_vy = vy;
}
