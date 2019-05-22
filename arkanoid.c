#include "./prototypes.h"
#include <stdio.h>

#define NUMBER_MAX_OF_BRICKS 260
#define NUMBER_OF_COLUMN_IN_BRICKS 13
#define NUMBER_MAX_ROW_IN_BRICKS 20

#define HEIGHT 600
#define WIDTH 420
#define HEIGHT_CENTER HEIGHT/2
#define WIDTH_CENTER WIDTH/2
#define HEIGHT_QUARTER HEIGHT/4
#define WIDTH_QUARTER WIDTH/4

#define MARGIN_PX 8

#define BALL_SPEED_X 10
#define BALL_SPEED_Y 10

#define BRICK_GRAY 12
#define BRICK_GOLD 13
#define BRICK_SHINING_ITERATION 6

#define NUMBER_OF_MENU 4
#define NUMBER_OF_BACKGROUD 6
#define MIN_HEIGH_OF_BOARD (HEIGHT/15)
#define MAX_HEIGH_OF_BOARD (HEIGHT/15)*14

#define SPRITE_CHAR_HEIGHT 32
#define SPRITE_CHAR_WIDHT 32
#define SPACE_BETWEEN_CHAR 20;
#define VIEW_SIZE_OF_SPACE_MEDIUM 18
#define VIEW_SIZE_OF_CHAR_BIG 1.5
#define VIEW_SIZE_OF_CHAR_MEDIUM 1
#define VIEW_SIZE_OF_CHAR_LOW 0.5

#define BONUS_NONE 'N'
#define BONUS_SLOWDOWN 'S'
#define BONUS_CATCHFIRE 'C'
#define BONUS_EXPAND 'E'
#define BONUS_DIVIDE 'D'
#define BONUS_LASERBEAM 'L'
#define BONUS_BREAK 'B'
#define BONUS_PLAYERADDITION 'P'

#define NUMBER_MAX_OF_BALL 3
#define NUMBER_MAX_OF_LASER 50

#define NUMBER_TAG_NAME 3
#define NUMBER_OF_ALPHABET 26
/* ------------ */
/*   VARIABLES  */
/* ------------ */


/* Enumeration */
enum ARKANOID_WINDOW_OPTION { WINDOW_MENU = 1, WINDOW_BOARD = 2, WINDOW_QUIT = 3, WINDOW_SCORES = 4, WINDOW_ABOUT = 5, WINDOW_SAVE = 6};

/* Global Variable */
static int G_Level = 1;
static int G_Score = 0;
static int G_Health = 3;
static bool G_GameOver = false;
static Bonus* G_BonusDropping = NULL;
static Bonus* G_BonusActive = NULL;
static bool G_IsSticky = false;
static int G_SelectorSelected = 0;
static int G_SelectorTagName = 0;
static int G_SelectedChar = ' ';
static Uint64 G_Prev, G_Now;    // timers
static double G_Delta_t;      // refresh frame (ms)
static int G_IntervalTimer = 0;
static int G_NumberOfBall = 0;
/* Arkanoid Levels Path */
static char* G_Level_path = "./public/level_";
static char* G_Level_path_extension = ".txt";
/* Menu Targeted */
static enum ARKANOID_WINDOW_OPTION WindowSelected = WINDOW_MENU;

/* Arkanoid Windows & Surfaces */
static SDL_Window* Arkanoid_Window = NULL;
static SDL_Surface* Arkanoid_Surface = NULL;
static SDL_Surface* Arkanoid_AdvancedSprite = NULL;
static SDL_Surface* Arkanoid_AlphabetSprite = NULL;

/* Backgrounds */
static SDL_Rect Arkanoid_Backgrounds[NUMBER_OF_BACKGROUD] = {
    {0,128,48,64},
    {64,128,64,64},
    {128,128,64,64},
    {192,128,64,64},
    {256,128,64,64},
    {320,128,64,64},
};


/* ------------ */
/*     MAIN     */
/* ------------ */

