#include <iostream>
#include "View.hpp"
#include <SDL2/SDL.h>

int main(void){

    // Entity *pacman = new Entity();
    // Entity *blinky = new Entity();
    // Entity *pinky = new Entity();
    // Entity *inky = new Entity();
    // Entity *clyde = new Entity();

    View *view = new View();
    SDL_Window *window = view->getWindow();

    // BOUCLE PRINCIPALE (partie à mettre dans le controller)
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
			default: break;
			}
		}

        // Gestion du clavier        
        int nbk;
        const Uint8* keys = SDL_GetKeyboardState(&nbk);
        if (keys[SDL_SCANCODE_ESCAPE])
            quit = true;
        if (keys[SDL_SCANCODE_LEFT])
            puts("LEFT");
        if (keys[SDL_SCANCODE_RIGHT])
            puts("RIGHT");

        // AFFICHAGE
        view->draw_beginning();
        SDL_UpdateWindowSurface(window);

        // LIMITE A 60 FPS
		SDL_Delay(16); // utiliser SDL_GetTicks64() pour plus de precisions
	}
    SDL_Quit(); // ON SORT
    return 0;

}