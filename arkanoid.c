#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./headers/ball.h"
#include "./headers/ship.h"

//struct { double x; double y; } ball_speed;
Ball ball;
Ship ship;

Uint64 prev, now; // timers
double delta_t;  // durée frame en ms
int x_vault;

SDL_Window* pWindow = NULL;
SDL_Surface* win_surf = NULL;
SDL_Surface* default_sprites = NULL; // default sprite
SDL_Surface* sprite = NULL; // all sprite
SDL_Surface* surfAlphabet = NULL;

//SDL_Rect srcBg = { 0,128, 96,128 }; // x,y, w,h (0,0) en haut a gauche
SDL_Rect srcBg = { 64,128, 64,64 }; // x,y, w,h (0,0) en haut a gauche
SDL_Rect srcBall = { 0,96,24,24 };
SDL_Rect scrVaiss = { 128,0,128,32 };



void init()
{
    // initialize window
    pWindow = SDL_CreateWindow("Arknoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);

	win_surf = SDL_GetWindowSurface(pWindow);

    // load sprites
    default_sprites = SDL_LoadBMP("./public/sprites.bmp");
    sprite = SDL_LoadBMP("./public/Arkanoid_sprites.bmp");
    //surfAlphabet = SDL_LoadBMP("./public/Arkanoid_ascii.bmp");

    SDL_SetColorKey(default_sprites, true, 0);  // 0: 00/00/00 noir -> transparent

    initializeBall(&ball);
    initializeShip(&ship);


    ball.m_x = win_surf->w / 2;
    ball.m_y = win_surf->h - 25;
    ball.m_vx = 1.0;
    ball.m_vy = 1.4;
    ball.m_src.x = 0;
    ball.m_src.y = 96;
    ball.m_src.w = 24;
    ball.m_src.h = 24;

    x_vault = 0;

    ship.m_x = x_vault;
    ship.m_y = win_surf->h - 32;
    ship.m_vx = 0;
    ship.m_vy = 0;
    ship.m_src.x = 128;
    ship.m_src.y = 0;
    ship.m_src.w = 128;
    ship.m_src.h = 32;
}


// fonction qui met à jour la surface de la fenetre "win_surf"
void draw()
{
	// remplit le fond 
	SDL_Rect dest = { 0,0,0,0 };
    for (int j = 0; j < win_surf->h; j+=64)
        for (int i = 0; i < win_surf->w; i += 64)
        {
			dest.x = i;
			dest.y = j;
            if(SDL_BlitSurface(sprite, &srcBg, win_surf, &dest) != 0)
            {
                // display error on console
                fprintf(stderr, "Erreur lors de l'affichage du background : %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
            }

		}

	
	// affiche balle
    SDL_Rect dstBall = {ball.m_x, ball.m_y, 0, 0};
    if(SDL_BlitSurface(default_sprites, &srcBall, win_surf, &dstBall) != 0)
    {
        // display error on console
        fprintf(stderr, "Erreur lors de l'affichage de la ball : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

	// dedplacement
    ball.m_x += ball.m_vx / delta_t;
    ball.m_y += ball.m_vy / delta_t;

	// collision bord
    if ((ball.m_x < 1) || (ball.m_x > (win_surf->w - 25)))
        ball.m_vx *= -1;
    if ((ball.m_y < 1) || (ball.m_y > (win_surf->h - 25)) || (ball.m_y < (scrVaiss.y - 25)))
        ball.m_vy *= -1;


	// touche bas -> rouge
    if (ball.m_y >(win_surf->h - 25))
		srcBall.y = 64;
	// touche bas -> vert
    if (ball.m_y < 1)
		srcBall.y = 96;

	// vaisseau
    dest.x = ship.m_x;
    dest.y = ship.m_y;
    if(SDL_BlitSurface(default_sprites, &scrVaiss, win_surf, &dest) != 0)
    {
        // display error on console
        fprintf(stderr, "Erreur lors de l'affichage du vaisseaux : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}



int main(int argc, char** argv)
{
    // initialize SDL : video
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        // display error on console
        fprintf(stderr, "Erreur lors de l'initialisation de la SDL : %s\n", SDL_GetError());

        // exit application
        exit(EXIT_FAILURE);
    }

	init();
    
	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		while (!quit && SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					// touche clavier
                    case SDLK_LEFT:  ship.m_x -= 10; break;
                    case SDLK_RIGHT: ship.m_x +=10; break;
					case SDLK_ESCAPE: quit = true; break;
					default: break;
				}
				break;
            case SDL_MOUSEMOTION:	ship.m_x += event.motion.xrel;	break;
			case SDL_MOUSEBUTTONDOWN:
				printf("mouse click %d\n", event.button.button);
				break;
			default: break;
			}
		}
		prev = now;
		now = SDL_GetPerformanceCounter();
		delta_t = (double)((now - prev) * 1000 / (double)SDL_GetPerformanceFrequency());
		draw();

        // Update surface for display sprites
        if(SDL_UpdateWindowSurface(pWindow) != 0)
        {
            // display error on console
            fprintf(stderr, "Erreur lors de l'update de la surface : %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

	}
    SDL_FreeSurface(win_surf);
    SDL_FreeSurface(default_sprites);
    SDL_FreeSurface(sprite);
    SDL_FreeSurface(surfAlphabet);


    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return EXIT_SUCCESS;
}
