#include "View.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include "Entity.hpp"
#include <vector>

#define SCREEN_WIDTH   672
#define SCREEN_HEIGHT  864


View::View(
        Entity* pacman,
        Entity* blinky,
        Entity* pinky,
        Entity* inky,
        Entity* clyde,
        Terrain* terrain)
        : m_pacman(pacman),
        m_blinky(blinky),
        m_pinky(pinky),
        m_inky(inky),
        m_clyde(clyde),
        m_terrain(terrain)
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
    // SDL_Surface* background = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 250, 250, 0);
    
    // On initialise maintenant tout ce qui est relatif à la map

    // Dimensions de la map et coordonnées de celle-ci dans la planche de sprites
    src_bg = { 1, 1, 224,248 }; // x,y, w,h (0,0) en haut a gauche
    bg = { 0,72, 672, 744 }; // ici scale x3 pour afficher le jeu
    black_bg = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    // Maintenant on est censé pouvoir afficher la map de taille 227x250 sur une fenêtre de taille 908x1000

    // On initialise les coordonnées des sprites
    m_pacman_sprite = {716, 1, 16, 16};
    m_blinky_sprite = {685, 65, 16, 16};
    m_pinky_sprite = {685, 81, 16, 16};
    m_inky_sprite = {685, 97, 16, 16};
    m_clyde_sprite = {685, 113, 16, 16};
    bg_pointless = {474, 327, 16, 16}; // Petite case noire de 16x16 pixels

    // points = {
    //     {1, 4},
    //     {2, 4},
    //     {3, 4},
    //     {4, 4},
    //     {16, 17}
    // };

    // Affichage score (partie immobile donc initialisée ici):

    SDL_Rect S_sprite = {254,264, 7, 7};
    SDL_Rect C_sprite = {246,256, 7, 7};
    SDL_Rect O_sprite = {342,256, 7, 7};
    SDL_Rect R_sprite = {246,264, 7, 7};
    SDL_Rect E_sprite = {263,256, 7, 7}; 
    SDL_Rect tiret = {320,275, 7, 7};
    SDL_Rect score_scaled = {96*2, 4*2, 7*2, 7*2};
    SDL_BlitScaled(plancheSprites, &S_sprite, win_surf, &score_scaled);
    score_scaled.x += 8*2;
    SDL_BlitScaled(plancheSprites, &C_sprite, win_surf, &score_scaled);
    score_scaled.x += 8*2;
    SDL_BlitScaled(plancheSprites, &O_sprite, win_surf, &score_scaled);
    score_scaled.x += 8*2;
    SDL_BlitScaled(plancheSprites, &R_sprite, win_surf, &score_scaled);
    score_scaled.x += 8*2;
    SDL_BlitScaled(plancheSprites, &E_sprite, win_surf, &score_scaled);
    score_scaled.x += 8*3;
    score_scaled.y += 8;
    SDL_BlitScaled(plancheSprites, &tiret, win_surf, &score_scaled);

    // Affichage des vies (partie immobile donc initialisée ici): (à faire)

    // On récupère les directions pour l'animation des sprites
    m_pacman_direction = m_pacman->get_direction();
    m_blinky_direction = m_blinky->get_direction();
    m_pinky_direction = m_pinky->get_direction();
    m_inky_direction = m_inky->get_direction();
    m_clyde_direction = m_clyde->get_direction();
    // Et on initialise les types de sprites à 1 :
    sprite_pacman_animation = 1;
    sprite_blinky_animation = 1;
    sprite_pinky_animation = 1;
    sprite_inky_animation = 1;
    sprite_clyde_animation = 1;

    frame = 0;

}
View::~View(){
    // On free les sprites 
    SDL_FreeSurface(plancheSprites);
    SDL_FreeSurface(win_surf);
    SDL_FreeSurface(background);

    // On détruit le rendu et la fenêtre
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
}

SDL_Window *View::getWindow(){
    return m_window;
}

void View::set_rect(SDL_Rect *rect, int x, int y){
    rect->x = x;
    rect->y = y;
}

