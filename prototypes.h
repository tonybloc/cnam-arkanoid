#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include "./headers/ball.h"
#include "./headers/ship.h"
#include "./headers/round.h"
#include "./headers/bonus.h"
#include "./headers/include.h"
#include "./headers/laser.h"

// Menus
void Arkanoid_ShowMenu(SDL_Window* win, SDL_Surface** rend);
void Arkanoid_ShowBoard(SDL_Window* win, SDL_Surface** rend);
void Arkanoid_ShowAbout(SDL_Window* window, SDL_Surface** surface);
void Arkanoid_ShowHighScores(SDL_Window* window, SDL_Surface** surface);
void Arkanoid_DrawBoard(SDL_Surface* surface, Round* round, Laser** lasers);
void Arkanoid_SaveYourScore(SDL_Window* window, SDL_Surface** surface);
bool WallIsEmpty(Round* round);

// Object creation
Ball* CreateBall(void);
Ship CreateShip(void);
SDL_Rect CreateRect(int x, int y, int w, int h);
Round CreateRound(Ship* s, Ball** b , int level);
Laser* CreateLaser(int x, int y);

// Position setters
void SetShipPosition(Ship* s, int x, int y);
void SetBallPosition(Ball* b, int x, int y);
void FixBallOnShip(Ball* b, Ship* s);

/* Update display */
void DisplayWallOfBricks(SDL_Surface* surface, Round* r, int x, int y);
// Update ball when Collision
bool CheckBallCollisionWithBrick(Ball* ball, Gui_Brick* brick);
bool CheckBallCollisionWithShip(Ball* ball, Ship* ship);
void CollisionBonus(SDL_Surface* surface, Bonus* bonusSelected, Bonus* bonusDropping, Ship* ship);
// String operation with Alphabet Sprite
void Arkanoid_PrintAlphaNumeric(SDL_Surface* origin, const char* string, int x, int y, double sizeOfSpace, double sizeOfChar);
SDL_Rect ConvertCharToAlphabetSprite(const char c);
SDL_Rect* ConvertStringToAlphabetRects(const char* str, const uint8_t size);
unsigned int GetNumberOfChar(const char string[]);
unsigned int GetNumberOfPX(const char string[], unsigned int sizeOfSpace, double sizeOfChar);

void View_UpdateLevel(SDL_Surface* surface);
void View_UpdateScore(SDL_Surface* surface);
void View_UpdateLife(SDL_Surface* surface);
void View_ClearLife(SDL_Surface* surface);

void SmallShip(Ship* ship);
void MediumShip(Ship* ship);
void BigShip(Ship* ship);

void SlowBall(Round* round);
void IncreaseBall(Round* round);
#endif
