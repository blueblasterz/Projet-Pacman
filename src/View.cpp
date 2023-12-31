#include "View.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include "Entity.hpp"
#include <vector>
#include <string.h>
#include "Ghost.hpp"
#include "Pacman.hpp"
#include "Logic.hpp"

#define SCREEN_WIDTH   672
#define SCREEN_HEIGHT  864


View::View(
        
        std::shared_ptr<Pacman> pacman,
        std::shared_ptr<Ghost> blinky,
        std::shared_ptr<Ghost> pinky,
        std::shared_ptr<Ghost> inky,
        std::shared_ptr<Ghost> clyde,
        std::shared_ptr<Terrain> terrain,
        std::shared_ptr<Logic> logic
        )
        : m_pacman(pacman),
        m_blinky(blinky),
        m_pinky(pinky),
        m_inky(inky),
        m_clyde(clyde),
        m_terrain(terrain),
        m_logic(logic),
        m_window(nullptr, SDL_DestroyWindow)
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
    m_window.reset(SDL_CreateWindow("PacMan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags));

    // On vérifie ensuite les valeurs renvoyées par la fenêtre SDL
    if (!m_window.get())
    {
        std::cerr << "La fenêtre " << SCREEN_WIDTH << "x" << SCREEN_HEIGHT << " n'a pas pu être ouverte : " << SDL_GetError() << std::endl;
        exit(1);
    }

    m_renderer = SDL_CreateRenderer(m_window.get(), -1, rendererFlags);

    if (!m_renderer)
    {
        std::cerr << "Le rendu n'a pas pu être créé : " << SDL_GetError() << std::endl;
        exit(1);
    }

    win_surf = SDL_GetWindowSurface(m_window.get());

    plancheSprites = SDL_LoadBMP("./src/texture.bmp");

    if (plancheSprites == NULL)
    {
        std::cerr << "Impossible de charger le fichier de sprites : " << SDL_GetError() << std::endl;
        exit(1);
    }

    // On initialise maintenant tout ce qui est relatif à la map

    bg_pointless = { 474, 327, 16, 16 }; // ici scale x3 pour afficher le jeu
    position_pointless = { 0, 0, 24, 24 }; // La case dans laquelle on veut enlever le pacgum 
    // Dimensions de la map et coordonnées de celle-ci dans la planche de sprites
    src_bg = { 1, 1, 224,248 }; // x,y, w,h (0,0) en haut a gauche
    bg = { 0,72, 672, 744 }; // ici scale x3 pour afficher le jeu
    // Maintenant on est censé pouvoir afficher la map de taille 227x250 sur une fenêtre de taille 908x1000

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
    score_scaled.x += 8*2;
    score_scaled.y += 8;
    SDL_BlitScaled(plancheSprites, &tiret, win_surf, &score_scaled);
    // On affiche le score de base à 0
    score_scaled.x += 8*2;
    score_scaled.y -= 8;

    m_number_sprite_0 = {230, 272, 7, 7};

    SDL_BlitScaled(plancheSprites, &m_number_sprite_0, win_surf, &score_scaled);
    score_scaled.x += 8*2;
    SDL_BlitScaled(plancheSprites, &m_number_sprite_0, win_surf, &score_scaled);


    // Ici on remarque que la largeur est de 16*7 pixels (unscaled) car le score est à 7 chiffres max
    black_bg_score = { score_scaled.x-16, score_scaled.y, 16*7*2, 14};
    position_score = {black_bg_score.x, black_bg_score.y, 14, 14};
    // Correspond au background noir du score (pour l'effacer et le réécrire par derrière)

    // On peut posséder jusqu'à 5 vies normalement (cas où on mange les 4 fantômes d'un coup)
    black_bg_lives = {48, SCREEN_HEIGHT-48, 48*5, 48};
    position_lives = {get_rect_x(black_bg_lives), get_rect_y(black_bg_lives), 48, 48};

    bg_message = {12*8*3-24, 21*8*3-24, 6*8, 6*8};
    position_message = {get_rect_x(bg_message), get_rect_y(bg_message), 8*3, 8*3};
    position_message2 = {get_rect_x(bg_message), get_rect_y(bg_message)+8*3, 8*3, 8*3};
    
    
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

    death_animation = 0;

    m_frame = 0;


    // On initialise tous les sprites :
    m_pacman_sprite_idle = {716, 1, 16, 16};
    m_pacman_sprite_right_1 = {701, 1, 16, 16};
    m_pacman_sprite_right_2 = {685, 1, 16, 16};
    m_pacman_sprite_left_1 = {701, 17, 16, 16};
    m_pacman_sprite_left_2 = {685, 17, 16, 16};
    m_pacman_sprite_up_1 = {701, 33, 16, 16};
    m_pacman_sprite_up_2 = {685, 33, 16, 16};
    m_pacman_sprite_down_1 = {701, 49, 16, 16};
    m_pacman_sprite_down_2 = {685, 49, 16, 16};

    m_pacman_sprite_dead_1 = {732, 1, 16, 16};
    m_pacman_sprite_dead_2 = {748, 1, 16, 16};
    m_pacman_sprite_dead_3 = {764, 1, 16, 16};
    m_pacman_sprite_dead_4 = {780, 1, 16, 16};
    m_pacman_sprite_dead_5 = {796, 1, 16, 16};
    m_pacman_sprite_dead_6 = {812, 1, 16, 16};
    m_pacman_sprite_dead_7 = {828, 1, 16, 16};
    m_pacman_sprite_dead_8 = {844, 1, 16, 16};
    m_pacman_sprite_dead_9 = {860, 1, 16, 16};
    m_pacman_sprite_dead_10 = {876, 1, 16, 16};
    m_pacman_sprite_dead_11 = {892, 1, 16, 16};

    m_blinky_sprite_right_1 = {685, 65, 16, 16};
    m_blinky_sprite_right_2 = {701, 65, 16, 16};
    m_blinky_sprite_left_1 = {717, 65, 16, 16};
    m_blinky_sprite_left_2 = {733, 65, 16, 16};
    m_blinky_sprite_up_1 = {749, 65, 16, 16};
    m_blinky_sprite_up_2 = {765, 65, 16, 16};
    m_blinky_sprite_down_1 = {781, 65, 16, 16};
    m_blinky_sprite_down_2 = {797, 65, 16, 16};

    m_pinky_sprite_right_1 = {685, 81, 16, 16};
    m_pinky_sprite_right_2 = {701, 81, 16, 16};
    m_pinky_sprite_left_1 = {717, 81, 16, 16};
    m_pinky_sprite_left_2 = {733, 81, 16, 16};
    m_pinky_sprite_up_1 = {749, 81, 16, 16};
    m_pinky_sprite_up_2 = {765, 81, 16, 16};
    m_pinky_sprite_down_1 = {781, 81, 16, 16};
    m_pinky_sprite_down_2 = {797, 81, 16, 16};

    m_inky_sprite_right_1 = {685, 97, 16, 16};
    m_inky_sprite_right_2 = {701, 97, 16, 16};
    m_inky_sprite_left_1 = {717, 97, 16, 16};
    m_inky_sprite_left_2 = {733, 97, 16, 16};
    m_inky_sprite_up_1 = {749, 97, 16, 16};
    m_inky_sprite_up_2 = {765, 97, 16, 16};
    m_inky_sprite_down_1 = {781, 97, 16, 16};
    m_inky_sprite_down_2 = {797, 97, 16, 16};

    m_clyde_sprite_right_1 = {685, 113, 16, 16};
    m_clyde_sprite_right_2 = {701, 113, 16, 16};
    m_clyde_sprite_left_1 = {717, 113, 16, 16};
    m_clyde_sprite_left_2 = {733, 113, 16, 16};
    m_clyde_sprite_up_1 = {749, 113, 16, 16};
    m_clyde_sprite_up_2 = {765, 113, 16, 16};
    m_clyde_sprite_down_1 = {781, 113, 16, 16};
    m_clyde_sprite_down_2 = {797, 113, 16, 16};

    m_fright_sprite_1 = {813, 65, 16, 16};
    m_fright_sprite_2 = {829, 65, 16, 16};
    m_fright_sprite_3 = {845, 65, 16, 16};
    m_fright_sprite_4 = {861, 65, 16, 16};

    m_eaten_sprite_right = {813, 81, 16, 16};
    m_eaten_sprite_left = {829, 81, 16, 16};
    m_eaten_sprite_up = {845, 81, 16, 16};
    m_eaten_sprite_down = {861, 81, 16, 16};

    m_score_sprite_200 = {690, 130, 16, 16};
    m_score_sprite_400 = {713, 130, 16, 16};
    m_score_sprite_800 = {736, 130, 16, 16};
    m_score_sprite_1600 = {759, 130, 16, 16};

    m_letter_sprite_A = {230, 290, 7, 7};
    m_letter_sprite_C = {246, 290, 7, 7};
    m_letter_sprite_D = {254, 290, 7, 7};
    m_letter_sprite_E = {262, 290, 7, 7};
    m_letter_sprite_G = {278, 290, 7, 7};
    m_letter_sprite_L = {318, 290, 7, 7};
    m_letter_sprite_M = {326, 290, 7, 7};
    m_letter_sprite_O = {342, 290, 7, 7};
    m_letter_sprite_P = {230, 298, 7, 7};
    m_letter_sprite_R = {246, 298, 7, 7};
    m_letter_sprite_S = {254, 298, 7, 7};
    m_letter_sprite_T = {262, 298, 7, 7};
    m_letter_sprite_U = {270, 298, 7, 7};
    m_letter_sprite_V = {278, 298, 7, 7};
    m_letter_sprite_Y = {303, 298, 7, 7};
    m_letter_sprite_excl = {318, 298, 7, 7};

}

View::~View(){
    // On free les sprites 
    SDL_FreeSurface(plancheSprites);
    SDL_FreeSurface(win_surf);
    // SDL_FreeSurface(background);

    // On détruit le rendu et la fenêtre
    SDL_DestroyRenderer(m_renderer);

    SDL_Quit();
}



SDL_Window* View::getWindow() const{
    return m_window.get();
}

void View::set_rect(SDL_Rect *rect, int x, int y){
    rect->x = x;
    rect->y = y;
}

SDL_Rect View::entity_scaled(SDL_Rect entity){
    SDL_Rect scaled_entity = {entity.x*3 - 24, entity.y*3 - 24, entity.w*3, entity.h*3};
    return scaled_entity;
}

void View::set_Rect_scaled(SDL_Rect *rect, double x, double y){
    rect->x = x*3 - 24;
    rect->y = y*3 - 24;
} 

int View::get_rect_x(SDL_Rect rect) const{
    return rect.x;
}

int View::get_rect_y(SDL_Rect rect) const{
    return rect.y;
}

void View::erase_point(std::vector<std::pair<int,int>> points){
    for (auto point : points){
        set_Rect_scaled(&position_pointless, point.first*8+8, point.second*8+8);
        SDL_BlitScaled(plancheSprites, &bg_pointless, win_surf, &position_pointless);
    }
}

void View::draw_score(){

    SDL_BlitScaled(plancheSprites, &bg_pointless, win_surf, &black_bg_score);
    int score = m_logic->get_score();
    // On récupère les chiffres du score
    std::string score_string = std::to_string(score);
    for (size_t i = 0; i < score_string.size(); i++){
        int score_digit = score_string[i] - '0';
        set_rect(&m_number_sprite_0, 230 + 8*score_digit, 272);
    
        SDL_BlitScaled(plancheSprites, &m_number_sprite_0, win_surf, &position_score);
        position_score.x += 14;
    }
    set_rect(&position_score, get_rect_x(black_bg_score), get_rect_y(black_bg_score));
    set_rect(&m_number_sprite_0, 230, 272);
}

void View::draw_lives(){
    SDL_BlitScaled(plancheSprites, &bg_pointless, win_surf, &black_bg_lives);
    // On récupère la position du score : 
    for (int i = 0; i < m_logic->get_life(); i++){
    
        SDL_BlitScaled(plancheSprites, &m_pacman_sprite_left_1, win_surf, &position_lives);
        position_lives.x += 48;
    }
    position_lives.x = black_bg_lives.x;
}

void View::draw_pause(){

    SDL_BlitScaled(plancheSprites, &m_letter_sprite_P, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_A, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_U, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_S, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_E, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_excl, win_surf, &position_message);
    position_message.x = get_rect_x(bg_message);
}

void View::draw_start(){

    SDL_BlitScaled(plancheSprites, &m_letter_sprite_R, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_E, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_A, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_D, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_Y, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_excl, win_surf, &position_message);
    position_message.x = get_rect_x(bg_message);

}

// void View::draw_win(){

//     SDL_BlitScaled(plancheSprites, &m_letter_sprite_U, win_surf, &position_message2);
//     position_message.x += 24;

// }
void View::draw_game_over(){
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_P, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_E, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_R, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_D, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_U, win_surf, &position_message);
    position_message.x += 24;
    SDL_BlitScaled(plancheSprites, &m_letter_sprite_excl, win_surf, &position_message);
    position_message.x = get_rect_x(bg_message);
}