int main(int argc, char* argv[])
{

    printf(" value : %d \n",GetNumberOfPX("Arkanoid", VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_BIG)/2);

    // Initialize SDL : video, audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0 )
    {
        SDL_Log("Erreur : La SDL n'a pas été initialisée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    // Load Window
    Arkanoid_Window = SDL_CreateWindow("Arknoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if(Arkanoid_Window == NULL)
    {
        SDL_Log("Erreur : La Fenêtre n'a pas été chargée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    // Load sprites
    Arkanoid_AlphabetSprite = SDL_LoadBMP("./public/Arkanoid_ascii.bmp");
    Arkanoid_AdvancedSprite = SDL_LoadBMP("./public/Arkanoid_sprites.bmp");
    if(Arkanoid_AdvancedSprite == NULL || Arkanoid_AlphabetSprite == NULL)
    {
        SDL_Log("Erreur : Le sprite n'a pas été chargée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    // Transparancy
    //SDL_SetColorKey(Arkanoid_AlphabetSprite, true, 0); //If we change black backround allowed it
    SDL_SetColorKey(Arkanoid_AdvancedSprite, true, 0);

    // Menu Selector
    while(WindowSelected != WINDOW_QUIT)
    {
        switch(WindowSelected)
        {
        case WINDOW_MENU:
            Arkanoid_ShowMenu(Arkanoid_Window, &Arkanoid_Surface);
            break;
        case WINDOW_BOARD:
            Arkanoid_ShowBoard(Arkanoid_Window, &Arkanoid_Surface);
            break;
        case WINDOW_ABOUT:
            Arkanoid_ShowAbout(Arkanoid_Window, &Arkanoid_Surface);
            break;
        case WINDOW_SCORES:
            Arkanoid_ShowHighScores(Arkanoid_Window, &Arkanoid_Surface);
            break;
        case WINDOW_SAVE:
            Arkanoid_SaveYourScore(Arkanoid_Window, &Arkanoid_Surface);
        case WINDOW_QUIT:
            break;
        }
    }

Redirection_Quit:
    free(Arkanoid_AdvancedSprite);
    free(Arkanoid_AlphabetSprite);
    SDL_FreeSurface(Arkanoid_Surface);
    SDL_DestroyWindow(Arkanoid_Window);
    SDL_Quit();

    return EXIT_SUCCESS;
}


/* ------------ */
/*   METHODES   */
/* ------------ */

void Arkanoid_ShowMenu(SDL_Window* window, SDL_Surface** surface)
{
    *surface = NULL;
    *surface = SDL_GetWindowSurface(window);
    if(surface == NULL)
    {
        SDL_Log("Erreur : La Surface n'a pas été récupérée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    enum MENU_ITEMS { NEW_GAME = 0, HIGH_SCORES = 1, ABOUT = 2, EXIT = 3 };
    char* MenuLabels[NUMBER_OF_MENU] = {"New Game", "High Score", "About", "Exit"};

    SDL_Rect MenuSelectorsPosition[NUMBER_OF_MENU] = {
        {(int)(WIDTH_QUARTER)-32, HEIGHT_CENTER+00, 32, 32},
        {(int)(WIDTH_QUARTER)-32, HEIGHT_CENTER+32, 32, 32},
        {(int)(WIDTH_QUARTER)-32, HEIGHT_CENTER+64, 32, 32},
        {(int)(WIDTH_QUARTER)-32, HEIGHT_CENTER+96, 32, 32}
    };

    // Print title of surface
    Arkanoid_PrintAlphaNumeric(*surface, "Arkanoid",
                               WIDTH_QUARTER,
                               HEIGHT_QUARTER,
                               VIEW_SIZE_OF_SPACE_MEDIUM,
                               VIEW_SIZE_OF_CHAR_BIG);

    for(int index = 0; index < NUMBER_OF_MENU; index++)
    {
        // Display menu label
        Arkanoid_PrintAlphaNumeric(*surface, MenuLabels[index],
                                   WIDTH_QUARTER,
                                   HEIGHT_CENTER+(index*32),
                                   VIEW_SIZE_OF_SPACE_MEDIUM,
                                   VIEW_SIZE_OF_CHAR_MEDIUM);
    }

    // Display current selector
    Arkanoid_PrintAlphaNumeric(*surface, "*",
                               MenuSelectorsPosition[G_SelectorSelected].x,
                               MenuSelectorsPosition[G_SelectorSelected].y,
                               VIEW_SIZE_OF_SPACE_MEDIUM,
                               VIEW_SIZE_OF_CHAR_MEDIUM);


    SDL_bool laucher = SDL_TRUE;

    // Events behaviors
    while(laucher)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                laucher = SDL_FALSE;
                WindowSelected = WINDOW_QUIT;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    laucher = SDL_FALSE;
                    WindowSelected = WINDOW_QUIT;
                    break;
                case SDLK_UP:
                    // Remove old selector
                    Arkanoid_PrintAlphaNumeric(*surface, " ",
                                               MenuSelectorsPosition[G_SelectorSelected].x,
                                               MenuSelectorsPosition[G_SelectorSelected].y,
                                               VIEW_SIZE_OF_SPACE_MEDIUM,
                                               VIEW_SIZE_OF_CHAR_MEDIUM);

                    // set new selected index
                    G_SelectorSelected = ((G_SelectorSelected-1) < 0) ? NUMBER_OF_MENU-1 : (G_SelectorSelected-1);

                    // Display new selector
                    Arkanoid_PrintAlphaNumeric(*surface, "*",
                                               MenuSelectorsPosition[G_SelectorSelected].x,
                                               MenuSelectorsPosition[G_SelectorSelected].y,
                                               VIEW_SIZE_OF_SPACE_MEDIUM,
                                               VIEW_SIZE_OF_CHAR_MEDIUM);
                    break;
                case SDLK_DOWN:

                    // Remove old selector
                    Arkanoid_PrintAlphaNumeric(*surface, " ",
                                               MenuSelectorsPosition[G_SelectorSelected].x,
                                               MenuSelectorsPosition[G_SelectorSelected].y,
                                               VIEW_SIZE_OF_SPACE_MEDIUM,
                                               VIEW_SIZE_OF_CHAR_MEDIUM);

                    // Update selected selector
                    G_SelectorSelected = ((G_SelectorSelected+1) > NUMBER_OF_MENU-1) ? 0 :(G_SelectorSelected+1);

                    // Display new selector
                    Arkanoid_PrintAlphaNumeric(*surface, "*",
                                               MenuSelectorsPosition[G_SelectorSelected].x,
                                               MenuSelectorsPosition[G_SelectorSelected].y,
                                               VIEW_SIZE_OF_SPACE_MEDIUM,
                                               VIEW_SIZE_OF_CHAR_MEDIUM);
                    break;
                case SDLK_RETURN:
                    switch (G_SelectorSelected)
                    {
                    case NEW_GAME:
                        WindowSelected = WINDOW_BOARD;
                        laucher = SDL_FALSE;
                        break;
                    case HIGH_SCORES:
                        WindowSelected = WINDOW_SCORES;
                        laucher = SDL_FALSE;
                        break;
                    case ABOUT:
                        WindowSelected = WINDOW_ABOUT;
                        laucher = SDL_FALSE;
                        break;
                    case EXIT:
                        WindowSelected = WINDOW_QUIT;
                        laucher = SDL_FALSE;
                        break;
                    default:
                        break;
                    }
                default:
                    break;
                }
                break;           
            default:
                break;
            }
            SDL_UpdateWindowSurface(window);


        }
    }

Redirection_Quit:
    SDL_FillRect(*surface, NULL, 0x000000);
    SDL_FreeSurface(*surface);

}
void Arkanoid_ShowBoard(SDL_Window* window, SDL_Surface** surface)
{
    *surface = NULL;

    // define new surface for window
    *surface = SDL_GetWindowSurface(window);
    if(surface == NULL)
    {
        SDL_Log("Erreur : La Surface n'a pas été récupérée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    Ball** balls = malloc(sizeof(Ball*)*3);
    balls[0] = CreateBall();
    balls[1] = NULL;
    balls[2] = NULL;

    Ship ship = CreateShip();
    Round round = CreateRound(&ship, balls, G_Level);


    SetShipPosition(&ship, (WIDTH_CENTER)-(round.ship->m_src.w/2), (MAX_HEIGH_OF_BOARD-round.ship->m_src.h-32) );
    FixBallOnShip(round.ball[0],&ship);


    // Fill surface background of the window in black
    SDL_FillRect(*surface, NULL, 0x00000000);

    View_UpdateLevel(*surface);
    View_UpdateScore(*surface);
    View_UpdateLife(*surface);


    // Initialize Lasers
    Laser** Lasers = malloc(sizeof(Laser*)*NUMBER_MAX_OF_LASER);
    for (int index = 0 ; index < NUMBER_MAX_OF_LASER; index++)
        Lasers[index] = NULL;


    // Events
    SDL_bool gameIsLauched = SDL_TRUE;

    while(gameIsLauched)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            round.ship->m_dir = 1;
            switch(event.type)
            {
            case SDL_QUIT:
                gameIsLauched = SDL_FALSE;
                break;
            case SDL_MOUSEMOTION:
                round.ship->m_x += event.motion.xrel;
                round.ship->m_dir = (event.motion.xrel >= 0) ? 1 : -1;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    if( (round.ship->m_x) >= 1 )
                    {
                        round.ship->m_x -= 10;
                        round.ship->m_dir = -1;
                    }
                    break;
                case SDLK_RIGHT:
                    if( (round.ship->m_x + round.ship->m_src.w) < WIDTH )
                    {
                        round.ship->m_x += 10;
                        round.ship->m_dir = 1;
                    }
                    break;
                case SDLK_ESCAPE:
                    gameIsLauched = SDL_FALSE;
                    WindowSelected = WINDOW_MENU;
                    break;
                case SDLK_SPACE :
                    for(int index = 0; index < NUMBER_MAX_OF_BALL; index++)
                        if(round.ball[index] != NULL)
                            round.ball[index]->isLaunch = true;

                    if( G_BonusActive != NULL && G_BonusActive->m_key == BONUS_LASERBEAM)
                    {
                        int count = 0;
                        for(int index = 0; (index<NUMBER_MAX_OF_LASER) && (count != 2); index++)
                        {
                            if(Lasers[index] == NULL)
                            {
                                Lasers[index] = (count == 1) ? CreateLaser((int)(round.ship->m_x+round.ship->m_src.w/2-16), (int)ship.m_y) : CreateLaser((int)(round.ship->m_x+round.ship->m_src.w/2+16), (int)ship.m_y);
                                //SDL_Delay(10);
                                count++;
                            }
                        }
                    }


                    if(G_BonusActive != NULL && G_BonusActive->m_key == BONUS_CATCHFIRE)
                    {
                        G_IsSticky = false;
                        for(int index = 0; index < NUMBER_MAX_OF_BALL; index++)
                        {
                            if(round.ball[index] != NULL)
                            {
                                round.ball[index]->m_hookX = -1;
                                round.ball[index]->isSticky = false;
                            }
                        }
                    }

                    break;
                default: break;
                }
                break;
            }
        }
        G_IntervalTimer = (G_IntervalTimer+1) % 3;
        G_Prev = G_Now;
        G_Now = SDL_GetPerformanceCounter();
        G_Delta_t = (double)( (G_Now-G_Prev) * 1000 / SDL_GetPerformanceFrequency());


        if( G_BonusActive != NULL && G_BonusActive->m_key == BONUS_BREAK)
        {
            G_BonusActive = NULL;

            if (G_Level < 36){
                G_Level++;
                gameIsLauched = false;
            }
        }
        else if (!WallIsEmpty(&round)){
            Arkanoid_DrawBoard(*surface, &round, Lasers);
            SDL_UpdateWindowSurface(window);
        }
        else {
            if (G_Level < 36){
                G_Level++;
                gameIsLauched = false;
            }
        }
        if(G_GameOver)
        {
            gameIsLauched = false;
            WindowSelected = WINDOW_SAVE;

        }
    }

Redirection_Quit:
    // free memory of round
    for(int index = 0; index < NUMBER_MAX_OF_LASER; index++)
        free(Lasers[index]);


    for (int index = 0; index < NUMBER_MAX_OF_BRICKS;index++)
        free(round.tab_bricks[index]);

    for(int index = 0; index < NUMBER_MAX_OF_BALL; index++)
        free(round.ball[index]);

    free(round.ball);
    free(round.tab_bricks);

    SDL_FillRect(*surface, NULL, 0x000000);
    SDL_FreeSurface(*surface);
}
void Arkanoid_ShowAbout(SDL_Window* window, SDL_Surface** surface)
{
    *surface = NULL;

    // define new surface for window
    *surface = SDL_GetWindowSurface(window);
    if(surface == NULL){
        SDL_Log("Erreur : La Surface n'a pas été récupérée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    Arkanoid_PrintAlphaNumeric(*surface, "About us", WIDTH_QUARTER, 20, VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_MEDIUM);
    Arkanoid_PrintAlphaNumeric(*surface, "This application was designed by :", 10, HEIGHT_QUARTER+32,VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_LOW);
    Arkanoid_PrintAlphaNumeric(*surface, "- Anthony Mochel", 10, HEIGHT_QUARTER+64, VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_LOW);
    Arkanoid_PrintAlphaNumeric(*surface, "- Carole Treser", 10, HEIGHT_QUARTER+96, VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_LOW);

    SDL_UpdateWindowSurface(window);

    // Events
    SDL_bool laucher = SDL_TRUE;

    while(laucher)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                laucher = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    laucher = SDL_FALSE;
                    WindowSelected = WINDOW_MENU;
                    break;
                default:
                    break;
                }

                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    printf("Coordnnee du click : %d (X) /%d (Y)\n", event.button.x, event.button.y);
                }
                break;
            default:
                break;
            }
            SDL_UpdateWindowSurface(window);
        }
    }

Redirection_Quit:
    SDL_FillRect(*surface, NULL, 0x000000);
    SDL_FreeSurface(*surface);
}
void Arkanoid_ShowHighScores(SDL_Window* window, SDL_Surface** surface)
{
    *surface = NULL;

    // define new surface for window
    *surface = SDL_GetWindowSurface(window);
    if(surface == NULL){
        SDL_Log("Erreur : La Surface n'a pas été récupérée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    Arkanoid_PrintAlphaNumeric(*surface, "Hight Scores", WIDTH_QUARTER, 20,VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_MEDIUM);
    Arkanoid_PrintAlphaNumeric(*surface, "No scores yet !", 10, HEIGHT_QUARTER+32,VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_LOW);
    SDL_UpdateWindowSurface(window);

    SDL_bool laucher = SDL_TRUE;

    // Events
    while(laucher)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                laucher = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    laucher = SDL_FALSE;
                    WindowSelected = WINDOW_MENU;
                    break;
                default:
                    break;
                }

                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    printf("Coordnnee du click : %d (X) /%d (Y)\n", event.button.x, event.button.y);
                }
                break;
            default:
                break;
            }
            SDL_UpdateWindowSurface(window);
        }
    }

