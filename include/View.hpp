#pragma once

#include "Entity.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "Terrain.hpp"
#include "Pacman.hpp"
#include "Ghost.hpp"



class View {
public:
    // Le constructeur prend en paramètre les pointeurs vers les modèles (Pacman, ghost, labyrinthe..) et initialise juste la fenêtre sans rien dedans
    View(
        std::shared_ptr<Pacman> pacman,
        std::shared_ptr<Ghost> blinky,
        std::shared_ptr<Ghost> clyde,
        std::shared_ptr<Ghost> pinky,
        std::shared_ptr<Ghost> inky,
        std::shared_ptr<Terrain> terrain
        );
    ~View();

    /*
    Permet de dessiner la fenêtre avec les sprites aux bonnes positions
    La fonction prend ainsi les coordonnées de chaque entités et les dessine à la bonne position
    */
    void draw();
    
    SDL_Window*  getWindow(void) const;
    int get_rect_x (SDL_Rect rect) const;
    int get_rect_y (SDL_Rect rect) const;
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
    Fonction permettant de redimensionner les sprites pour les afficher en 3x plus grand.
    La fenêtre de jeu étant 3x plus grande que les dimensions du sprite sur le fichier BMP,
    la fonction permet de faire le lien entre les coordonnées du modèle et les coordonnées de la fenêtre.
    */
    SDL_Rect entity_scaled(SDL_Rect entity);

 
    /*
    Permet de modifier un rectangle et de le normaliser
    */
    void set_Rect_scaled(SDL_Rect *rect, double x, double y);

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
    Permet de changer le sprite de PacMan en fonction de sa direction
    */
    void change_sprite(SDL_Rect position_scaled);

    /*
    Permet de changer le sprite de Blinky en fonction de sa direction
    */
    void change_sprite_b(SDL_Rect position_scaled);

    /*
    Permet de changer le sprite de Pinky en fonction de sa direction
    */
    void change_sprite_p(SDL_Rect position_scaled);

    /*
    Permet de changer le sprite de Inky en fonction de sa direction
    */
    void change_sprite_i(SDL_Rect position_scaled);

    /*
    Permet de changer le sprite de Clyde en fonction de sa direction
    */
    void change_sprite_c(SDL_Rect position_scaled);


    /*
    Permet de changer le sprite des fantomes en mode "fright"
    Les sprites étant communs, il n'y a pas besoin de scinder les fonctions
    Rq : Cela vaut aussi pour les fonctions change_sprite_fright_end(), 
    change_sprite_fright(), change_sprite_eaten() et change_sprite_score().
    */
    void change_sprite_fright(Ghost* ghost);

    /*
    Permet de dessiner le nombre de vies restantes du pacman
    */
    void draw_lives();

 
protected:
    // pointeurs vers les modèles (Pacman, ghost..)
    // On a pas de pointeurs pour le terrain car il n'est pas censé bouger
    std::shared_ptr<Pacman> m_pacman;
    std::shared_ptr<Ghost> m_blinky;
    std::shared_ptr<Ghost> m_pinky;
    std::shared_ptr<Ghost> m_inky;
    std::shared_ptr<Ghost> m_clyde;

    // Terrain pour récupérer les coordonnées des pacgums mangés
    std::shared_ptr<Terrain> m_terrain;

    // On récupère les direction pour l'animation des sprites
    Direction::Direction m_pacman_direction;
    Direction::Direction m_blinky_direction;
    Direction::Direction m_pinky_direction;
    Direction::Direction m_inky_direction;
    Direction::Direction m_clyde_direction;

    int m_frame;
    int m_score;
    int m_lives;

    // entiers permettant de switcher les animations
    int sprite_pacman_animation; // int pour le type d'animation du pacman
    int sprite_blinky_animation; // int pour le type d'animation du blinky
    int sprite_pinky_animation;
    int sprite_inky_animation;
    int sprite_clyde_animation;

    // Sprite du terrain de jeu
    SDL_Rect src_bg;
    // Zone de jeu (toute la fenêtre en gros)
    SDL_Rect bg;

    // Case noire pour effacer les points
    SDL_Rect bg_pointless;
    SDL_Rect position_pointless;