void View::draw(){
    // Ghost::State state = m_blinky->get_state();

    SDL_SetColorKey(plancheSprites, false, 0);
    SDL_BlitScaled(plancheSprites, &src_bg, win_surf, &bg);
    std::vector<std::pair<int,int>> points = m_terrain->get_eaten(); 
    erase_point(points);
    draw_score();
    draw_lives();
    
    // On définit le noir comme transparent lorsqu'on affiche les sprites
    
    SDL_SetColorKey(plancheSprites, true, 0);

    if (m_logic->get_life() == -1){
        draw_game_over();
    }
    if (m_logic->is_paused()){
        draw_pause();
    }
    else if (m_logic->show_ready()){
        draw_start();
    }
    
    SDL_Rect pos = {int(m_blinky->get_x()*3-24), int(m_blinky->get_y()*3-24), 48, 48};
    
    animation_ghost(pos, m_blinky.get(), 0);

    set_Rect_scaled(&pos, m_pinky->get_x(), m_pinky->get_y());
    animation_ghost(pos, m_pinky.get(), 1);

    set_Rect_scaled(&pos, m_inky->get_x(), m_inky->get_y());
    animation_ghost(pos, m_inky.get(), 2);

    set_Rect_scaled(&pos, m_clyde->get_x(), m_clyde->get_y());
    animation_ghost(pos, m_clyde.get(), 3);

    set_Rect_scaled(&pos, m_pacman->get_x(), m_pacman->get_y());
    if (!m_pacman->is_dead()){
        change_sprite(pos);
    }
    else{
        change_sprite_death(pos);
    }
    
    m_frame++;
}




