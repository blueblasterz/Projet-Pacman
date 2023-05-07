#pragma once

#include "Entity.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "Terrain.hpp"



class View {
public:
    // Le constructeur prend en paramètre les pointeurs vers les modèles (Pacman, ghost, labyrinthe..) et initialise juste la fenêtre sans rien dedans
    View(
        Entity* pacman,
        Entity* blinky,
        Entity* pinky,
        Entity* inky,
        Entity* clyde,
        Terrain* terrain
        );
    ~View();

    /*
    Permet de dessiner la fenêtre avec les sprites aux bonnes positions
    La fonction prend ainsi les coordonnées de chaque entités et les dessine à la bonne position
    */
    void draw();
    
    SDL_Window *getWindow(void);

    /*
    Fonction permettant de modifier les valeurs des SDL_Rect pour modifier les sprites notamment
    */
    void set_rect(SDL_Rect *rect, int x, int y);

    /*
    Permet de modifier le score du joueur
    */
    void set_score(int score);

    /*
    Permet de modifier le nombre de vies
    */
    void set_lives(int lives);

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
    map où il n'y a rien (case vide). Cela permet ainsi de faire disparaitre le point au centre d'une tuile.
    */
    void erase_point(std::vector<std::pair<int,int>> points);

    /*
    Fonction permettant de dessiner la valeur du score en haut de la fenêtre de jeu.
    */
    void draw_score();

    /*
    Permet de changer le sprite des entités en fonction de leur direction
    */
    void change_sprite();

    /*
    Permet de dessiner le nombre de vies restantes du pacman
    */
    // void draw_lives(int lives);

    /*
    Permet de dessiner le score du joueur
    */
    // void draw_score(int score);

protected:
    // pointeurs vers les modèles (Pacman, ghost..)
    // On a pas de pointeurs pour le terrain car il n'est pas censé bouger
    Entity* m_pacman;
    Entity* m_blinky;
    Entity* m_pinky;
    Entity* m_inky;
    Entity* m_clyde;


    // Terrain pour récupérer les coordonnées des pacgums mangés
    Terrain *m_terrain;
    

    // On récupère les direction pour l'animation des sprites
    Direction::Direction m_pacman_direction;
    Direction::Direction m_blinky_direction;
    Direction::Direction m_pinky_direction;
    Direction::Direction m_inky_direction;
    Direction::Direction m_clyde_direction;

    int m_frame;
    int m_score;
    int m_lives;

    // pointeurs vers les rectangles correspondant aux coordonnées des sprites
    SDL_Rect m_pacman_sprite;
    int sprite_pacman_animation; // int pour le type d'animation du pacman
    SDL_Rect m_blinky_sprite;
    int sprite_blinky_animation; // int pour le type d'animation du blinky
    SDL_Rect m_pinky_sprite;
    int sprite_pinky_animation;
    SDL_Rect m_inky_sprite;
    int sprite_inky_animation;
    SDL_Rect m_clyde_sprite;
    int sprite_clyde_animation;
    // Sprite du terrain de jeu
    SDL_Rect src_bg;
    // Zone de jeu (toute la fenêtre en gros)
    SDL_Rect bg;
    // Case noire pour effacer les points
    SDL_Rect bg_pointless;
    // Case noire pour effacer le score
    SDL_Rect black_bg_score;
    // Case noire pour effacer les vies
    SDL_Rect black_bg_lives;

    // pointeurs vers les fenêtres et rendu
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Surface* win_surf;
    SDL_Surface* plancheSprites;
    SDL_Surface* background;
    };
