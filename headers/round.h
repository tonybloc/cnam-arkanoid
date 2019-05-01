#ifndef ROUND_H
#define ROUND_H

#include <stdio.h>
#include <stdlib.h>

#include "ship.h"
#include "ball.h"
#include "gui_brick.h"

/**
 * @brief Ship
 */
typedef struct Round Round;

/**
 * @brief The Ship struct
 */
struct Round
{
   Gui_Brick** tab_bricks;
   Ship* ship;
   Ball* ball;
};

// Initialize a round with a
void initializeRound(Round* r, Ship* s, Ball* b, char filename[], int row, int col);

//read values in the given file
void reafFile(Round* r, char filename[], int row, int col);

// initialize the bricks wall
void initTab(Gui_Brick** tab,int row, int col);

// Determine the right Gui_Brick to the given value read in the file
Gui_Brick* witch(int n);

#endif // ROUND_H