// On a une fonction d'animation de base pour chaque entités

void View::change_sprite_b(SDL_Rect pos){

    if ( m_blinky_direction != m_blinky->get_direction() ){ // Changement de direction
        m_blinky_direction = m_blinky->get_direction();
        switch (m_blinky_direction){
            case Direction::Direction::Left:
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_left_1, win_surf, &pos);
                break;
            case Direction::Direction::Right:
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_right_1, win_surf, &pos);
                break;
            case Direction::Direction::Up:
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_up_1, win_surf, &pos);
                break;
            case Direction::Direction::Down:
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_down_1, win_surf, &pos);
                break;
        }
    }
    switch(m_blinky_direction){
        case Direction::Direction::Left:
            if ( sprite_blinky_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_left_2, win_surf, &pos);
                sprite_blinky_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_blinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_left_1, win_surf, &pos);
                sprite_blinky_animation = 1;
            }
            else if (sprite_blinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_left_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_left_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Right:
            if ( sprite_blinky_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_right_2, win_surf, &pos);
                sprite_blinky_animation = 2;
            }
            else if (sprite_blinky_animation == 2 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_right_1, win_surf, &pos);
                sprite_blinky_animation = 1;
            }
            else if (sprite_blinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_right_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_right_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Up:
            if ( sprite_blinky_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_up_2, win_surf, &pos);
                sprite_blinky_animation = 2;
            }
            else if (sprite_blinky_animation == 2 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_up_1, win_surf, &pos);
                sprite_blinky_animation = 1;
            }
            else if (sprite_blinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_up_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_up_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Down:
            if ( sprite_blinky_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_down_2, win_surf, &pos);
                sprite_blinky_animation = 2;
            }
            else if (sprite_blinky_animation == 2 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_down_1, win_surf, &pos);
                sprite_blinky_animation = 1;
            }
            else if (sprite_blinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_down_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_blinky_sprite_down_1, win_surf, &pos);
            }
            break;
    }
}


