#pragma once

#include "Entity.hpp"
#include <SDL2/SDL.h>
#include <iostream>




class View {
public:
    // Le constructeur prend en paramètre les pointeurs vers les modèles (Pacman, ghost, labyrinthe..) et initialise juste la fenêtre sans rien dedans
    View(
        // Entity* pacman,
        // Entity* blinky,
        // Entity* pinky,
        // Entity* inky,
        // Entity* clyde
        );
    ~View();
    // void draw(void);
    SDL_Window *getWindow(void);
    void draw_beginning(void);
    SDL_Rect entity_scaled(SDL_Rect entity);
protected:
    // pointeurs vers les modèles (Pacman, ghost..)
    // On a pas de pointeurs pour le terrain car il n'est pas censé bouger
    // Entity* m_pacman;
    // Entity* m_blinky;
    // Entity* m_pinky;
    // Entity* m_inky;
    // Entity* m_clyde;

    // pointeurs vers les rectangles correspondant aux coordonnées des sprites
    SDL_Rect m_pacman_sprite;
    SDL_Rect m_blinky_sprite;
    SDL_Rect m_pinky_sprite;
    SDL_Rect m_inky_sprite;
    SDL_Rect m_clyde_sprite;
    SDL_Rect src_bg;
    SDL_Rect bg;

    // pointeurs vers les fenetres et rendu
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Surface* win_surf;
    SDL_Surface* plancheSprites;
    };
