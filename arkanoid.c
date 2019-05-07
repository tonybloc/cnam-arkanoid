#include "./headers/lib.h"
#include "./headers/ball.h"
#include "./headers/ship.h"
#include "./headers/round.h"

#define NUMBER_MAX_OF_BRICKS 260
#define NUMBER_OF_COLUMN_IN_BRICKS 13
#define NUMBER_MAX_ROW_IN_BRICKS 20

#define HEIGHT 600
#define WIDTH 600
#define HEIGHT_CENTER HEIGHT/2
#define WIDTH_CENTER WIDTH/2
#define HEIGHT_QUARTER HEIGHT/4
#define WIDTH_QUARTER WIDTH/4
#define ALLOWED_PX 5

#define BALL_SPEED_X 10
#define BALL_SPEED_Y 10

#define BRICK_GRAY 12
#define BRICK_GOLD 13
#define BRICK_SHINING 6

#define NUMBER_OF_MENU 4

//struct { double x; double y; } ball_speed;

/* Constant */

/* Enumeration */
enum ARKANOID_WINDOW_OPTION { WINDOW_MENU = 1, WINDOW_BOARD = 2, WINDOW_QUIT = 3, WINDOW_SCORES = 4, WINDOW_ABOUT = 5};

/* Global Variable */
static Ball ball;           // Ball
static Ship ship;           // Ship
static Round r;
static int Score = 0;
static Uint64 prev, now;    // timers
static double delta_t;      // refresh frame (ms)
static double ball_nextX, ball_nextY = 0;
static Gui_Brick* brick;
static enum ARKANOID_WINDOW_OPTION WindowBehavior = WINDOW_MENU;
static double prev_vault_x = 0;
//static Gui_Brick BrickShining[]

/* Arkanoid Window */
static SDL_Window* Arkanoid_Window = NULL;
static SDL_Surface* Arkanoid_Surface = NULL;

/* Background */
static SDL_Rect rectBackground = { 0,128, 48,64 };  // x,y, w,h (0,0) en haut a gauche




/* Prototype */
void Arkanoid_ShowMenu(SDL_Window* win, SDL_Surface** rend);
void Arkanoid_ShowBoard(SDL_Window* win, SDL_Surface** rend);
void Arkanoid_ShowAbout(SDL_Window* window, SDL_Surface** surface);
void Arkanoid_ShowHighScores(SDL_Window* window, SDL_Surface** surface);
void Arkanoid_DrawBoard(SDL_Surface* surface, SDL_Surface* advancedSprites);


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

    while(WindowBehavior != WINDOW_QUIT)
    {
        switch(WindowBehavior)
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
    SDL_FreeSurface(Arkanoid_Surface);
    SDL_DestroyWindow(Arkanoid_Window);
    SDL_Quit();

    return EXIT_SUCCESS;
}