void View::change_sprite_p(SDL_Rect pos){
    if ( m_pinky_direction != m_pinky->get_direction() ){ // Changement de direction
        m_pinky_direction = m_pinky->get_direction();
        switch (m_pinky_direction){
            case Direction::Direction::Left:
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_left_1, win_surf, &pos);
                break;
            case Direction::Direction::Right:
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_right_1, win_surf, &pos);
                break;
            case Direction::Direction::Up:
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_up_1, win_surf, &pos);
                break;
            case Direction::Direction::Down:
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_down_1, win_surf, &pos);
                break;
        }
    }

    switch(m_pinky_direction){
        case Direction::Direction::Left:
            if ( sprite_pinky_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_left_2, win_surf, &pos);
                sprite_pinky_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_pinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_left_1, win_surf, &pos);
                sprite_pinky_animation = 1;
            }
            else if (sprite_pinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_left_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_left_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Right:
            if ( sprite_pinky_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_right_2, win_surf, &pos);
                sprite_pinky_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_pinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_right_1, win_surf, &pos);
                sprite_pinky_animation = 1;
            }
            else if (sprite_pinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_right_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_right_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Up:
            if ( sprite_pinky_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_up_2, win_surf, &pos);
                sprite_pinky_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_pinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_up_1, win_surf, &pos);
                sprite_pinky_animation = 1;
            }
            else if (sprite_pinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_up_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_up_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Down:
            if ( sprite_pinky_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_down_2, win_surf, &pos);
                sprite_pinky_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_pinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_down_1, win_surf, &pos);
                sprite_pinky_animation = 1;
            }
            else if (sprite_pinky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_down_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_pinky_sprite_down_1, win_surf, &pos);
            }
            break;
    }
}