Redirection_Quit:
    SDL_FillRect(*surface, NULL, 0x000000);
    SDL_FreeSurface(*surface);
}
void Arkanoid_DrawBoard(SDL_Surface* surface, Round* round, Laser** lasers)
{

    /* -- DISPLAY BACKGROUND -- */

    // Fill surface background of the game only
    SDL_Rect position = { 0.0, 0.0, 0.0, 0.0 };
    for (int y = MIN_HEIGH_OF_BOARD ; y < MAX_HEIGH_OF_BOARD ; y+=64)
    {
        for (int x = 0; x < WIDTH; x += ((G_Level-1)%NUMBER_OF_BACKGROUD==0)?48:64)
        {
            SDL_Rect source = Arkanoid_Backgrounds[(G_Level-1)%NUMBER_OF_BACKGROUD];
            position.x = x;
            position.y = y;
            position.h = 0;
            position.w = 0;
            source.h = (MAX_HEIGH_OF_BOARD-y >= 64) ? 64 : (MAX_HEIGH_OF_BOARD-y);
            source.w = ((G_Level-1)%NUMBER_OF_BACKGROUD==0)?48:64;
            SDL_BlitSurface(Arkanoid_AdvancedSprite, &source, surface, &position);
        }
    }

    /* -- DISPLAY BRICKS WALL -- */
    DisplayWallOfBricks(surface, round, 0, MIN_HEIGH_OF_BOARD );


    /* -- DISPLAY SHIP -- */
    position.x = (int)round->ship->m_x;
    position.y = (int)round->ship->m_y;
    position.h = 0;
    position.w = 0;
    SDL_BlitSurface(Arkanoid_AdvancedSprite, &round->ship->m_src, surface, &position);



    /* -- DISPLAY BONUS -- */
    if(G_BonusDropping != NULL)
    {
        if(G_IntervalTimer == 1)
        {
            G_BonusDropping->m_src.x += (G_BonusDropping->m_src.x < 512)? 32 : -256;
        }
        G_BonusDropping->m_y += 1;
        SDL_Rect destBonus = {(int)G_BonusDropping->m_x, (int)G_BonusDropping->m_y, 0,0};
        SDL_BlitSurface(Arkanoid_AdvancedSprite, &G_BonusDropping->m_src, surface, &destBonus);

        if( (G_BonusDropping->m_x+G_BonusDropping->m_src.w >= round->ship->m_x) && (G_BonusDropping->m_x <= round->ship->m_x + round->ship->m_src.w)
                && (G_BonusDropping->m_y+G_BonusDropping->m_src.h >= round->ship->m_y))
        {

            G_BonusActive = G_BonusDropping;
            G_BonusDropping = NULL;

            double OriginX = 0;
            double OriginY = 0;
            double OriginVx = 0;
            double OriginVy = 0;
            double coef[NUMBER_MAX_OF_BALL];

            G_NumberOfBall = 0;
            switch(G_BonusActive->m_key){
            case BONUS_SLOWDOWN:
                SDL_Delay(20);
                MediumShip(round->ship);
                break;
            case BONUS_CATCHFIRE:
                G_IsSticky = true;
                MediumShip(round->ship);
                break;
            case BONUS_LASERBEAM:
                MediumShip(round->ship);
                break;
            case BONUS_EXPAND:
                BigShip(round->ship);
                break;
            case BONUS_DIVIDE:
                for (int index = 0;index < NUMBER_MAX_OF_BALL; index ++) {
                    if(round->ball[index] != NULL)
                    {
                        OriginX = round->ball[index]->m_x;
                        OriginY = round->ball[index]->m_y;
                        OriginVx = round->ball[index]->m_vx;
                        OriginVy = round->ball[index]->m_vy;
                    }
                    coef[index] = (index%2) ? index/10 : -(index/10);
                }
                for (int index = 0;index < NUMBER_MAX_OF_BALL; index ++) {
                    if(round->ball[index] == NULL)
                    {
                        round->ball[index] = CreateBall();
                        round->ball[index]->m_x = OriginX;
                        round->ball[index]->m_y = OriginY;
                        round->ball[index]->m_vx = OriginVx+(OriginVx*coef[index])*2;
                        round->ball[index]->m_vy = OriginVy;
                        round->ball[index]->isLaunch = true;
                    }
                }

                for (int index = 0; index <NUMBER_MAX_OF_BALL; index++) {
                    if(round->ball[index] != NULL)
                        G_NumberOfBall++;
                };


                break;
            case BONUS_BREAK:
                MediumShip(round->ship);
                // Next level
                break;
            case BONUS_PLAYERADDITION:
                MediumShip(round->ship);
                G_Health +=1;
                View_UpdateLife(surface);
                break;
            default:
                break;
            }
        }
        else if(G_BonusDropping->m_y+G_BonusDropping->m_src.h > MAX_HEIGH_OF_BOARD){
            G_BonusDropping = NULL;
        }
    }

    /* -- DISPLAY LASERS -- */
    for(int index = 0; index < NUMBER_MAX_OF_LASER; index++)
    {
        if(lasers[index] != NULL)
        {
            lasers[index]->m_y -= 2;
            if(lasers[index]->m_y < MIN_HEIGH_OF_BOARD)
            {
                lasers[index] = NULL;
            }
            else{
                position.x = (int)lasers[index]->m_x;
                position.y = (int)lasers[index]->m_y;
                position.w = (int)(lasers[index]->m_src.w*0.6);
                position.h = (int)(lasers[index]->m_src.h*0.6);
                SDL_BlitScaled(Arkanoid_AdvancedSprite, &lasers[index]->m_src, surface, &position);


                /* Collision with brick */
                for (int j = 0; j < NUMBER_MAX_OF_BRICKS; j++)
                {
                    Gui_Brick* brick = round->tab_bricks[j];
                    if(brick != NULL)
                    {

                        if( (lasers[index]->m_x <= brick->m_x+brick->m_src.w) && (lasers[index]->m_x+lasers[index]->m_src.w >= brick->m_x)
                                && (lasers[index]->m_y <= brick->m_y+brick->m_src.h) && (lasers[index]->m_y+lasers[index]->m_src.h >= brick->m_y))
                        {

                            // Shine bricks
                            if( brick->key == BRICK_GOLD || brick->key == BRICK_GRAY)
                            {
                                brick->m_isShining = true;
                                brick->m_indexShining = 1;
                            }

                            // Update brick's health
                            round->tab_bricks[j]->m_health += (brick->key == BRICK_GOLD) ? 0 : -1;

                            if(brick->m_health <= 0)
                            {
                                G_Score += brick->score;

                                // release bonus
                                if( (brick->m_bonus != NULL) && (G_BonusDropping == NULL) && ( (G_BonusActive != NULL && G_BonusActive->m_key != BONUS_DIVIDE) || G_BonusActive == NULL)  )
                                {
                                    G_BonusDropping = brick->m_bonus;
                                    G_BonusDropping->m_y = brick->m_y;
                                    G_BonusDropping->m_x = brick->m_x;
                                }
                                View_UpdateScore(surface);
                                round->tab_bricks[j] = NULL;
                            }

                            lasers[index] = NULL;
                            break;
                        }

                    }
                }

            }



        }
    }

    /* -- DISPLAY BALLS -- */
    for (int i = 0; i < NUMBER_MAX_OF_BALL; i++) {

        if(round->ball[i] != NULL)
        {
            position.x = (int)round->ball[i]->m_x;
            position.y = (int)round->ball[i]->m_y;
            position.h = 0;
            position.w = 0;
            SDL_BlitSurface(Arkanoid_AdvancedSprite, &round->ball[i]->m_src, surface, &position);

            /* -- CHECK COLLISION -- */
            if(round->ball[i]->isLaunch == true || G_NumberOfBall > 1)
            {
                SetBallPosition(round->ball[i],
                                (int)(round->ball[i]->m_x + round->ball[i]->m_vx / G_Delta_t),
                                (int)(round->ball[i]->m_y + round->ball[i]->m_vy / G_Delta_t)) ;


                position.x = (int)round->ball[i]->m_x;
                position.y = (int)round->ball[i]->m_y;
                SDL_BlitSurface(Arkanoid_AdvancedSprite, &round->ball[i]->m_src, surface, &position);

                // Define variable for simplify reading of code
                const int shipHeight = round->ship->m_src.h;
                const int shipWidth = round->ship->m_src.w;
                const int ballHeight = round->ball[i]->m_src.h;
                const int ballWidth = round->ball[i]->m_src.w;
                const double ballNextY = round->ball[i]->m_y + 2*(round->ball[i]->m_vy / G_Delta_t);
                const double ballNextX = round->ball[i]->m_x + 2*(round->ball[i]->m_vx / G_Delta_t);


                /* -- Check collision with Windows -- */
                if( ((ballNextY+ballHeight) > (MAX_HEIGH_OF_BOARD-MARGIN_PX)) || ((ballNextY) < (MIN_HEIGH_OF_BOARD+MARGIN_PX)) )
                {
                    printf("Colision Y \n");
                    round->ball[i]->m_vy *= -1;
                }
                else if( (ballNextX+ballWidth < MARGIN_PX) || (ballNextX+ballWidth > WIDTH-MARGIN_PX) )
                {
                    round->ball[i]->m_vx *= -1;
                }
                else if( ballNextY > round->ship->m_y+shipHeight )
                {
                    if(G_NumberOfBall > 1)
                    {
                        round->ball[i] = NULL;
                        G_NumberOfBall -= 1;
                        continue;
                    }
                    else{
                        View_ClearLife(surface);
                        G_Health -= 1;
                        if(G_Health <= 0){
                            printf("GameOver ! \n");
                            G_GameOver = true;
                            break;
                        }else{
                            round->ball[i]->isLaunch = false;
                            FixBallOnShip(round->ball[i], round->ship);
                            View_UpdateLife(surface);
                        }
                    }

                }

                /* -- Check collision with Ship -- */
                else if ( ((ballNextX+ballWidth) > round->ship->m_x) && (ballNextX < (round->ship->m_x+shipWidth) ) && (ballNextY+ballHeight > round->ship->m_y) )
                {

                    if(G_BonusActive != NULL && G_BonusActive->m_key == BONUS_CATCHFIRE && G_IsSticky)
                    {
                        round->ball[i]->isSticky = true;
                        round->ball[i]->m_y = round->ship->m_y-round->ship->m_src.h;
                        round->ball[i]->m_hookX = round->ball[i]->m_x - round->ship->m_x;
                        continue;
                    }else{
                    //if(G_IsSticky)
                    //{
                    //    continue;
                    //}
                    //else{
                        round->ball[i]->m_vy *= -1;

                        if( round->ship->m_dir > 0 ){
                            //round->ball[i]->m_vx = ((ballNextX-round->ship->m_x)/(shipWidth/2)) *5;
                            round->ball[i]->m_vx = (((round->ship->m_x+shipWidth-ballNextX)/(shipWidth/2)) *5);
                        }else{
                            round->ball[i]->m_vx = - (((round->ship->m_x+shipWidth-ballNextX)/(shipWidth/2)) *5);
                        }


                    }

                }

                /* -- Check collision with Bricks -- */
                else{
                    for (int index = 0;index < NUMBER_MAX_OF_BRICKS; index++)
                    {
                        Gui_Brick* brick = round->tab_bricks[index];
                        if(brick != NULL)
                        {
                            if( ((ballNextX <= brick->m_x+brick->m_src.w) && (ballNextX+ballWidth >= brick->m_x))
                                    && ( (ballNextY <= brick->m_y+brick->m_src.h) && (ballNextY+ballHeight >= brick->m_y)) )
                            {
                                if( (round->ball[i]->m_x+ballWidth > brick->m_x+brick->m_src.w || round->ball[i]->m_x < brick->m_x) ){
                                    round->ball[i]->m_vx *= -1;
                                }

                                if( (round->ball[i]->m_y+ballHeight > brick->m_y+brick->m_src.h || round->ball[i]->m_y < brick->m_y) ){
                                    round->ball[i]->m_vy *= -1;
                                }

                                // Shine bricks
                                if( brick->key == BRICK_GOLD || brick->key == BRICK_GRAY)
                                {
                                    brick->m_isShining = true;
                                    brick->m_indexShining = 1;
                                }

                                // Update brick's health
                                round->tab_bricks[index]->m_health += (brick->key == BRICK_GOLD) ? 0 : -1;

                                if(brick->m_health <= 0)
                                {
                                    G_Score += brick->score;

                                    // release bonus
                                    if( (brick->m_bonus != NULL) && (G_BonusDropping == NULL) && ( (G_BonusActive != NULL && G_BonusActive->m_key != BONUS_DIVIDE) || G_BonusActive == NULL)  )
                                    {
                                        G_BonusDropping = brick->m_bonus;
                                        G_BonusDropping->m_y = brick->m_y;
                                        G_BonusDropping->m_x = brick->m_x;
                                    }
                                    View_UpdateScore(surface);
                                    round->tab_bricks[index] = NULL;
                                }
                                break;
                            }
                        }
                    }
                }
            }
            else if(G_BonusActive != NULL && G_BonusActive->m_key == BONUS_CATCHFIRE && G_IsSticky)
            {
                SetBallPosition(round->ball[i],
                                (int)(round->ship->m_x + round->ball[i]->m_hookX),
                                (int)(round->ball[i]->m_y));
            }
            else
            {
                FixBallOnShip(round->ball[i], round->ship);
            }
        }
    }

    if(G_NumberOfBall <= 1 && G_BonusActive != NULL && G_BonusActive->m_key == BONUS_DIVIDE)
    {
        G_BonusActive = NULL;
    }
}
void Arkanoid_SaveYourScore(SDL_Window* window, SDL_Surface** surface)
{
    *surface = NULL;
    *surface = SDL_GetWindowSurface(window);
    if(surface == NULL)
    {
        SDL_Log("Erreur : La Surface n'a pas été récupérée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    enum MENU_ITEMS { NEW_GAME = 0, HIGH_SCORES = 1, ABOUT = 2, EXIT = 3 };
    char* TextAllowed = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    SDL_Rect MenuSelectorsPosition[NUMBER_TAG_NAME] = {
        {(int)(WIDTH_QUARTER)+00, HEIGHT_CENTER+32, 32, 32},
        {(int)(WIDTH_QUARTER)+32, HEIGHT_CENTER+32, 32, 32},
        {(int)(WIDTH_QUARTER)+64, HEIGHT_CENTER+32, 32, 32}
    };

    // Print title of surface

    static char txt_title[40];
    sprintf(txt_title,"%s%d%s", "Well Play ! You have : ", G_Score, " points");


    Arkanoid_PrintAlphaNumeric(*surface, txt_title,
                               WIDTH_QUARTER,
                               HEIGHT_QUARTER,
                               VIEW_SIZE_OF_SPACE_MEDIUM,
                               VIEW_SIZE_OF_CHAR_LOW);


    Arkanoid_PrintAlphaNumeric(*surface, "Entre your tag name : ",
                               WIDTH_QUARTER,
                               HEIGHT_CENTER,
                               VIEW_SIZE_OF_SPACE_MEDIUM,
                               VIEW_SIZE_OF_CHAR_LOW);


    // Display current selector
    Arkanoid_PrintAlphaNumeric(*surface, "_",
                               MenuSelectorsPosition[G_SelectorSelected].x,
                               MenuSelectorsPosition[G_SelectorSelected].y,
                               VIEW_SIZE_OF_SPACE_MEDIUM,
                               VIEW_SIZE_OF_CHAR_LOW);


    SDL_bool laucher = SDL_TRUE;

    // Events behaviors
    while(laucher)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                laucher = SDL_FALSE;
                WindowSelected = WINDOW_QUIT;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    laucher = SDL_FALSE;
                    WindowSelected = WINDOW_QUIT;
                    break;
                case SDLK_LEFT:
                    // Remove old selector


                    G_SelectorTagName = ((G_SelectorTagName-1) < 0) ? NUMBER_TAG_NAME-1 : (G_SelectorTagName-1);

                    Arkanoid_PrintAlphaNumeric(*surface, "_",
                                               MenuSelectorsPosition[G_SelectorTagName].x,
                                               MenuSelectorsPosition[G_SelectorTagName].y,
                                               VIEW_SIZE_OF_SPACE_MEDIUM,
                                               VIEW_SIZE_OF_CHAR_LOW);


                    break;
                case SDLK_RIGHT:


                    G_SelectorTagName = ((G_SelectorTagName+1) > NUMBER_TAG_NAME-1) ? 0 : (G_SelectorTagName+1);

                    Arkanoid_PrintAlphaNumeric(*surface, "_",
                                               MenuSelectorsPosition[G_SelectorTagName].x,
                                               MenuSelectorsPosition[G_SelectorTagName].y,
                                               VIEW_SIZE_OF_SPACE_MEDIUM,
                                               VIEW_SIZE_OF_CHAR_LOW);
                    break;
                case SDLK_UP:


                    Arkanoid_PrintAlphaNumeric(*surface, " ",
                                               MenuSelectorsPosition[G_SelectorTagName].x,
                                               MenuSelectorsPosition[G_SelectorTagName].y,
                                               VIEW_SIZE_OF_SPACE_MEDIUM,
                                               VIEW_SIZE_OF_CHAR_LOW);

                    // set new selected index
                    G_SelectedChar = ((G_SelectedChar-1) < 0) ? NUMBER_OF_ALPHABET-1 : (G_SelectedChar+1);

                    char view_char[1] = {TextAllowed[G_SelectedChar]};
                    sprintf(view_char, "%c", TextAllowed[G_SelectedChar]);

                    Arkanoid_PrintAlphaNumeric(*surface, view_char,
                                               MenuSelectorsPosition[G_SelectorTagName].x,
                                               MenuSelectorsPosition[G_SelectorTagName].y,
                                               VIEW_SIZE_OF_SPACE_MEDIUM,
                                               VIEW_SIZE_OF_CHAR_LOW);
                    break;
                case SDLK_DOWN:

                    Arkanoid_PrintAlphaNumeric(*surface, " ",
                                               MenuSelectorsPosition[G_SelectorTagName].x,
                                               MenuSelectorsPosition[G_SelectorTagName].y,
                                               VIEW_SIZE_OF_SPACE_MEDIUM,
                                               VIEW_SIZE_OF_CHAR_LOW);

                    G_SelectedChar = ((G_SelectedChar-1) > NUMBER_OF_ALPHABET-1) ? 0 :(G_SelectedChar+1);


                    char view_cha2r[1] = {TextAllowed[G_SelectedChar]};
                    sprintf(view_cha2r, "%c", TextAllowed[G_SelectedChar]);

                    Arkanoid_PrintAlphaNumeric(*surface, view_cha2r,
                                               MenuSelectorsPosition[G_SelectorTagName].x,
                                               MenuSelectorsPosition[G_SelectorTagName].y,
                                               VIEW_SIZE_OF_SPACE_MEDIUM,
                                               VIEW_SIZE_OF_CHAR_LOW);
                    break;
                case SDLK_RETURN:
                   // Save in file
                default:
                    break;
                }
                break;
            default:
                break;
            }
            SDL_UpdateWindowSurface(window);


        }
    }

