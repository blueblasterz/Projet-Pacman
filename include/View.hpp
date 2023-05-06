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

    /*
    Permet de dessiner la fenêtre avec les sprites aux bonnes positions
    La fonction prend ainsi les coordonnées de chaque entités et les dessine à la bonne position
    */
    void draw(Entity *pacman, Entity *blinky, Entity *pinky, Entity *inky, Entity *clyde);
    
    SDL_Window *getWindow(void);

    /* 
    Fonction prototype de draw() avec les positions de base du début de partie
    */
    void draw_beginning();

    /*
    Fonction permettant de redimensionner les sprites pour les afficher en 3x plus grand.
    La fenêtre de jeu étant 3x plus grande que les dimensions du sprite sur le fichier BMP,
    la fonction permet de faire le lien entre les coordonnées du modèle et les coordonnées de la fenêtre.
    */
    SDL_Rect entity_scaled(SDL_Rect entity);

    /*
    Fonction permettant de remplacer la tuile de la case où se trouve le pacman par une tuile de la
    map où il n'y a rien (case vide)
    */
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

    // pointeurs vers les fenêtres et rendu
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Surface* win_surf;
    SDL_Surface* plancheSprites;
    };
