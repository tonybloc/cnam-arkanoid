#ifndef ROUND_H
#define ROUND_H

#include <stdio.h>
#include <stdlib.h>

#include "ship.h"
#include "ball.h"
#include "gui_brick.h"


/**
 * @brief The Ship struct
 */
typedef struct Round
{
   Gui_Brick** tab_bricks;
   Ship* ship;
   Ball* ball;
} Round;


// Initialize a round with a
void initializeRound(Round* r, Ship* s, Ball* b, char filename[]);

//read values in the given file
Gui_Brick** readFile(char filename[]);

// Determine the right Gui_Brick to the given value read in the file
Gui_Brick* witch(int n);

#endif // ROUND_H