Redirection_Quit:
    SDL_FillRect(*surface, NULL, 0x000000);
    SDL_FreeSurface(*surface);


}
bool WallIsEmpty(Round* round)
{
    int cpt = 0;

    for (int i = 0; i < NUMBER_MAX_OF_BRICKS; i++)
    {
        if((round->tab_bricks[i]) != NULL && round->tab_bricks[i]->key != BRICK_GOLD)
        {
            cpt++;
            break;
        }
    }
    return (cpt == 0);}

Ball* CreateBall(){
    Ball* ball = malloc(sizeof (Ball));
    initializeBall(ball);

    ball->m_src.x = 80;
    ball->m_src.y = 64;
    ball->m_src.w = 16;
    ball->m_src.h = 16;
    ball->m_x = 0;
    ball->m_y = 0;
    ball->m_vx = BALL_SPEED_X;
    ball->m_vy = BALL_SPEED_Y;
    ball->isLaunch = false;
    ball->isSticky = false;

    return ball;
}
Ship CreateShip()
{
    Ship ship;
    initializeShip(&ship);

    MediumShip(&ship);
    ship.m_x = 0;
    ship.m_y = 0;
    ship.m_vx = 4;
    ship.m_dir = 0;

    return ship;
}
Round CreateRound(Ship* s, Ball** b , int level)
{
    // Find level to load
    static char levelPath[40];
    if(level < 10){
        sprintf(levelPath,"%s%d%d%s", G_Level_path, 0, level, G_Level_path_extension);
    }else {
        sprintf(levelPath,"%s%d%s", G_Level_path, level, G_Level_path_extension);
    }

    Round round;
    initializeRound(&round, s, b, levelPath, level);

    return round;
}
SDL_Rect CreateRect(int x, int y, int w, int h){
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = h;
    rect.w = w;

    return rect;
}
Laser* CreateLaser(int x, int y)
{
    Laser* laser = malloc(sizeof(Laser));
    laser->m_src.h = 48;
    laser->m_src.w = 16;
    laser->m_src.x = 0;
    laser->m_src.y = 80;
    laser->m_x = x;
    laser->m_y = y;

    return laser;
}


