#ifndef GUI_BRICK_H
#define GUI_BRICK_H

#include "./include.h"

/**
 * @brief The Brick struct
 */
typedef struct Gui_Brick
{
    int key;
    double m_x;
    double m_y;
    SDL_Rect m_src;
} Gui_Brick;

/**
 * @brief initializeBrick : initialize new gui_brick
 * @param guiBrick
 */
void initializeGuiBrick(Gui_Brick* guiBrick);

#endif // GUI_BRICK_H