void View::change_sprite_i(SDL_Rect pos){
    if ( m_inky_direction != m_inky->get_direction() ){ // Changement de direction
        m_inky_direction = m_inky->get_direction();
        switch (m_inky_direction){
            case Direction::Direction::Left:
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_left_1, win_surf, &pos);
                break;
            case Direction::Direction::Right:
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_right_1, win_surf, &pos);
                break;
            case Direction::Direction::Up:
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_up_1, win_surf, &pos);
                break;
            case Direction::Direction::Down:
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_down_1, win_surf, &pos);
                break;
        }
    }

    switch(m_inky_direction){
        case Direction::Direction::Left:
            if ( sprite_inky_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_left_2, win_surf, &pos);
                sprite_inky_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_inky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_left_1, win_surf, &pos);
                sprite_inky_animation = 1;
            }
            else if (sprite_inky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_left_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_left_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Right:
            if ( sprite_inky_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_right_2, win_surf, &pos);
                sprite_inky_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_inky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_right_1, win_surf, &pos);
                sprite_inky_animation = 1;
            }
            else if (sprite_inky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_right_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_right_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Up:
            if ( sprite_inky_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_up_2, win_surf, &pos);
                sprite_inky_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_inky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_up_1, win_surf, &pos);
                sprite_inky_animation = 1;
            }
            else if (sprite_inky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_up_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_up_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Down:
            if ( sprite_inky_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_down_2, win_surf, &pos);
                sprite_inky_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_inky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_down_1, win_surf, &pos);
                sprite_inky_animation = 1;
            }
            else if (sprite_inky_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_down_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_inky_sprite_down_1, win_surf, &pos);
            }
            break;
    }
}