void SetShipPosition(Ship* s, int x, int y)
{
    s->m_x = x;
    s->m_y = y;
}
void SetBallPosition(Ball* b, int x, int y)
{
    b->m_x = x;
    b->m_y = y;
}
void FixBallOnShip(Ball* ball, Ship* ship)
{
    SetBallPosition(ball, (int)(ship->m_x+(ship->m_src.w/2)-(ball->m_src.w/2)), (int)(ship->m_y-(ball->m_src.h)) );
}
void DisplayWallOfBricks(SDL_Surface* surface, Round* round, int x, int y)
{
    int row = 0;
    int column = 0;

    SDL_Rect destGuiBrique = {0,0,0,0};
    SDL_Rect destShadow = {0,0,0,0};

    for (int i = 0; i < NUMBER_MAX_OF_BRICKS; i++) {

        if( (round->tab_bricks[i]) != NULL)
        {
            row = (i / NUMBER_OF_COLUMN_IN_BRICKS);
            column = i % NUMBER_OF_COLUMN_IN_BRICKS;
            destGuiBrique.x = (column * 32) + x;
            destGuiBrique.y = (row * 16) + y;

            Gui_Brick* brick = (round->tab_bricks[i]);
            brick->m_x = destGuiBrique.x;
            brick->m_y = destGuiBrique.y;

            destShadow.x = destGuiBrique.x + 10;
            destShadow.y = destGuiBrique.y + 10;

            // shadow
            SDL_Surface* shadow = SDL_CreateRGBSurface(0,brick->m_src.w, brick->m_src.h,32,0,0,0,0);
            SDL_FillRect(shadow, NULL, SDL_MapRGB(shadow->format, 10,10,10));
            SDL_SetSurfaceBlendMode(shadow, SDL_BLENDMODE_BLEND);
            SDL_SetSurfaceAlphaMod(shadow,148);
            SDL_BlitSurface(shadow, NULL, surface, &destShadow);

            if(brick->m_isShining)
            {
                SDL_Rect shine_source = brick->m_src;
                shine_source.x += 32*brick->m_indexShining;
                brick->m_indexShining += 1;

                SDL_BlitSurface(Arkanoid_AdvancedSprite, &shine_source, surface, &destGuiBrique);

                if(brick->m_indexShining >= 5)
                {
                    brick->m_isShining = false;
                    brick->m_indexShining = 0;
                }
            }else {
                SDL_BlitSurface(Arkanoid_AdvancedSprite, &brick->m_src, surface, &destGuiBrique);
            }

            SDL_FreeSurface(shadow);
        }

    }
}