SDL_Rect View::entity_scaled(SDL_Rect entity){
    SDL_Rect scaled_entity = {entity.x*3 - 24, entity.y*3 - 24, entity.w*3, entity.h*3};
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

void View::erase_point(std::vector<std::pair<int,int>> points){
    // SDL_SetColorKey(plancheSprites, false, 0);
    // SDL_BlitScaled(plancheSprites, &src_bg, win_surf, &bg);
    // SDL_SetColorKey(plancheSprites, true, 0);
    for (auto point : points){
        SDL_Rect point_case = {point.first*8 + 8, point.second*8 + 8, 8, 8}; // Case de point
        SDL_Rect point_scaled = entity_scaled(point_case);
        SDL_BlitScaled(plancheSprites, &bg_pointless, win_surf, &point_scaled);
    }
    SDL_SetColorKey(plancheSprites, false, 0);

}

void View::draw(){
    change_sprite();
    // On définit le noir comme non-transparent pour la planche de sprites
    SDL_SetColorKey(plancheSprites, false, 0);
    // SDL_SetColorKey(background, false, 0);
    // On remplit la fenêtre de noir pour éviter les bugs d'affichage (on ne peut pas afficher de score sinon)
    // SDL_FillRect(win_surf, &black_bg, SDL_MapRGB(win_surf->format, 0, 0, 0));
    SDL_BlitScaled(plancheSprites, &src_bg, win_surf, &bg);
    std::vector<std::pair<int,int>> points = m_terrain->get_eaten(); 
    erase_point(points);
    
    // On définit le noir comme transparent lorsqu'on affiche les sprites
    
    SDL_SetColorKey(plancheSprites, true, 0);

    SDL_Rect pacman_case = {m_pacman->get_x(), m_pacman->get_y(), 16, 16}; // Case de pacman
    SDL_Rect pacman_scaled = entity_scaled(pacman_case);
    SDL_BlitScaled(plancheSprites, &m_pacman_sprite, win_surf, &pacman_scaled);

    SDL_Rect blinky_case = {m_blinky->get_x(), m_blinky->get_y(), 16, 16}; // Case de blinky
    SDL_Rect blinky_scaled = entity_scaled(blinky_case);
    SDL_BlitScaled(plancheSprites, &m_blinky_sprite, win_surf, &blinky_scaled);

    SDL_Rect pinky_case = {m_pinky->get_x(), m_pinky->get_y(), 16, 16}; // Case de pinky
    SDL_Rect pinky_scaled = entity_scaled(pinky_case);
    SDL_BlitScaled(plancheSprites, &m_pinky_sprite, win_surf, &pinky_scaled);

    SDL_Rect inky_case = {m_inky->get_x(), m_inky->get_y(), 16, 16}; // Case de inky
    SDL_Rect inky_scaled = entity_scaled(inky_case);
    SDL_BlitScaled(plancheSprites, &m_inky_sprite, win_surf, &inky_scaled);

    SDL_Rect clyde_case = {m_clyde->get_x(), m_clyde->get_y(), 16, 16}; // Case de clyde
    SDL_Rect clyde_scaled = entity_scaled(clyde_case);
    SDL_BlitScaled(plancheSprites, &m_clyde_sprite, win_surf, &clyde_scaled);
    
    // SDL_SetColorKey(background, true, 0);

    frame++;
}






void View::draw_score(int score){

    

}

void View::change_sprite(){

    if ( m_pacman_direction != m_pacman->get_direction() ){ // Changement de direction
        m_pacman_direction = m_pacman->get_direction();
        switch (m_pacman_direction){
            case Direction::Direction::Left:
                m_pacman_sprite = {701, 17, 16, 16};
                break;
            case Direction::Direction::Right:
                m_pacman_sprite = {701, 1, 16, 16};
                break;
            case Direction::Direction::Up:
                m_pacman_sprite = {701, 33, 16, 16};
                break;
            case Direction::Direction::Down:
                m_pacman_sprite = {701, 49, 16, 16};
                break;
        }
    }
    if (frame%8 == 0){
        switch(m_pacman_direction){
            case Direction::Direction::Left:
                if ( sprite_pacman_animation == 1){
                    m_pacman_sprite = {685, 17, 16, 16};
                    sprite_pacman_animation = 2;
                }
                else{
                    m_pacman_sprite = {701, 17, 16, 16};
                    sprite_pacman_animation = 1;
                }
                break;
            case Direction::Direction::Right:
                if ( sprite_pacman_animation == 1){
                    m_pacman_sprite = {685, 1, 16, 16};
                    sprite_pacman_animation = 2;
                }
                else{
                    m_pacman_sprite = {701, 1, 16, 16};
                    sprite_pacman_animation = 1;
                }
                break;
            case Direction::Direction::Up:
                if ( sprite_pacman_animation == 1){
                    m_pacman_sprite = {685, 33, 16, 16};
                    sprite_pacman_animation = 2;
                }
                else{
                    m_pacman_sprite = {701, 33, 16, 16};
                    sprite_pacman_animation = 1;
                }
                break;
            case Direction::Direction::Down:
                if ( sprite_pacman_animation == 1){
                    m_pacman_sprite = {685, 49, 16, 16};
                    sprite_pacman_animation = 2;
                }
                else{
                    m_pacman_sprite = {701, 49, 16, 16};
                    sprite_pacman_animation = 1;
                }
                break;
        }
    }

    // On recommence avec chaque fantômes :        

    if ( m_blinky_direction != m_blinky->get_direction() ){ // Changement de direction
        m_blinky_direction = m_blinky->get_direction();
        switch (m_blinky_direction){
            case Direction::Direction::Left:
                m_blinky_sprite = {717, 66, 16, 16};
                break;
            case Direction::Direction::Right:
                m_blinky_sprite = {685, 66, 16, 16};
                break;
            case Direction::Direction::Up:
                m_blinky_sprite = {749, 66, 16, 16};
                break;
            case Direction::Direction::Down:
                m_blinky_sprite = {781, 66, 16, 16};
                break;
        }
    }
    if (frame%8 == 0){
        switch(m_blinky_direction){
            case Direction::Direction::Left:
                if ( sprite_blinky_animation == 1){
                    m_blinky_sprite = {733, 66, 16, 16};
                    sprite_blinky_animation = 2;
                }
                else{
                    m_blinky_sprite = {717, 66, 16, 16};
                    sprite_blinky_animation = 1;
                }
                break;
            case Direction::Direction::Right:
                if ( sprite_blinky_animation == 1){
                    m_blinky_sprite = {701, 66, 16, 16};
                    sprite_blinky_animation = 2;
                }
                else{
                    m_blinky_sprite = {685, 66, 16, 16};
                    sprite_blinky_animation = 1;
                }
                break;
            case Direction::Direction::Up:
                if ( sprite_blinky_animation == 1){
                    m_blinky_sprite = {765, 66, 16, 16};
                    sprite_blinky_animation = 2;
                }
                else{
                    m_blinky_sprite = {749, 66, 16, 16};
                    sprite_blinky_animation = 1;
                }
                break;
            case Direction::Direction::Down:
                if ( sprite_blinky_animation == 1){
                    m_blinky_sprite = {797, 66, 16, 16};
                    sprite_blinky_animation = 2;
                }
                else{
                    m_blinky_sprite = {781, 66, 16, 16};
                    sprite_blinky_animation = 1;
                }
                break;
        }
    }

    if ( m_pinky_direction != m_pinky->get_direction() ){ // Changement de direction
        m_pinky_direction = m_pinky->get_direction();
        switch (m_pinky_direction){
            case Direction::Direction::Left:
                m_pinky_sprite = {717, 82, 16, 16};
                break;
            case Direction::Direction::Right:
                m_pinky_sprite = {685, 82, 16, 16};
                break;
            case Direction::Direction::Up:
                m_pinky_sprite = {749, 82, 16, 16};
                break;
            case Direction::Direction::Down:
                m_pinky_sprite = {781, 82, 16, 16};
                break;
        }
    }

    if (frame%8 == 0){
        switch(m_pinky_direction){
            case Direction::Direction::Left:
                if ( sprite_pinky_animation == 1){
                    m_pinky_sprite = {733, 82, 16, 16};
                    sprite_pinky_animation = 2;
                }
                else{
                    m_pinky_sprite = {717, 82, 16, 16};
                    sprite_pinky_animation = 1;
                }
                break;
            case Direction::Direction::Right:
                if ( sprite_pinky_animation == 1){
                    m_pinky_sprite = {701, 82, 16, 16};
                    sprite_pinky_animation = 2;
                }
                else{
                    m_pinky_sprite = {685, 82, 16, 16};
                    sprite_pinky_animation = 1;
                }
                break;
            case Direction::Direction::Up:
                if ( sprite_pinky_animation == 1){
                    m_pinky_sprite = {765, 82, 16, 16};
                    sprite_pinky_animation = 2;
                }
                else{
                    m_pinky_sprite = {749, 82, 16, 16};
                    sprite_pinky_animation = 1;
                }
                break;
            case Direction::Direction::Down:
                if ( sprite_pinky_animation == 1){
                    m_pinky_sprite = {797, 82, 16, 16};
                    sprite_pinky_animation = 2;
                }
                else{
                    m_pinky_sprite = {781, 82, 16, 16};
                    sprite_pinky_animation = 1;
                }
                break;
        }
    }

    if ( m_inky_direction != m_inky->get_direction() ){ // Changement de direction
        m_inky_direction = m_inky->get_direction();
        switch (m_inky_direction){
            case Direction::Direction::Left:
                m_inky_sprite = {717, 98, 16, 16};
                break;
            case Direction::Direction::Right:
                m_inky_sprite = {685, 98, 16, 16};
                break;
            case Direction::Direction::Up:
                m_inky_sprite = {749, 98, 16, 16};
                break;
            case Direction::Direction::Down:
                m_inky_sprite = {781, 98, 16, 16};
                break;
        }
    }

    if (frame%8 == 0){
        switch(m_inky_direction){
            case Direction::Direction::Left:
                if ( sprite_inky_animation == 1){
                    m_inky_sprite = {733, 98, 16, 16};
                    sprite_inky_animation = 2;
                }
                else{
                    m_inky_sprite = {717, 98, 16, 16};
                    sprite_inky_animation = 1;
                }
                break;
            case Direction::Direction::Right:
                if ( sprite_inky_animation == 1){
                    m_inky_sprite = {701, 98, 16, 16};
                    sprite_inky_animation = 2;
                }
                else{
                    m_inky_sprite = {685, 98, 16, 16};
                    sprite_inky_animation = 1;
                }
                break;
            case Direction::Direction::Up:
                if ( sprite_inky_animation == 1){
                    m_inky_sprite = {765, 98, 16, 16};
                    sprite_inky_animation = 2;
                }
                else{
                    m_inky_sprite = {749, 98, 16, 16};
                    sprite_inky_animation = 1;
                }
                break;
            case Direction::Direction::Down:
                if ( sprite_inky_animation == 1){
                    m_inky_sprite = {797, 98, 16, 16};
                    sprite_inky_animation = 2;
                }
                else{
                    m_inky_sprite = {781, 98, 16, 16};
                    sprite_inky_animation = 1;
                }
                break;
        }
    }

    if ( m_clyde_direction != m_clyde->get_direction() ){ // Changement de direction
        m_clyde_direction = m_clyde->get_direction();
        switch (m_clyde_direction){
            case Direction::Direction::Left:
                m_clyde_sprite = {717, 114, 16, 16};
                break;
            case Direction::Direction::Right:
                m_clyde_sprite = {685, 114, 16, 16};
                break;
            case Direction::Direction::Up:
                m_clyde_sprite = {749, 114, 16, 16};
                break;
            case Direction::Direction::Down:
                m_clyde_sprite = {781, 114, 16, 16};
                break;
        }
    }

    if (frame%8 == 0){
        switch(m_clyde_direction){
            case Direction::Direction::Left:
                if ( sprite_clyde_animation == 1){
                    m_clyde_sprite = {733, 114, 16, 16};
                    sprite_clyde_animation = 2;
                }
                else{
                    m_clyde_sprite = {717, 114, 16, 16};
                    sprite_clyde_animation = 1;
                }
                break;
            case Direction::Direction::Right:
                if ( sprite_clyde_animation == 1){
                    m_clyde_sprite = {701, 114, 16, 16};
                    sprite_clyde_animation = 2;
                }
                else{
                    m_clyde_sprite = {685, 114, 16, 16};
                    sprite_clyde_animation = 1;
                }
                break;
            case Direction::Direction::Up:
                if ( sprite_clyde_animation == 1){
                    m_clyde_sprite = {765, 114, 16, 16};
                    sprite_clyde_animation = 2;
                }
                else{
                    m_clyde_sprite = {749, 114, 16, 16};
                    sprite_clyde_animation = 1;
                }
                break;
            case Direction::Direction::Down:
                if ( sprite_clyde_animation == 1){
                    m_clyde_sprite = {797, 114, 16, 16};
                    sprite_clyde_animation = 2;
                }
                else{
                    m_clyde_sprite = {781, 114, 16, 16};
                    sprite_clyde_animation = 1;
                }
                break;
        }
    }
}