void View::change_sprite_c(SDL_Rect pos){
    if ( m_clyde_direction != m_clyde->get_direction() ){ // Changement de direction
        m_clyde_direction = m_clyde->get_direction();
        switch (m_clyde_direction){
            case Direction::Direction::Left:
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_left_1, win_surf, &pos);
                break;
            case Direction::Direction::Right:
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_right_1, win_surf, &pos);
                break;
            case Direction::Direction::Up:
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_up_1, win_surf, &pos);
                break;
            case Direction::Direction::Down:
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_down_1, win_surf, &pos);
                break;
        }
    }

    switch(m_clyde_direction){
        case Direction::Direction::Left:
            if ( sprite_clyde_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_left_2, win_surf, &pos);
                sprite_clyde_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_clyde_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_left_1, win_surf, &pos);
                sprite_clyde_animation = 1;
            }
            else if (sprite_clyde_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_left_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_left_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Right:
            if ( sprite_clyde_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_right_2, win_surf, &pos);
                sprite_clyde_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_clyde_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_right_1, win_surf, &pos);
                sprite_clyde_animation = 1;
            }
            else if (sprite_clyde_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_right_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_right_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Up:
            if ( sprite_clyde_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_up_2, win_surf, &pos);
                sprite_clyde_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_clyde_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_up_1, win_surf, &pos);
                sprite_clyde_animation = 1;
            }
            else if (sprite_clyde_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_up_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_up_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Down:
            if ( sprite_clyde_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_down_2, win_surf, &pos);
                sprite_clyde_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_clyde_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_down_1, win_surf, &pos);
                sprite_clyde_animation = 1;
            }
            else if (sprite_clyde_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_down_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_clyde_sprite_down_1, win_surf, &pos);
            }
            break;
    }
}



void View::change_sprite(SDL_Rect pos){

    if ( m_pacman_direction != m_pacman->get_direction() ){ // Changement de direction
        m_pacman_direction = m_pacman->get_direction();
        switch (m_pacman_direction){
            case Direction::Direction::Left:
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_left_1, win_surf, &pos);
                break;
            case Direction::Direction::Right:
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_right_1, win_surf, &pos);
                break;
            case Direction::Direction::Up:
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_up_1, win_surf, &pos);  
                break;
            case Direction::Direction::Down:
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_down_1, win_surf, &pos);
                break;
        }
    }
    switch(m_pacman_direction){
        case Direction::Direction::Left:
            if ( sprite_pacman_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_left_2, win_surf, &pos);
                sprite_pacman_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_pacman_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_left_1, win_surf, &pos);
                sprite_pacman_animation = 1;
            }
            else if (sprite_pacman_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_left_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_left_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Right:
            if ( sprite_pacman_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_right_2, win_surf, &pos);
                sprite_pacman_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_pacman_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_right_1, win_surf, &pos);
                sprite_pacman_animation = 1;
            }
            else if (sprite_pacman_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_right_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_right_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Up:
            if ( sprite_pacman_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_up_2, win_surf, &pos);
                sprite_pacman_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_pacman_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_up_1, win_surf, &pos);
                sprite_pacman_animation = 1;
            }
            else if (sprite_pacman_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_up_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_up_1, win_surf, &pos);
            }
            break;
        case Direction::Direction::Down:
            if ( sprite_pacman_animation == 1 && m_frame % 8 == 0){
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_down_2, win_surf, &pos);
                sprite_pacman_animation = 2;
            }
            else if (m_frame % 8 == 0 && sprite_pacman_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_down_1, win_surf, &pos);
                sprite_pacman_animation = 1;
            }
            else if (sprite_pacman_animation == 2){
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_down_2, win_surf, &pos);
            }
            else{
                SDL_BlitScaled(plancheSprites, &m_pacman_sprite_down_1, win_surf, &pos);
            }
            break;
    }
}


// On a maintenant les fonctions d'animations pour les fantômes :(communes entres chaque fantômes)

// Lorsqu'ils sont apeurés :

void View::change_sprite_fright(SDL_Rect pos){

    int n = m_frame%16;
    if (n < 8){
        SDL_BlitScaled(plancheSprites, &m_fright_sprite_1, win_surf, &pos);
    }
    else if (n < 16){
        SDL_BlitScaled(plancheSprites, &m_fright_sprite_2, win_surf, &pos);
    }

}

void View::change_sprite_fright_end(SDL_Rect pos){
    int n = m_frame%32;
    if (n < 8){
        SDL_BlitScaled(plancheSprites, &m_fright_sprite_1, win_surf, &pos);
    }
    else if (n < 16){
        SDL_BlitScaled(plancheSprites, &m_fright_sprite_2, win_surf, &pos);
    }
    else if (n < 24){
        SDL_BlitScaled(plancheSprites, &m_fright_sprite_3, win_surf, &pos);
    }
    else{
        SDL_BlitScaled(plancheSprites, &m_fright_sprite_4, win_surf, &pos);
    }
}

