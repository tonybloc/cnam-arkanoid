#include "./../headers/lib.h"


void Arkanoid_PrintAlphaNumeric(SDL_Surface* origin, SDL_Surface* alphaNumericSprite, const char* string, int x, int y, double zoom)
{
    unsigned int index = 0;
    while(string[index] != '\0')
    {
        SDL_Rect sourceOfCharSprite = ConvertCharToAlphabetSprite(string[index]);
        SDL_Rect dest;
        dest.x = x + (int)(index*16*zoom);
        dest.y = y;
        dest.h = (int)(sourceOfCharSprite.h*zoom);
        dest.w = (int)(sourceOfCharSprite.h*zoom);

        //SDL_BlitSurface(alphaNumericSprite, &sourceOfCharSprite, origin, &dest);
        SDL_BlitScaled(alphaNumericSprite, &sourceOfCharSprite, origin, &dest);
        index++;
    }
}

/* Alphabetics Sprites Behaviors*/
SDL_Rect ConvertCharToAlphabetSprite(const char c)
{
    const int heightOfChar = 32;
    const int widthOfChar = 32;

    int ascii = c;
    int y = (ascii <= heightOfChar || ascii >= 127)? 0 : (int)(floor((ascii - widthOfChar) / 16)) * widthOfChar;
    int x = (ascii <= widthOfChar || ascii >= 127)? 0 : ((ascii - widthOfChar) % 16) * widthOfChar;

    SDL_Rect src = {x, y, heightOfChar, widthOfChar};

    return src;
}
SDL_Rect* ConvertStringToAlphabetRects(const char* str, const uint8_t size){
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
unsigned int GetSizeOfStringAccordingAlphabetSprite(const char string[])
{
    unsigned int index = 0;
    while(string[index] != '\0')
        index++;

    return index*20;
}


SDL_Window* Arkanoid_LoadNewWindow(int height, int width)
{
    // Create new window
    SDL_Window* window = SDL_CreateWindow("Arknoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        SDL_Log("Error creation of new window : %s \n, Line : %d, File : %s, Function : %s \n", SDL_GetError(), __LINE__, __FILE__, __FUNCTION__);
        return NULL;
    }

    return window;
}

