/*SDL_Rect brick_White = { 0, 0, 32, 16 };
SDL_Rect brick_Orange = { 32, 0, 32, 16 };
SDL_Rect brick_Turquoise = { 64, 0, 32, 16 };
SDL_Rect brick_Green = { 128, 0, 32, 16 };
SDL_Rect brick_Blue = { 256, 0, 32, 16 };
SDL_Rect brick_Vert_Fonce = { 512, 0, 32, 16 };

SDL_Rect brick_Red = { 0, 16, 32, 16 };
SDL_Rect brick_Bleu_Clair = { 32, 16, 32, 16 };
SDL_Rect brick_Rose = { 64, 16, 32, 16 };
SDL_Rect brick_Jaune = { 128, 16, 32, 16 };
SDL_Rect brick_Rouge_Fonce = { 256, 16, 32, 16 };
SDL_Rect brick_Turquoise_Fonce = { 512, 16, 32, 16 };
*/
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "./../headers/round.h"

#define NUMBER_MAX_OF_BRICKS 260

void initializeRound(Round* r, Ship* s, Ball* b ,char filename[]){
    Gui_Brick** bricks = readFile(filename);
    r->tab_bricks = bricks;
    r->ship = s;
    r->ball = b;
}

///
/// \brief reafFile : method which read the content of a file to build a wall of brick
/// \param filename : name of the file
///
Gui_Brick** readFile( char filename[]){

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
                Gui_Brick* brk = witch(brickKey);
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
Gui_Brick* witch(int n){

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
        break;
    case 1:
        // Brique orange
        b->key = 1;
        b->m_src.x =32;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    case 2:
        //Brique turquoise
        b->key = 2;
        b->m_src.x =64;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    case 3:
        //Brique verte
        b->key = 3;
        b->m_src.x =96;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    case 4:
        //Brique bleue
        b->key = 4;
        b->m_src.x =128;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    case 5:
        //Brique verte foncée
        b->key = 5;
        b->m_src.x =160;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    case 6:
        //Brique rouge
        b->key = 6;
        b->m_src.x =0;
        b->m_src.y =16;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    case 7:
        //Brique bleue claire
        b->key = 7;
        b->m_src.x =32;
        b->m_src.y =16;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    case 8:
        //Brique rose
        b->key = 8;
        b->m_src.x =64;
        b->m_src.y =16;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    case 9:
        //Brique jaune
        b->key = 9;
        b->m_src.x =96;
        b->m_src.y =16;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    case 10:
        //Brique rouge fonce
        b->key = 10;
        b->m_src.x =128;
        b->m_src.y =16;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    case 11:
        //Brique turquoise fonce
        b->key = 11;
        b->m_src.x =160;
        b->m_src.y =16;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    case 12:
        // Brique blanche
        b->key = 12;
        b->m_src.x =0;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    case 13:
        // Brique blanche
        b->key = 13;
        b->m_src.x =0;
        b->m_src.y =0;
        b->m_src.w =32;
        b->m_src.h =16;
        break;
    default:
        NULL;
        free(b);
    }

    return b;
}