// Lorsqu'ils sont mangés :

void View::change_sprite_eaten(Ghost* ghost, SDL_Rect pos){
    switch (ghost->get_direction()){
        case Direction::Direction::Left:
            SDL_BlitScaled(plancheSprites, &m_eaten_sprite_left , win_surf, &pos);
            break;
        case Direction::Direction::Right:
            SDL_BlitScaled(plancheSprites, &m_eaten_sprite_right, win_surf, &pos);
            break;
        case Direction::Direction::Up:
            SDL_BlitScaled(plancheSprites, &m_eaten_sprite_up, win_surf, &pos);  
            break;
        case Direction::Direction::Down:
            SDL_BlitScaled(plancheSprites, &m_eaten_sprite_down, win_surf, &pos);
            break;
    }
}

// Lorsque PacMan meurt :

void View::change_sprite_death(SDL_Rect pos){
    int n = death_animation%88;
    if (n < 8){
        SDL_BlitScaled(plancheSprites, &m_pacman_sprite_dead_1, win_surf, &pos);
    }
    else if (n < 16){
        SDL_BlitScaled(plancheSprites, &m_pacman_sprite_dead_2, win_surf, &pos);
    }
    else if (n < 24){
        SDL_BlitScaled(plancheSprites, &m_pacman_sprite_dead_3, win_surf, &pos);
    }
    else if (n < 32){
        SDL_BlitScaled(plancheSprites, &m_pacman_sprite_dead_4, win_surf, &pos);
    }
    else if (n < 40){
        SDL_BlitScaled(plancheSprites, &m_pacman_sprite_dead_5, win_surf, &pos);
    }
    else if (n < 48){
        SDL_BlitScaled(plancheSprites, &m_pacman_sprite_dead_6, win_surf, &pos);
    }
    else if (n < 56){
        SDL_BlitScaled(plancheSprites, &m_pacman_sprite_dead_7, win_surf, &pos);
    }
    else if (n < 64){
        SDL_BlitScaled(plancheSprites, &m_pacman_sprite_dead_8, win_surf, &pos);
    }
    else if (n < 72){
        SDL_BlitScaled(plancheSprites, &m_pacman_sprite_dead_9, win_surf, &pos);
    }
    else if (n < 80){
        SDL_BlitScaled(plancheSprites, &m_pacman_sprite_dead_10, win_surf, &pos);
    }
    else{
        SDL_BlitScaled(plancheSprites, &m_pacman_sprite_dead_11, win_surf, &pos);
    }
    death_animation++;
    if (death_animation == 88){
        death_animation = 0;
    }
}

// Lorsque les fantômes se font manger (ils se transforment en score) :

void View::change_sprite_score(SDL_Rect pos, int score){

    switch (score)
    {
    case 200:
        SDL_BlitScaled(plancheSprites, &m_score_sprite_200, win_surf, &pos);
        break;
    case 400:
        SDL_BlitScaled(plancheSprites, &m_score_sprite_400, win_surf, &pos);
        break;
    case 800:
        SDL_BlitScaled(plancheSprites, &m_score_sprite_800, win_surf, &pos);
        break;
    case 1600:
        SDL_BlitScaled(plancheSprites, &m_score_sprite_1600, win_surf, &pos);
        break;
    
    default:
        break;
    }
}

void View::animation_ghost(SDL_Rect pos, Ghost* ghost, int name){
    Ghost::State state = ghost->get_state();
    switch (state)
    {
    case Ghost::State::NORMAL:
        switch (name)
        {
        case 0:
            change_sprite_b(pos);
            break;
        case 1:
            change_sprite_p(pos);
            break;
        case 2:
            change_sprite_i(pos);
            break;
        case 3:
            change_sprite_c(pos);
            break;
        default:
            break;
        }
        break;

    case Ghost::State::FRIGHT_END:
        change_sprite_fright_end(pos);
        break;

    case Ghost::State::FRIGHT:
        change_sprite_fright(pos);
        break;

    case Ghost::State::EATEN:
        change_sprite_eaten(ghost, pos);
        break;

    case Ghost::State::SCORE:
        change_sprite_score(pos, ghost->get_score());
        break;
    default:
        break;
    }
}