/* Windows behaviors */
void Arkanoid_ShowMenu(SDL_Window* window, SDL_Surface** surface)
{
    *surface = NULL;
    SDL_Surface* advancedSprite = NULL;
    SDL_Surface* alphabetSprite = NULL;

    *surface = SDL_GetWindowSurface(window);
    if(surface == NULL)
    {
        SDL_Log("Erreur : La Surface n'a pas été récupérée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    // Load sprites
    alphabetSprite = SDL_LoadBMP("./public/Arkanoid_ascii.bmp");
    advancedSprite = SDL_LoadBMP("./public/Arkanoid_sprites.bmp");

    if(advancedSprite == NULL || alphabetSprite == NULL)
    {
        SDL_Log("Erreur : Le sprite n'a pas été chargée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    // Transparancy
    SDL_SetColorKey(alphabetSprite, true, 0);
    SDL_SetColorKey(advancedSprite, true, 0);


    enum MENU_ITEMS { NEW_GAME = 0, HIGH_SCORES = 1, ABOUT = 2, EXIT = 3 };

    char* MenuLabels[NUMBER_OF_MENU] = {"New Game", "High Score", "About", "Exit"};

    SDL_Rect MenuPositionOfSelectors[NUMBER_OF_MENU] = {
        {(int)(WIDTH_QUARTER*1.5)-32, WIDTH_QUARTER+00, 32, 32},
        {(int)(WIDTH_QUARTER*1.5)-32, WIDTH_QUARTER+32, 32, 32},
        {(int)(WIDTH_QUARTER*1.5)-32, WIDTH_QUARTER+64, 32, 32},
        {(int)(WIDTH_QUARTER*1.5)-32, WIDTH_QUARTER+96, 32, 32}
    };

    // initialize Menu surfaces selectors
    SDL_Surface* MenuSurfaceOfSelectors[NUMBER_OF_MENU] = {NULL};
    for (int i = 0; i < NUMBER_OF_MENU; i++)
        MenuSurfaceOfSelectors[i] = SDL_CreateRGBSurface(0, 32, 32, 32,0,0,0,0);


    // Print title of surface
    Arkanoid_PrintAlphaNumeric(*surface, alphabetSprite, "Arkanoid",(int)(WIDTH_QUARTER*1.5),20);


    for(int index = 0; index < NUMBER_OF_MENU; index++)
    {
        // Initialize position of selectors
        Arkanoid_PrintAlphaNumeric(MenuSurfaceOfSelectors[index], alphabetSprite, " ", 0,0);
        SDL_BlitSurface(MenuSurfaceOfSelectors[index], NULL, *surface, &(MenuPositionOfSelectors[index]));

        // Print menus's labels
        Arkanoid_PrintAlphaNumeric(*surface, alphabetSprite, MenuLabels[index], (int)(WIDTH_QUARTER*1.5), HEIGHT_QUARTER+(index*32));
    }

    int indexSelected = 0;

    // Print current selectors
    Arkanoid_PrintAlphaNumeric(MenuSurfaceOfSelectors[indexSelected], alphabetSprite, "*", 0,0);
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
                    WindowBehavior = WINDOW_QUIT;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            laucher = SDL_FALSE;
                            WindowBehavior = WINDOW_QUIT;
                            break;
                        case SDLK_UP:
                            // remove current selector
                            SDL_FillRect(MenuSurfaceOfSelectors[indexSelected], NULL, 0x000000);
                            SDL_BlitSurface(MenuSurfaceOfSelectors[indexSelected], NULL, *surface, &(MenuPositionOfSelectors[indexSelected]));

                            // set new selected index
                            indexSelected = ((indexSelected-1) < 0) ? NUMBER_OF_MENU-1 :(indexSelected-1);

                            // print new current selector
                            Arkanoid_PrintAlphaNumeric(MenuSurfaceOfSelectors[indexSelected], alphabetSprite, "*", 0,0);
                            SDL_BlitSurface(MenuSurfaceOfSelectors[indexSelected], NULL, *surface, &(MenuPositionOfSelectors[indexSelected]));
                            break;
                        case SDLK_DOWN:
                            // remove current selector
                            SDL_FillRect(MenuSurfaceOfSelectors[indexSelected], NULL, 0x000000);
                            SDL_BlitSurface(MenuSurfaceOfSelectors[indexSelected], NULL, *surface, &(MenuPositionOfSelectors[indexSelected]));

                            // set new selected index
                            indexSelected = ((indexSelected+1) > NUMBER_OF_MENU-1) ? 0 :(indexSelected+1);

                            // print next current selector
                            Arkanoid_PrintAlphaNumeric(MenuSurfaceOfSelectors[indexSelected], alphabetSprite, "*", 0,0);
                            SDL_BlitSurface(MenuSurfaceOfSelectors[indexSelected], NULL, *surface, &(MenuPositionOfSelectors[indexSelected]));
                            break;
                        case SDLK_RETURN:
                            switch (indexSelected)
                            {
                                case NEW_GAME:
                                    WindowBehavior = WINDOW_BOARD;
                                    laucher = SDL_FALSE;
                                    break;
                                case HIGH_SCORES:
                                    WindowBehavior = WINDOW_SCORES;
                                    laucher = SDL_FALSE;
                                    break;
                                case ABOUT:
                                    WindowBehavior = WINDOW_ABOUT;
                                    laucher = SDL_FALSE;
                                    break;
                                case EXIT:
                                    WindowBehavior = WINDOW_QUIT;
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
    SDL_FreeSurface(alphabetSprite);
    SDL_FreeSurface(advancedSprite);

}
void Arkanoid_ShowBoard(SDL_Window* window, SDL_Surface** surface)
{
    *surface = NULL;
    SDL_Surface* advancedSprite = NULL;
    SDL_Surface* alphabetSprite = NULL;

    // define new surface for window
    *surface = SDL_GetWindowSurface(window);
    if(surface == NULL)
    {
        SDL_Log("Erreur : La Surface n'a pas été récupérée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }


    // Load sprites
    alphabetSprite = SDL_LoadBMP("./public/Arkanoid_ascii.bmp");
    advancedSprite = SDL_LoadBMP("./public/Arkanoid_sprites.bmp");

    if(advancedSprite == NULL || alphabetSprite == NULL)
    {
        SDL_Log("Erreur : Le sprite n'a pas été chargée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    // Transparancy
    SDL_SetColorKey(alphabetSprite, true, 0);
    SDL_SetColorKey(advancedSprite, true, 0);


    initializeBall(&ball);
    initializeShip(&ship);
    initializeRound(&r, &ship, &ball, "./public/mur.txt");


    ship.m_src.x = 384;
    ship.m_src.y = 128;
    ship.m_src.w = 64;
    ship.m_src.h = 16;
    ship.m_x = ((*surface)->w/2) - (ship.m_src.w/2);
    ship.m_y = (*surface)->h - 32;
    ship.m_vx = 4;
    ship.m_dir = 0;

    ball.m_src.x = 80;
    ball.m_src.y = 64;
    ball.m_src.w = 16;
    ball.m_src.h = 16;
    ball.m_x = ship.m_x + (ship.m_src.w/2) - (ball.m_src.w/2);
    ball.m_y = ship.m_y - (ball.m_src.h);
    ball.m_vx = BALL_SPEED_X;
    ball.m_vy = BALL_SPEED_Y;



    /*
    for(int index =0; index < NUMBER_MAX_OF_BRICKS; index ++)
    {
        if((r.tab_bricks[index]) != NULL)
        {
            Gui_Brick* brick = r.tab_bricks[index];
            printf("brick (%d): x%d  y%d  h%d  w%d \n", brick->key, brick->m_src.x, brick->m_src.y, brick->m_src.h, brick->m_src.w);
        }
        else {
            printf("NULL \n");
        }
    }
    */

    // Events

    SDL_bool gameLaucher = SDL_TRUE;


    while(gameLaucher)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ship.m_dir = 1;
            switch(event.type)
            {
                case SDL_QUIT:
                    gameLaucher = SDL_FALSE;
                    break;
                case SDL_MOUSEMOTION:
                    ship.m_x += event.motion.xrel;
                    ship.m_dir = (event.motion.xrel > 0) ? 1 : -1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_LEFT:
                            if( (ship.m_x) >= 1 )
                            {
                                printf("position -x : %d  \n",(int)(prev_vault_x-ship.m_x));
                                ship.m_x -= 10 ;
                                ship.m_dir = -1;
                                prev_vault_x = ship.m_x;
                            }
                            break;
                        case SDLK_RIGHT:
                            if( (ship.m_x + ship.m_src.w) < (*surface)->w )
                            {

                                printf("position +x: %d  \n",(int)(ship.m_x-prev_vault_x));
                                ship.m_x += 10;
                                ship.m_dir = 1;
                                prev_vault_x = ship.m_x;
                            }
                            break;
                        case SDLK_ESCAPE:
                            gameLaucher = SDL_FALSE;
                            WindowBehavior = WINDOW_MENU;
                            break;
                        case SDLK_SPACE :
                            ball.isLaunch = 1;
                            break;
                        default: break;
                    }
                    break;
                default:
                    break;
            }
        }
        prev = now;
        now = SDL_GetPerformanceCounter();
        delta_t = (double)( (now-prev) * 1000 / SDL_GetPerformanceFrequency());
        Arkanoid_DrawBoard(*surface, advancedSprite);

        SDL_UpdateWindowSurface(window);
    }

    Redirection_Quit:
    // free memory of round
    for (int index = 0; index < NUMBER_MAX_OF_BRICKS;index++) {
        free(r.tab_bricks[index]);
    }
    free(r.tab_bricks);

    SDL_FillRect(*surface, NULL, 0x000000);
    SDL_FreeSurface(*surface);
    SDL_FreeSurface(alphabetSprite);
    SDL_FreeSurface(advancedSprite);
}
void Arkanoid_ShowAbout(SDL_Window* window, SDL_Surface** surface)
{
    *surface = NULL;
    SDL_Surface* advancedSprite = NULL;
    SDL_Surface* alphabetSprite = NULL;

    // define new surface for window
    *surface = SDL_GetWindowSurface(window);
    if(surface == NULL){
        SDL_Log("Erreur : La Surface n'a pas été récupérée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    // Load sprites
    alphabetSprite = SDL_LoadBMP("./public/Arkanoid_ascii.bmp");
    advancedSprite = SDL_LoadBMP("./public/Arkanoid_sprites.bmp");

    if(advancedSprite == NULL || alphabetSprite == NULL)
    {
        SDL_Log("Erreur : Le sprite n'a pas été chargée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    // Transparancy
    SDL_SetColorKey(alphabetSprite, true, 0);
    SDL_SetColorKey(advancedSprite, true, 0);

    Arkanoid_PrintAlphaNumeric(*surface, alphabetSprite, "About us",(int)(WIDTH_QUARTER*1.5),20);

    Arkanoid_PrintAlphaNumeric(*surface, alphabetSprite, "This application was designed by :",10,HEIGHT_QUARTER+32);
    Arkanoid_PrintAlphaNumeric(*surface, alphabetSprite, "- Carole TRESER",10,HEIGHT_QUARTER+64);
    Arkanoid_PrintAlphaNumeric(*surface, alphabetSprite, "- Anthony Mochel",10,HEIGHT_QUARTER+96);
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
                            WindowBehavior = WINDOW_MENU;
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
    SDL_FreeSurface(alphabetSprite);
    SDL_FreeSurface(advancedSprite);
}
void Arkanoid_ShowHighScores(SDL_Window* window, SDL_Surface** surface)
{
    *surface = NULL;
    SDL_Surface* advancedSprite = NULL;
    SDL_Surface* alphabetSprite = NULL;

    // define new surface for window
    *surface = SDL_GetWindowSurface(window);
    if(surface == NULL){
        SDL_Log("Erreur : La Surface n'a pas été récupérée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    // Load sprites
    alphabetSprite = SDL_LoadBMP("./public/Arkanoid_ascii.bmp");
    advancedSprite = SDL_LoadBMP("./public/Arkanoid_sprites.bmp");

    if(advancedSprite == NULL || alphabetSprite == NULL)
    {
        SDL_Log("Erreur : Le sprite n'a pas été chargée : %s \n", SDL_GetError());
        SDL_Log("Line : %d, File : %s, Function : %s \n", __LINE__, __FILE__, __FUNCTION__);
        goto Redirection_Quit ;
    }

    // Transparancy
    SDL_SetColorKey(alphabetSprite, true, 0);
    SDL_SetColorKey(advancedSprite, true, 0);


    Arkanoid_PrintAlphaNumeric(*surface, alphabetSprite, "Hight Scores",(int)(WIDTH_QUARTER*1.5),20);

    Arkanoid_PrintAlphaNumeric(*surface, alphabetSprite, "No scores yet !",10,HEIGHT_QUARTER+32);
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
                            WindowBehavior = WINDOW_MENU;
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
    SDL_FreeSurface(alphabetSprite);
    SDL_FreeSurface(advancedSprite);
}
void Arkanoid_DrawBoard(SDL_Surface* surface, SDL_Surface* advancedSprites)
{

    // Fill surface background
    SDL_Rect position = { 0.0, 0.0, 0.0, 0.0 };
    for (int j = 0; j < surface->h; j+=64)
    {
        for (int i = 0; i < surface->w; i += 48)
        {
            position.x = i;
            position.y = j;
            SDL_BlitSurface(advancedSprites, &rectBackground, surface, &position);
        }
    }

    // display wall
    int row = 0;
    int column = 0;
    SDL_Rect destGuiBrique = {0,0,0,0};
    SDL_Rect destShadow = {0,0,0,0};
    for (int i = 0; i < NUMBER_MAX_OF_BRICKS; i++) {

        if( (r.tab_bricks[i]) != NULL)
        {
            row = i / NUMBER_OF_COLUMN_IN_BRICKS;
            column = i % NUMBER_OF_COLUMN_IN_BRICKS;
            destGuiBrique.x = column * 32;
            destGuiBrique.y = row * 16;

            Gui_Brick* brick = (r.tab_bricks[i]);
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
                r.tab_bricks[i]->m_indexShining += 1;

                SDL_BlitSurface(advancedSprites, &shine_source, surface, &destGuiBrique);

                if(r.tab_bricks[i]->m_indexShining >= 5)
                {
                    r.tab_bricks[i]->m_isShining = false;
                    r.tab_bricks[i]->m_indexShining = 0;
                }
            }else {
                SDL_BlitSurface(advancedSprites, &(r.tab_bricks[i])->m_src, surface, &destGuiBrique);
            }

            SDL_FreeSurface(shadow);
        }

    }


    // display ball
    SDL_Rect ballPosition = { (int)(ball.m_x), (int)(ball.m_y), 0, 0 };

    SDL_Surface* ball_shadow = SDL_CreateRGBSurface(0, ball.m_src.w, ball.m_src.h,32,0,0,0,0);
    SDL_FillRect(ball_shadow, NULL, SDL_MapRGB(ball_shadow->format, 255,0,0));
    SDL_SetSurfaceBlendMode(ball_shadow, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceColorMod(ball_shadow,255,0,0);

    SDL_BlitSurface(advancedSprites, &ball.m_src, ball_shadow, &ballPosition);



    SDL_BlitSurface(ball_shadow, NULL, surface, &ballPosition);
    //SDL_BlitSurface(advancedSprites, &ball.m_src, surface, &ballPosition);


    // Define ball position
    if(ball.isLaunch == true)
    {
        ball.m_x += ball.m_vx / delta_t;
        ball.m_y += ball.m_vy / delta_t;

        ball_nextX = ball.m_x + (ball.m_vx / delta_t);
        ball_nextY = ball.m_y + (ball.m_vy / delta_t);


        // ball collision
        if ((ball_nextY < ALLOWED_PX) || (ball_nextY+ball.m_src.h > (surface->h)-ALLOWED_PX))
        {
            ball.m_vy *= -1;
        }
        else if ((ball_nextX < ALLOWED_PX) || (ball_nextX+ball.m_src.w > (surface->w)-ALLOWED_PX))
        {
            ball.m_vx *= -1;
        }
        else if ((ball_nextX+ball.m_src.w >= ship.m_x) && (ball_nextX <= ship.m_x + ship.m_src.w)
                 && (ball_nextY+ball.m_src.h >= ship.m_y))
        {
            ball.m_vy *= -1;
            ball.m_vx = (ball_nextX - ship.m_x)/(ship.m_src.w/2)*5;
        }
        else
        {
            for (int index = 0;index < NUMBER_MAX_OF_BRICKS; index++)
            {
                Gui_Brick* brick = r.tab_bricks[index];
                if(brick != NULL)
                {
                    if( (ball_nextX < brick->m_x+brick->m_src.w && ball_nextX+ball.m_src.w > brick->m_x)
                            && (ball_nextY < brick->m_y+brick->m_src.h && ball_nextY+ball.m_src.h > brick->m_y))
                    {
                        if( (ball.m_x > brick->m_x+brick->m_src.w || ball.m_x < brick->m_x) )
                            ball.m_vx *= -1;


                        if( (ball.m_y > brick->m_y+brick->m_src.h || ball.m_y < brick->m_y) )
                            ball.m_vy *= -1;


                        brick->m_health += -1;

                        if(brick->m_health <= 0)
                            r.tab_bricks[index] = NULL;

                        if(brick->key == BRICK_GOLD || brick->key == BRICK_GRAY)
                        {
                            r.tab_bricks[index]->m_isShining = true;
                            r.tab_bricks[index]->m_indexShining = 1;
                        }
                        break;
                    }

                }
            }
        }
    }
    else
    {
        ball.m_x = (int)(ship.m_x + (ship.m_src.w/2) - (ball.m_src.w/2));
        ball.m_y = (int)(ship.m_y - (ball.m_src.h));
    }

    //printf("direction vault : vx%d vy%d \n ", (int)ship.m_vx, (int)ship.m_dir);
    position.x = (int) ship.m_x;
    position.y = (int) ship.m_y;

    SDL_BlitSurface(advancedSprites, &ship.m_src, surface, &position);
}
