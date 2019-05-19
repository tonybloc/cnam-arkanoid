#include "./headers/lib.h"
#include "./headers/ball.h"
#include "./headers/ship.h"
#include "./headers/round.h"
#include "./headers/include.h"
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

#define ALLOWED_PX 5

#define BALL_SPEED_X 10
#define BALL_SPEED_Y 10

#define BRICK_GRAY 12
#define BRICK_GOLD 13
#define BRICK_SHINING_ITERATION 6

#define NUMBER_OF_MENU 4
#define NUMBER_OF_BACKGROUD 6
#define MIN_HEIGH_OF_BOARD (HEIGHT/15)
#define MAX_HEIGH_OF_BOARD (HEIGHT/15)*14


/* Enumeration */
enum ARKANOID_WINDOW_OPTION { WINDOW_MENU = 1, WINDOW_BOARD = 2, WINDOW_QUIT = 3, WINDOW_SCORES = 4, WINDOW_ABOUT = 5};

/* Global Variable */
static int level = 31;
static int Score = 0;
static int health = 3;

static Uint64 prev, now;    // timers
static double delta_t;      // refresh frame (ms)

static double ball_nextX, ball_nextY = 0;

static enum ARKANOID_WINDOW_OPTION WindowSelected = WINDOW_MENU;

static char* path = "./public/level_";
static char* extension = ".txt";

/* Arkanoid Window */
static SDL_Window* Arkanoid_Window = NULL;
static SDL_Surface* Arkanoid_Surface = NULL;

static SDL_Surface* Arkanoid_AdvancedSprite = NULL;
static SDL_Surface* Arkanoid_AlphabetSprite = NULL;

/* Background */
static SDL_Rect rectBackgrounds[NUMBER_OF_BACKGROUD] = {
    {0,128,48,64},
    {64,128,64,64},
    {128,128,64,64},
    {192,128,64,64},
    {256,128,64,64},
    {320,128,64,64},

};  // x,y, w,h (0,0) en haut a gauche


/* Prototype */
void Arkanoid_ShowMenu(SDL_Window* win, SDL_Surface** rend);
void Arkanoid_ShowBoard(SDL_Window* win, SDL_Surface** rend);
void Arkanoid_ShowAbout(SDL_Window* window, SDL_Surface** surface);
void Arkanoid_ShowHighScores(SDL_Window* window, SDL_Surface** surface);
void Arkanoid_DrawBoard(SDL_Surface* surface, Round* round);

bool WallIsEmpty(Round* round);

Ball CreateBall(void);
Ship CreateShip(void);
SDL_Rect CreateRect(int x, int y, int w, int h);
Round CreateRound(Ship* s, Ball* b , int level);

void SetShipPosition(Ship* s, int x, int y);
void SetBallPosition(Ball* b, int x, int y);
void FixBallOnShip(Ball* b, Ship* s);
void DisplayWallOfBricks(SDL_Surface* surface, Round* r, int x, int y);

bool CheckBallCollisionWithWindow(Ball* ball);
bool CheckBallCollisionWithBrick(Ball* ball, Gui_Brick* brick);
bool CheckBallCollisionWithShip(Ball* ball, Ship* ship);