void Arkanoid_PrintAlphaNumeric(SDL_Surface* origin, const char* string, int x, int y, double space, double zoom)
{
    unsigned int index = 0;
    while(string[index] != '\0')
    {
        SDL_Rect sourceOfCharSprite = ConvertCharToAlphabetSprite(string[index]);
        SDL_Rect dest;
        dest.x = x + (int)(index*space*zoom);
        dest.y = y;
        dest.h = (int)(sourceOfCharSprite.h*zoom);
        dest.w = (int)(sourceOfCharSprite.w*zoom);

        SDL_BlitScaled(Arkanoid_AlphabetSprite, &sourceOfCharSprite, origin, &dest);
        index++;
    }
}
SDL_Rect ConvertCharToAlphabetSprite(const char c)
{
    int ascii = c;
    int y = (ascii <= SPRITE_CHAR_HEIGHT || ascii >= 127)? 0 : (int)(floor((ascii - SPRITE_CHAR_WIDHT) / 16)) * SPRITE_CHAR_WIDHT;
    int x = (ascii <= SPRITE_CHAR_WIDHT || ascii >= 127)? 0 : ((ascii - SPRITE_CHAR_WIDHT) % 16) * SPRITE_CHAR_WIDHT;

    SDL_Rect src = {x, y, SPRITE_CHAR_WIDHT, SPRITE_CHAR_HEIGHT};

    return src;
}
SDL_Rect* ConvertStringToAlphabetRects(const char* str, const uint8_t size)
{
    SDL_Rect* rect = malloc( sizeof(SDL_Rect) * size);

    uint8_t index = 0;
    while(str[index] != '\0')
    {
        SDL_Rect r = ConvertCharToAlphabetSprite(str[index]);
        rect[index] = r;
        index++;
    }

    return rect;
}
unsigned int GetNumberOfChar(const char string[])
{
    unsigned int number = 0;
    while(string[number] != '\0')
        number++;

    return number;
}

