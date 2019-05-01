#ifndef GUI_BRICK_H
#define GUI_BRICK_H

#include <SDL.h>

/**
 * @brief Gui_Brick
 */
typedef struct Gui_Brick Gui_Brick;


/**
 * @brief The Brick struct
 */
struct Gui_Brick
{
    int key;
    SDL_Rect m_src;
};

/**
 * @brief initializeBrick : initialize new gui_brick
 * @param guiBrick
 */
void initializeGuiBrick(Gui_Brick* guiBrick);

#endif // GUI_BRICK_H
