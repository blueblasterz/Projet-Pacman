#include "View.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include "Entity.hpp"

#define SCREEN_WIDTH   672
#define SCREEN_HEIGHT  864

View::View()
        // Entity* pacman,
        // Entity* blinky,
        // Entity* pinky,
        // Entity* inky,
        // Entity* clyde) 
        // : m_pacman(pacman),
        // m_blinky(blinky),
        // m_pinky(pinky),
        // m_inky(inky),
        // m_clyde(clyde),
        {
    // Initialisation du rendu et de la fenêtre
    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) // Allume le système graphique de SDL
    {
        std::cerr << "SDL n'a pas pu être initialisé :" << SDL_GetError() << std::endl;
        exit(1);
    }

    // Création de la fenêtre avec les paramètres par défaut de l'OS (et les dimensions qu'on a défini)
    m_window = SDL_CreateWindow("PacMan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    // On vérifie ensuite les valeurs renvoyées par la fenêtre SDL
    if (!m_window)
    {
        std::cerr << "La fenêtre " << SCREEN_WIDTH << "x" << SCREEN_HEIGHT << " n'a pas pu être ouverte : " << SDL_GetError() << std::endl;
        exit(1);
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);

    if (!m_renderer)
    {
        std::cerr << "Le rendu n'a pas pu être créé : " << SDL_GetError() << std::endl;
        exit(1);
    }

    win_surf = SDL_GetWindowSurface(m_window);

    plancheSprites = SDL_LoadBMP("./src/texture.bmp");

    if (plancheSprites == NULL)
    {
        std::cerr << "Impossible de charger le fichier de sprites : " << SDL_GetError() << std::endl;
        exit(1);
    }

    // On initialise maintenant tout ce qui est relatif à la map

    // Dimensions de la map et coordonnées de celle-ci dans la planche de sprites
    src_bg = { 1, 1, 224,248 }; // x,y, w,h (0,0) en haut a gauche
    bg = { 0,72, 672, 744 }; // ici scale x3 pour afficher le jeu

    // Maintenant on est censé pouvoir afficher la map de taille 227x250 sur une fenêtre de taille 908x1000

    // On initialise les coordonnées des sprites
    m_pacman_sprite = {716, 1, 16, 16};
    m_blinky_sprite = {685, 65, 16, 16};
    m_pinky_sprite = {685, 81, 16, 16};
    m_inky_sprite = {685, 97, 16, 16};
    m_clyde_sprite = {685, 113, 16, 16};
}
View::~View(){
    // On free les sprites 
    SDL_FreeSurface(plancheSprites);
    SDL_FreeSurface(win_surf);
    
    // On détruit le rendu et la fenêtre
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
}

SDL_Window *View::getWindow(){
    return m_window;
}



SDL_Rect View::entity_scaled(SDL_Rect entity){
    SDL_Rect scaled_entity = {entity.x*3, entity.y*3, entity.w*3, entity.h*3};
    return scaled_entity;
}
// Fonction de base pour afficher les sprites (réutilisable pour les autres fonctions)
void View::draw_beginning(){

    // On définit le noir comme non-transparent pour la planche de sprites
    SDL_SetColorKey(plancheSprites, false, 0);
    SDL_BlitScaled(plancheSprites, &src_bg, win_surf, &bg);

    // On définit le noir comme transparent lorsqu'on affiche les sprites
    SDL_SetColorKey(plancheSprites, true, 0);
    SDL_Rect pacman_case = {105, 181, 16, 16}; // Case de pacman
    SDL_Rect pacman_scaled = entity_scaled(pacman_case);
    SDL_BlitScaled(plancheSprites, &m_pacman_sprite, win_surf, &pacman_scaled);

    SDL_Rect blinky_case = {105, 86, 16, 16}; // Case de blinky
    SDL_Rect blinky_scaled = entity_scaled(blinky_case);
    SDL_BlitScaled(plancheSprites, &m_blinky_sprite, win_surf, &blinky_scaled);

    SDL_Rect pinky_case = {89, 109, 16, 16}; // Case de pinky
    SDL_Rect pinky_scaled = entity_scaled(pinky_case);
    SDL_BlitScaled(plancheSprites, &m_pinky_sprite, win_surf, &pinky_scaled);

    SDL_Rect inky_case = {105, 109, 16, 16}; // Case de inky
    SDL_Rect inky_scaled = entity_scaled(inky_case);
    SDL_BlitScaled(plancheSprites, &m_inky_sprite, win_surf, &inky_scaled);

    SDL_Rect clyde_case = {121, 109, 16, 16}; // Case de clyde
    SDL_Rect clyde_scaled = entity_scaled(clyde_case);
    SDL_BlitScaled(plancheSprites, &m_clyde_sprite, win_surf, &clyde_scaled);
    
}

void View::draw(Entity *pacman, Entity *blinky, Entity *pinky, Entity *inky, Entity *clyde){
    
    // On définit le noir comme non-transparent pour la planche de sprites
    SDL_SetColorKey(plancheSprites, false, 0);
    SDL_BlitScaled(plancheSprites, &src_bg, win_surf, &bg);

    // On définit le noir comme transparent lorsqu'on affiche les sprites
    SDL_SetColorKey(plancheSprites, true, 0);

    SDL_Rect pacman_case = {pacman->get_x(), pacman->get_y(), 16, 16}; // Case de pacman
    SDL_Rect pacman_scaled = entity_scaled(pacman_case);
    SDL_BlitScaled(plancheSprites, &m_pacman_sprite, win_surf, &pacman_scaled);

    SDL_Rect blinky_case = {blinky->get_x(), blinky->get_y(), 16, 16}; // Case de blinky
    SDL_Rect blinky_scaled = entity_scaled(blinky_case);
    SDL_BlitScaled(plancheSprites, &m_blinky_sprite, win_surf, &blinky_scaled);

    SDL_Rect pinky_case = {pinky->get_x(), pinky->get_y(), 16, 16}; // Case de pinky
    SDL_Rect pinky_scaled = entity_scaled(pinky_case);
    SDL_BlitScaled(plancheSprites, &m_pinky_sprite, win_surf, &pinky_scaled);

    SDL_Rect inky_case = {inky->get_x(), inky->get_y(), 16, 16}; // Case de inky
    SDL_Rect inky_scaled = entity_scaled(inky_case);
    SDL_BlitScaled(plancheSprites, &m_inky_sprite, win_surf, &inky_scaled);

    SDL_Rect clyde_case = {clyde->get_x(), clyde->get_y(), 16, 16}; // Case de clyde
    SDL_Rect clyde_scaled = entity_scaled(clyde_case);
    SDL_BlitScaled(plancheSprites, &m_clyde_sprite, win_surf, &clyde_scaled);

}