    // Case noire pour effacer le score
    SDL_Rect black_bg_score;
    SDL_Rect position_score;

    // Case noire pour effacer les vies
    SDL_Rect black_bg_lives;
    SDL_Rect position_lives;

    // pointeurs vers les fenêtres et rendu
    // SDL_Window* m_window;
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
    SDL_Renderer* m_renderer;
    SDL_Surface* win_surf;
    SDL_Surface* plancheSprites;

    // chargement de tous les sprites des entités !

    SDL_Rect m_pacman_sprite_idle;
    SDL_Rect m_pacman_sprite_right_1;
    SDL_Rect m_pacman_sprite_right_2;
    SDL_Rect m_pacman_sprite_left_1;
    SDL_Rect m_pacman_sprite_left_2;
    SDL_Rect m_pacman_sprite_up_1;
    SDL_Rect m_pacman_sprite_up_2;
    SDL_Rect m_pacman_sprite_down_1;
    SDL_Rect m_pacman_sprite_down_2;

    SDL_Rect m_pacman_sprite_dead_1;
    SDL_Rect m_pacman_sprite_dead_2;
    SDL_Rect m_pacman_sprite_dead_3;
    SDL_Rect m_pacman_sprite_dead_4;
    SDL_Rect m_pacman_sprite_dead_5;
    SDL_Rect m_pacman_sprite_dead_6;
    SDL_Rect m_pacman_sprite_dead_7;
    SDL_Rect m_pacman_sprite_dead_8;
    SDL_Rect m_pacman_sprite_dead_9;
    SDL_Rect m_pacman_sprite_dead_10;
    SDL_Rect m_pacman_sprite_dead_11;

    SDL_Rect m_blinky_sprite_right_1;
    SDL_Rect m_blinky_sprite_right_2;
    SDL_Rect m_blinky_sprite_left_1;
    SDL_Rect m_blinky_sprite_left_2;
    SDL_Rect m_blinky_sprite_up_1;
    SDL_Rect m_blinky_sprite_up_2;
    SDL_Rect m_blinky_sprite_down_1;
    SDL_Rect m_blinky_sprite_down_2;

    SDL_Rect m_pinky_sprite_right_1;
    SDL_Rect m_pinky_sprite_right_2;
    SDL_Rect m_pinky_sprite_left_1;
    SDL_Rect m_pinky_sprite_left_2;
    SDL_Rect m_pinky_sprite_up_1;
    SDL_Rect m_pinky_sprite_up_2;
    SDL_Rect m_pinky_sprite_down_1;
    SDL_Rect m_pinky_sprite_down_2;

    SDL_Rect m_inky_sprite_right_1;
    SDL_Rect m_inky_sprite_right_2;
    SDL_Rect m_inky_sprite_left_1;
    SDL_Rect m_inky_sprite_left_2;
    SDL_Rect m_inky_sprite_up_1;
    SDL_Rect m_inky_sprite_up_2;
    SDL_Rect m_inky_sprite_down_1;
    SDL_Rect m_inky_sprite_down_2;

    SDL_Rect m_clyde_sprite_right_1;
    SDL_Rect m_clyde_sprite_right_2;
    SDL_Rect m_clyde_sprite_left_1;
    SDL_Rect m_clyde_sprite_left_2;
    SDL_Rect m_clyde_sprite_up_1;
    SDL_Rect m_clyde_sprite_up_2;
    SDL_Rect m_clyde_sprite_down_1;
    SDL_Rect m_clyde_sprite_down_2;

    SDL_Rect m_fright_sprite_1;
    SDL_Rect m_fright_sprite_2;
    SDL_Rect m_fright_sprite_3;
    SDL_Rect m_fright_sprite_4;

    SDL_Rect m_eaten_sprite_right;
    SDL_Rect m_eaten_sprite_left;
    SDL_Rect m_eaten_sprite_up;
    SDL_Rect m_eaten_sprite_down;

    SDL_Rect m_score_sprite_200;
    SDL_Rect m_score_sprite_400;
    SDL_Rect m_score_sprite_800;
    SDL_Rect m_score_sprite_1600;

    SDL_Rect m_number_sprite_0;


    };
