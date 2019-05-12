#include "./../headers/gui_brick.h"

void initializeGuiBrick(Gui_Brick* b){
    b->key = 0;
    b->m_x = 0;
    b->m_y = 0;
    b->m_health = 1;
    b->m_src.x = 0;
    b->m_src.y = 0;
    b->m_src.h = 0;
    b->m_src.w = 0;
    b->m_isShining = false;
    b->m_indexShining = 0;
    b->score = 0;
}
