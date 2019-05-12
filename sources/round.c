#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "./../headers/round.h"

#define NUMBER_MAX_OF_BRICKS 260

void initializeRound(Round* r, Ship* s, Ball* b ,char filename[], int l){
    Gui_Brick** bricks = readFile(filename, l);
    r->tab_bricks = bricks;
    r->ship = s;
    r->ball = b;
    r->level = l;
}

///
/// \brief reafFile : method which read the content of a file to build a wall of brick
/// \param filename : name of the file
///
Gui_Brick** readFile( char filename[], int level){

    int i = 0;
    int brickKey = 0;

    // Allocate memory for wall
    Gui_Brick** bricks = malloc(sizeof(Gui_Brick*)*NUMBER_MAX_OF_BRICKS);

    // Open file
    FILE* fichier = fopen(filename, "r");

    // If we can open the file
    if (fichier != NULL)
    {
        while(fscanf(fichier, "%d ", &brickKey) != EOF)
        {
            // Add new bricks in wall
            if(brickKey >= 0 && brickKey <= 13)
            {
                Gui_Brick* brk = witch(brickKey, level);
                bricks[i] = brk;
                i++;
            }
            else
            {
                bricks[i] = NULL;
                i++;
            }

        }
        fclose(fichier);
    }
    else
    {
        printf("Impossible d'ouvrir le fichier");
    }

    return bricks;
}

///
/// \brief witch : Return the right brick corresponding to the given value
/// \param n : given value from the file
/// \return a gui_brick with the right attributes
///
Gui_Brick* witch(int n, int level){

    Gui_Brick* b = malloc(sizeof(Gui_Brick)* NUMBER_MAX_OF_BRICKS);

    initializeGuiBrick(b);

    switch(n){
    case 0:
        // Brique blanche
        b->key = 0;
        b->m_src.x =0;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 50;
        break;
    case 1:
        // Brique orange
        b->key = 1;
        b->m_src.x =32;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 60;
        break;
    case 2:
        //Brique turquoise
        b->key = 2;
        b->m_src.x =64;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 70;
        break;
    case 3:
        //Brique verte
        b->key = 3;
        b->m_src.x =96;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 80;
        break;
    case 4:
        //Brique bleue
        b->key = 4;
        b->m_src.x =128;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 100;
        break;
    case 5:
        //Brique verte foncée
        b->key = 5;
        b->m_src.x =160;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 85;
        break;
    case 6:
        //Brique rouge
        b->key = 6;
        b->m_src.x =0;
        b->m_src.y =16;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 90;
        break;
    case 7:
        //Brique bleue claire
        b->key = 7;
        b->m_src.x =32;
        b->m_src.y =16;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 95;
        break;
    case 8:
        //Brique rose
        b->key = 8;
        b->m_src.x =64;
        b->m_src.y =16;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 110;
        break;
    case 9:
        //Brique jaune
        b->key = 9;
        b->m_src.x =96;
        b->m_src.y =16;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 120;
        break;
    case 10:
        //Brique rouge fonce
        b->key = 10;
        b->m_src.x =128;
        b->m_src.y =16;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 125;
        break;
    case 11:
        //Brique turquoise fonce
        b->key = 11;
        b->m_src.x =160;
        b->m_src.y =16;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 130;
        break;
    case 12:
        // Brique argenté
        b->key = 12;
        b->m_health = 2;
        b->m_src.x =0;
        b->m_src.y =32;
        b->m_src.w =32;
        b->m_src.h =16;
        b->score = 50 * level;
        b->m_isShining = true;
        b->m_indexShining = 0;
        break;
    case 13:
        // Brique dorée
        b->key = 13;
        b->m_health = 1;
        b->m_src.x =0;
        b->m_src.y =48;
        b->m_src.w =32;
        b->m_src.h =16;
        b->m_isShining = true;
        b->m_indexShining = 0;
        b->score = 0;
        break;
    default:
        b = NULL;
        free(b);
    }

    return b;
}
