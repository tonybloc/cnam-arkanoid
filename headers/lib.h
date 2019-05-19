#ifndef LIB_H
#define LIB_H

#include "./include.h"


void Arkanoid_PrintAlphaNumeric(SDL_Surface* origin, SDL_Surface* alphaNumericSprite, const char* string, int x, int y, double zoom);


SDL_Rect ConvertCharToAlphabetSprite(const char c);
SDL_Rect* ConvertStringToAlphabetRects(const char* str, const uint8_t size);
unsigned int GetSizeOfStringAccordingAlphabetSprite(const char string[]);

SDL_Window* Arkanoid_LoadNewWindow(int height, int width);


#endif // LIB_H
