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

int key = 0;
Gui_Brick b;

void initializeRound(Round* r, Ship* s, Ball* b ,char filename[], int row, int col){
    reafFile(r, filename, row, col);
    r->ship = s;
    r->ball = b;
}

///
/// \brief reafFile : method which read the content of a file to build a wall of brick
/// \param r : round
/// \param filename : name of the file
/// \param row : number of rows
/// \param col : number of columns
///
void reafFile(Round* r, char filename[], int row, int col){

    int i = 0;
    // Number of values in the array
    int nbValeur = row*col;
    // column in the brick array
    int c =0 ;
    // row in the brick array
    int l = 0;
    //array of the values in the file
    int tab[nbValeur];

    initTab(r->tab_bricks, row, col);

    // Open file
    FILE* fichier = fopen(filename, "r");

    // If we can open the file
    if (fichier != NULL){

        while (fscanf(fichier, "%d ", &tab[i]) != EOF)
        {
            printf("%d : %d ",i, tab[i]);
            ++i;
        }
        fclose(fichier);
    }

    else {
        printf("Impossible d'ouvrir le fichier");
    }

    // Read the array with the values of the file
    for (int i = 0; i< nbValeur; i++){
        printf("%d ", tab[i]);

        if (c == col){
            l++;
            c = 0;
        }
        // Brick corresponding to the value
        Gui_Brick* test = witch(tab[i]);

        //add the gui_brick into the brick wall
        r->tab_bricks[l][c] = *test ;

        c++;

    }
}

///
/// \brief initTab : initialize the bricks wall
/// \param tab : array storing the bricks
/// \param row : number of rows
/// \param col : number of columns
///
void initTab(Gui_Brick** tab,int row, int col){

    /*Gui_Brick wall[row][col];

    Gui_Brick b;

    for (int i=0; i<row; i++) {
        for (int j=0; j< col;j++) {
            initializeGuiBrick(&b);
            tab[i][j]=b;
        }
    }*/

    tab = (Gui_Brick **) malloc(row*sizeof (Gui_Brick*));

    for (int i=0; i<row; i++) {
        tab[i] = (Gui_Brick*) malloc(col*sizeof (Gui_Brick));

    }
}

///
/// \brief witch : Return the right brick corresponding to the given value
/// \param n : given value from the file
/// \return a gui_brick with the right attributes
///
Gui_Brick* witch(int n){

    initializeGuiBrick(&b);

    switch(n){
    case 0:
        // Brique blanche
        b.key =key;
        b.m_src.x =0;
        b.m_src.y =0;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    case 1:
        // Brique orange
        b.key =key;
        b.m_src.x =32;
        b.m_src.y =0;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    case 2:
        //Brique turquoise
        b.key =key;
        b.m_src.x =64;
        b.m_src.y =0;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    case 3:
        //Brique verte
        b.m_src.x =128;
        b.m_src.y =0;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    case 4:
        //Brique bleue
        b.key =key;
        b.m_src.x =256;
        b.m_src.y =0;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    case 5:
        //Brique verte foncée
        b.key =key;
        b.m_src.x =512;
        b.m_src.y =0;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    case 6:
        //Brique rouge
        b.key =key;
        b.m_src.x =0;
        b.m_src.y =16;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    case 7:
        //Brique bleue claire
        b.key =key;
        b.m_src.x =32;
        b.m_src.y =16;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    case 8:
        //Brique rose
        b.key =key;
        b.m_src.x =64;
        b.m_src.y =16;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    case 9:
        //Brique jaune
        b.key =key;
        b.m_src.x =128;
        b.m_src.y =16;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    case 10:
        //Brique rouge fonce
        b.m_src.x =256;
        b.m_src.y =16;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    case 11:
        //Brique turquoise fonce
        b.m_src.x =512;
        b.m_src.y =16;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    default:
        // Brique blanche
        b.key =key;
        b.m_src.x =0;
        b.m_src.y =0;
        b.m_src.w =32;
        b.m_src.h =16;
        break;
    }
    key ++;

    return &b;
}