unsigned int GetNumberOfPX(const char string[], unsigned int sizeOfSpace, double sizeOfChar)
{
    return GetNumberOfChar(string)*(SPRITE_CHAR_WIDHT*sizeOfChar) + (GetNumberOfChar(string)-1)*(sizeOfSpace*sizeOfChar);
}

void View_UpdateLevel(SDL_Surface* surface)
{
    char level_parsed[5];
    sprintf(level_parsed, "%d", G_Level);
    Arkanoid_PrintAlphaNumeric(surface, "Niveau:",10, 10, VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_LOW);
    Arkanoid_PrintAlphaNumeric(surface, level_parsed,130,10, VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_LOW);

}
void View_UpdateScore(SDL_Surface* surface)
{
    char score_parsed[10];
    sprintf(score_parsed, "%d", G_Score);
    Arkanoid_PrintAlphaNumeric(surface, "Score:",180, 10, VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_LOW);
    Arkanoid_PrintAlphaNumeric(surface, score_parsed,280,10, VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_LOW);
}
void View_UpdateLife(SDL_Surface* surface)
{

    Arkanoid_PrintAlphaNumeric(surface, "Vie:",0,MAX_HEIGH_OF_BOARD+10, VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_LOW);
    for(int health_index = 0; health_index < G_Health; health_index++ )
    {
        SDL_Rect source = {384,128,64,16};
        SDL_Rect dest = {50+(int)(source.w*0.6*health_index), MAX_HEIGH_OF_BOARD+source.h, (int)(source.w*0.5), (int)(source.h*0.5)};
        SDL_BlitScaled(Arkanoid_AdvancedSprite,&source,surface,&dest);
    }
}
void View_ClearLife(SDL_Surface* surface)
{
    Arkanoid_PrintAlphaNumeric(surface, "Vie:",0,MAX_HEIGH_OF_BOARD+10, VIEW_SIZE_OF_SPACE_MEDIUM, VIEW_SIZE_OF_CHAR_LOW);
    for(int health_index = 0; health_index < G_Health; health_index++ )
    {
        SDL_Rect source = {448,128,64,16}; // cheat
        SDL_Rect dest = {50+(int)(source.w*0.6*health_index), MAX_HEIGH_OF_BOARD+source.h, (int)(source.w*0.5), (int)(source.h*0.5)};
        SDL_Surface* black = SDL_CreateRGBSurface(0,64,16,32,0,0,0,0);
        SDL_FillRect(black, NULL, SDL_MapRGB(black->format, 0,0,0));
        //SDL_SetSurfaceBlendMode(black, SDL_BLENDMODE_BLEND);

        SDL_BlitSurface(black, NULL, surface, &dest);
    }
}

void SmallShip(Ship* ship)
{
    ship->m_src.x = 384;
    ship->m_src.y = 128;
    ship->m_src.w = 64;
    ship->m_src.h = 16;
}
void MediumShip(Ship* ship)
{
    ship->m_src.x = 384;
    ship->m_src.y = 192;
    ship->m_src.w = 96;
    ship->m_src.h = 16;
}
void BigShip(Ship* ship){
    ship->m_src.x = 384;
    ship->m_src.y = 240;
    ship->m_src.w = 128;
    ship->m_src.h = 16;

}