/* Main */
int main(int argc, char* argv[])
{
    // initialize SDL : video, audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0 )
    {
        SDL_Log("Erreur : La SDL n'a pas été initialisée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    // Load Window
    Arkanoid_Window = Arkanoid_LoadNewWindow(HEIGHT, WIDTH);
    if(Arkanoid_Window == NULL)
    {
        SDL_Log("Erreur : La Fenêtre n'a pas été chargée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }


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


/* Windows behaviors */
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
    SDL_SetColorKey(Arkanoid_AlphabetSprite, true, 0);
    SDL_SetColorKey(Arkanoid_AdvancedSprite, true, 0);


    enum MENU_ITEMS { NEW_GAME = 0, HIGH_SCORES = 1, ABOUT = 2, EXIT = 3 };
    char* MenuLabels[NUMBER_OF_MENU] = {"New Game", "High Score", "About", "Exit"};

    SDL_Rect MenuPositionOfSelectors[NUMBER_OF_MENU] = {
        {(int)(WIDTH_QUARTER)-32, HEIGHT_CENTER+00, 32, 32},
        {(int)(WIDTH_QUARTER)-32, HEIGHT_CENTER+32, 32, 32},
        {(int)(WIDTH_QUARTER)-32, HEIGHT_CENTER+64, 32, 32},
        {(int)(WIDTH_QUARTER)-32, HEIGHT_CENTER+96, 32, 32}
    };

    // initialize Menu surfaces selectors
    SDL_Surface* MenuSurfaceOfSelectors[NUMBER_OF_MENU] = {NULL};
    for (int i = 0; i < NUMBER_OF_MENU; i++)
        MenuSurfaceOfSelectors[i] = SDL_CreateRGBSurface(0, 32, 32, 32,0,0,0,0);


    // Print title of surface
    Arkanoid_PrintAlphaNumeric(*surface, Arkanoid_AlphabetSprite, "Arkanoid",(int)(WIDTH_QUARTER),HEIGHT_QUARTER,1);


    for(int index = 0; index < NUMBER_OF_MENU; index++)
    {
        // Initialize position of selectors
        Arkanoid_PrintAlphaNumeric(MenuSurfaceOfSelectors[index], Arkanoid_AlphabetSprite, " ", 0,0,1);
        SDL_BlitSurface(MenuSurfaceOfSelectors[index], NULL, *surface, &(MenuPositionOfSelectors[index]));

        // Print menus's labels
        Arkanoid_PrintAlphaNumeric(*surface, Arkanoid_AlphabetSprite, MenuLabels[index], (int)(WIDTH_QUARTER), HEIGHT_CENTER+(index*32),1);
    }

    int indexSelected = 0;

    // Print current selectors
    Arkanoid_PrintAlphaNumeric(MenuSurfaceOfSelectors[indexSelected], Arkanoid_AlphabetSprite, "*", 0,0,1);
    SDL_BlitSurface(MenuSurfaceOfSelectors[indexSelected], NULL, *surface, &(MenuPositionOfSelectors[indexSelected]));

    SDL_UpdateWindowSurface(window);


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
                    // remove current selector
                    SDL_FillRect(MenuSurfaceOfSelectors[indexSelected], NULL, 0x000000);
                    SDL_BlitSurface(MenuSurfaceOfSelectors[indexSelected], NULL, *surface, &(MenuPositionOfSelectors[indexSelected]));

                    // set new selected index
                    indexSelected = ((indexSelected-1) < 0) ? NUMBER_OF_MENU-1 :(indexSelected-1);

                    // print new current selector
                    Arkanoid_PrintAlphaNumeric(MenuSurfaceOfSelectors[indexSelected], Arkanoid_AlphabetSprite, "*", 0,0,1);
                    SDL_BlitSurface(MenuSurfaceOfSelectors[indexSelected], NULL, *surface, &(MenuPositionOfSelectors[indexSelected]));
                    break;
                case SDLK_DOWN:
                    // remove current selector
                    SDL_FillRect(MenuSurfaceOfSelectors[indexSelected], NULL, 0x000000);
                    SDL_BlitSurface(MenuSurfaceOfSelectors[indexSelected], NULL, *surface, &(MenuPositionOfSelectors[indexSelected]));

                    // set new selected index
                    indexSelected = ((indexSelected+1) > NUMBER_OF_MENU-1) ? 0 :(indexSelected+1);

                    // print next current selector
                    Arkanoid_PrintAlphaNumeric(MenuSurfaceOfSelectors[indexSelected], Arkanoid_AlphabetSprite, "*", 0,0,1);
                    SDL_BlitSurface(MenuSurfaceOfSelectors[indexSelected], NULL, *surface, &(MenuPositionOfSelectors[indexSelected]));
                    break;
                case SDLK_RETURN:
                    switch (indexSelected)
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
    SDL_SetColorKey(Arkanoid_AlphabetSprite, true, 0);
    SDL_SetColorKey(Arkanoid_AdvancedSprite, true, 0);

    Ball ball = CreateBall();
    Ship ship = CreateShip();
    Round round = CreateRound(&ship, &ball, level);

    SetShipPosition(&ship, (WIDTH_CENTER)-(round.ship->m_src.w/2), (MAX_HEIGH_OF_BOARD-round.ship->m_src.h-32) );
    FixBallOnShip(&ball,&ship);


    // Events
    SDL_bool gameIsLauched = SDL_TRUE;

    while(gameIsLauched)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ship.m_dir = 1;
            switch(event.type)
            {
            case SDL_QUIT:
                gameIsLauched = SDL_FALSE;
                break;
            case SDL_MOUSEMOTION:
                round.ship->m_x += event.motion.xrel;
                round.ship->m_dir = (event.motion.xrel > 0) ? 1 : -1;
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
                    round.ball->isLaunch = 1;
                    break;
                default: break;
                }
                break;

            }
        }
        prev = now;
        now = SDL_GetPerformanceCounter();
        delta_t = (double)( (now-prev) * 1000 / SDL_GetPerformanceFrequency());

        if (!WallIsEmpty(&round)){
            Arkanoid_DrawBoard(*surface, &round);
            SDL_UpdateWindowSurface(window);
        }
        else {
            if (level < 36){
                level++;
                break;
            }
        }
    }

Redirection_Quit:
    // free memory of round
    for (int index = 0; index < NUMBER_MAX_OF_BRICKS;index++) {
        free(round.tab_bricks[index]);
    }
    free(round.tab_bricks);
    free(round.ball);
    free(round.ship);

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
    SDL_SetColorKey(Arkanoid_AlphabetSprite, true, 0);
    SDL_SetColorKey(Arkanoid_AdvancedSprite, true, 0);

    Arkanoid_PrintAlphaNumeric(*surface, Arkanoid_AlphabetSprite, "About us",(int)(WIDTH_QUARTER*1.5),20,1);

    Arkanoid_PrintAlphaNumeric(*surface, Arkanoid_AlphabetSprite, "This application was designed by :",10,HEIGHT_QUARTER+32,0.5);
    Arkanoid_PrintAlphaNumeric(*surface, Arkanoid_AlphabetSprite, "- Anthony Mochel",10,HEIGHT_QUARTER+64, 0.5);
    Arkanoid_PrintAlphaNumeric(*surface, Arkanoid_AlphabetSprite, "- Carole Treser",10,HEIGHT_QUARTER+96, 0.5);

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
    SDL_SetColorKey(Arkanoid_AlphabetSprite, true, 0);
    SDL_SetColorKey(Arkanoid_AdvancedSprite, true, 0);


    Arkanoid_PrintAlphaNumeric(*surface, Arkanoid_AlphabetSprite, "Hight Scores",(int)(WIDTH_QUARTER*1.5),20,1);

    Arkanoid_PrintAlphaNumeric(*surface, Arkanoid_AlphabetSprite, "No scores yet !",10,HEIGHT_QUARTER+32,0.5);
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
void Arkanoid_DrawBoard(SDL_Surface* surface, Round* round)
{

    /* -- DISPLAY BACKGROUND -- */

    // Fill surface background of the window in black
    SDL_FillRect(surface, NULL, 0x00000000);

    // Fill surface background of the game only
    SDL_Rect position ={ 0.0, 0.0, 0.0, 0.0 };

    for (int y = MIN_HEIGH_OF_BOARD ; y < MAX_HEIGH_OF_BOARD ; y+=64)
    {
        for (int x = 0; x < WIDTH; x += ((level-1)%NUMBER_OF_BACKGROUD==0)?48:64)
        {
            SDL_Rect source = rectBackgrounds[(level-1)%NUMBER_OF_BACKGROUD];
            position.x = x;
            position.y = y;
            source.h = (MAX_HEIGH_OF_BOARD-y >= 64) ? 64 : (MAX_HEIGH_OF_BOARD-y);
            source.w = ((level-1)%NUMBER_OF_BACKGROUD==0)?48:64;
            SDL_BlitSurface(Arkanoid_AdvancedSprite, &source, surface, &position);
        }
    }


    /* -- DISPLAY GAME INDICATORS -- */

    // Show level and score
    char level_parsed[5];
    char score_parsed[10];
    sprintf(score_parsed, "%d", Score);
    sprintf(level_parsed, "%d", level);

    Arkanoid_PrintAlphaNumeric(surface, Arkanoid_AlphabetSprite, "Niveau:",10, 10, 0.5);
    Arkanoid_PrintAlphaNumeric(surface, Arkanoid_AlphabetSprite, level_parsed,130,10, 0.5);
    Arkanoid_PrintAlphaNumeric(surface, Arkanoid_AlphabetSprite, "Score:",180, 10, 0.5);
    Arkanoid_PrintAlphaNumeric(surface, Arkanoid_AlphabetSprite, score_parsed,280,10, 0.5);
    Arkanoid_PrintAlphaNumeric(surface, Arkanoid_AlphabetSprite, "Vie:",0,MAX_HEIGH_OF_BOARD+10, 0.5);

    for(int health_index = 0; health_index < health; health_index++ )
    {
        SDL_Rect source = round->ship->m_src;
        SDL_Rect dest = {50+(int)(source.w*0.6*health_index), MAX_HEIGH_OF_BOARD+source.h, (int)(source.w*0.5), (int)(source.h*0.5)};
        SDL_BlitScaled(Arkanoid_AdvancedSprite,&source,surface,&dest);
    }

    /* -- DISPLAY BRICKS WALL -- */
    DisplayWallOfBricks(surface, round, 0, MIN_HEIGH_OF_BOARD );


    /* -- DISPLAY BALL -- */
    position.x = (int)round->ball->m_x;
    position.y = (int)round->ball->m_y;
    SDL_BlitSurface(Arkanoid_AdvancedSprite, &round->ball->m_src, surface, &position);

    /* -- DISPLAY SHIP -- */
    position.x = (int)round->ship->m_x;
    position.y = (int)round->ship->m_y;
    SDL_BlitSurface(Arkanoid_AdvancedSprite, &round->ship->m_src, surface, &position);

    /* -- CHECK COLLISION -- */

    // Define ball position
    if(round->ball->isLaunch == true)
    {
        SetBallPosition(round->ball,
                        (int)(round->ball->m_x + round->ball->m_vx / delta_t),
                        (int)(round->ball->m_y + round->ball->m_vy / delta_t)) ;

        ball_nextX = round->ball->m_x + (round->ball->m_vx / delta_t);
        ball_nextY = round->ball->m_y + (round->ball->m_vy / delta_t);

        // Check collisions
        if(CheckBallCollisionWithWindow(round->ball)){}
        else if(CheckBallCollisionWithShip(round->ball, round->ship)){}
        else{
            for (int index = 0;index < NUMBER_MAX_OF_BRICKS; index++)
                if(CheckBallCollisionWithBrick(round->ball, round->tab_bricks[index])) break;
        }
    }
    else
    {
        FixBallOnShip(round->ball, round->ship);
    }
}

bool WallIsEmpty(Round* round)
{
    bool empty = false;
    int cpt =0;

    for (int i = 0; i < NUMBER_MAX_OF_BRICKS; i++)
    {
        if((round->tab_bricks[i]) == NULL)
        {
            cpt++;
        }
    }

    if (cpt == NUMBER_MAX_OF_BRICKS)
        empty = true;

    return empty;
}

Ball CreateBall(){
    Ball ball;
    initializeBall(&ball);

    ball.m_src.x = 80;
    ball.m_src.y = 64;
    ball.m_src.w = 16;
    ball.m_src.h = 16;
    ball.m_x = 0;
    ball.m_y = 0;
    ball.m_vx = BALL_SPEED_X;
    ball.m_vy = BALL_SPEED_Y;

    return ball;
}
Ship CreateShip()
{
    Ship ship;
    initializeShip(&ship);

    ship.m_src.x = 384;
    ship.m_src.y = 128;
    ship.m_src.w = 64;
    ship.m_src.h = 16;
    ship.m_x = 0;
    ship.m_y = 0;
    ship.m_vx = 4;
    ship.m_dir = 0;

    return ship;
}
Round CreateRound(Ship* s, Ball* b , int level)
{
    // Find level to load
    static char levelPath[40];
    if(level < 10){
        sprintf(levelPath,"%s%d%d%s",path,0, level, extension);
    }else {
        sprintf(levelPath,"%s%d%s",path, level, extension);
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


bool CheckBallCollisionWithWindow(Ball* ball)
{
    if(ball != NULL)
    {
        if ( (ball_nextY+ball->m_src.h) > (MAX_HEIGH_OF_BOARD-ALLOWED_PX)
             || (ball_nextY+ball->m_src.h) < (MIN_HEIGH_OF_BOARD+ALLOWED_PX) )
        {
            ball->m_vy *= -1;
            return true;
        }
        else if ( (ball_nextX < ALLOWED_PX) || (ball_nextX+ball->m_src.w > WIDTH-ALLOWED_PX))
        {
            ball->m_vx *= -1;
            return true;
        }
    }
    return false;
}


bool CheckBallCollisionWithBrick(Ball* ball, Gui_Brick* brick)
{
    if(brick != NULL && ball != NULL)
    {
        if( (ball_nextX < brick->m_x+brick->m_src.w && ball_nextX+ball->m_src.w > brick->m_x)
                && (ball_nextY < brick->m_y+brick->m_src.h && ball_nextY+ball->m_src.h > brick->m_y))
        {
            if( (ball->m_x > brick->m_x+brick->m_src.w || ball->m_x < brick->m_x) ){
                ball->m_vx *= -1;
            }

            if( (ball->m_y > brick->m_y+brick->m_src.h || ball->m_y < brick->m_y) ){
                ball->m_vy *= -1;
            }

            if( brick->key == BRICK_GOLD ||brick->key == BRICK_GRAY)
            {
                brick->m_isShining = true;
                brick->m_indexShining = 1;
            }

            brick->m_health += -1;

            if(brick->m_health <= 0){
                if (brick->key != BRICK_GOLD){
                    Score += brick->score;
                    brick = NULL;
                }
            }
            return true;
        }
    }
    return false;
}
//bool CheckShipCollisionWithBonus(Bonus*, Ship*);
bool CheckBallCollisionWithShip(Ball* ball, Ship* ship)
{
    if(ball != NULL && ship != NULL)
    {
        if ( ((ball_nextX+ship->m_src.w) >= ship->m_x) && (ball_nextX <= (ship->m_x+ship->m_src.w) )
                 && (ball_nextY+ball->m_src.h >= ship->m_y))
        {
            ball->m_vy *= -1;
            ball->m_vx = (ship->m_dir > 0) ? ((ball_nextX - ship->m_x)/(ship->m_src.w/2)*5) : -((ship->m_x+ship->m_src.w -ball_nextX)/(ship->m_src.w/2)*5);
            return true;
        }
    }
    return false;
}
// Notes sur le fontionnement générale des SDL_Surface et SDL_Rect :

// on vas définir notre nouvelle SDL_Rect (brick) à positionner sur la SDL_Rect "center"
//SDL_Rect src_brick = {96,0,32,16};

// On défini ensuite la position de notre brick sur la fenetre (en loccurance la SDL_Rect du centre)
//SDL_Rect position_brick_on_window = {center.x,center.y +10,src_brick.h,src_brick.w};

// En faite la position (la destination de notre birck) sera banaliser par les coordonnée du SDL_Rect "center"
// On pourras donc afficher notre brick sur la SDL_Rect "center" avec :
//SDL_BlitSurface(advancedSprites, &src_brick, surface,&position_brick_on_window );

// Il faut voir ça comme des couche de SDL_Rect
// Couche 1 : c'est notre surface de base (fenêtre de l'application = '*surface = SDL_GetWindowSurface(window);' )
// Couche 2 : se sera toutes les SDL_Rect qu'on vas positionnée sur la fenêtre

// Ce n'est que de la supérposition de SDL_Rect. C'est comme de la peinture
// (1)Couche blance : couche de base.
// (2)Puis on recouvre cette couche par une autre couleur.
// (3)Puis on recouvre cette nouvelle couche avec une nouvelle couleur sur une zone précise.

// Au final Seul la dernière couche (3) sera visiblee. Tout les zone que la couche (3) ne recouvre pas c'est la couche (2) que l'on veras.
// Tout se que la couche (2) et (3) ne recouvre pas, c'est la couche 1 qui sera visible.

// Les SDL_Rect ne peuvent être placé que sur des surfaces

// Il faut vraiment voir ça comme des couche de peinture.

// SDL_Blitsurface n'intevient en fait qu'en dernier pour permettre l'affichage de ces différentes couhes ? oui c'est ça
// On peut surperposé autant de couches de SDL_Rect que l'on veut, il faut juste terminer par l'afficher, un sorte de "vernis"

// C'est comme pour le background. on l'affiche en premier pour afficher le reste par dessus
