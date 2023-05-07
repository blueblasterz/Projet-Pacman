#include "Game.hpp"

#include <iostream>

using std::cout, std::cin, std::endl;

// pour pouvoir utiliser << sur des pair
template<typename T,typename U>
std::ostream& operator << ( std::ostream& out, 
                const std::pair< T, U >& p )
{
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}


Game::Game() {
    m_pacman = std::make_shared<Pacman>();
    m_pinky = std::make_shared<Pinky>(0,0,m_pacman);
    m_blinky = std::make_shared<Blinky>(0,0,m_pacman);
    m_inky = std::make_shared<Inky>(0,0,m_pacman,m_blinky);
    m_clyde = std::make_shared<Clyde>(0,0,m_pacman);

    m_terrain = std::make_shared<Terrain>();
    m_logic = std::make_shared<Logic>(
        m_pacman,
        m_blinky,
        m_clyde,
        m_pinky,
        m_inky,
        m_terrain
    );
    m_view = std::make_shared<View>();
    cout << "Fin de l'initialisation" << endl;
}

Game::~Game() {}


void Game::launch() {
    m_pacman->set_direction(Direction::Left);
    m_pacman->set_pos(112,212);
    m_pacman->set_speed(0.8);

    // m_blinky->set_pos(112,116);
    m_blinky->set_tile(14,32);
    m_blinky->set_direction(Direction::Left);
    m_blinky->set_speed(0.75);
    m_pinky->set_pos(112,140);
    m_pinky->set_direction(Direction::Left);
    m_pinky->set_speed(0.75);
    m_inky->set_pos(96,140);
    m_inky->set_direction(Direction::Left);
    m_inky->set_speed(0.75);
    m_clyde->set_pos(128,140);
    m_clyde->set_direction(Direction::Left);
    m_clyde->set_speed(0.75);

    
    bool quit = false;
    bool pause = true;
    bool one_frame = false;
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
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_f:
                        m_logic->do_frame();
                        cout << m_pacman->get_pos() << endl;
                        break;
                    case SDLK_p:
                        pause = !pause;
                        break;
                    case SDLK_d:
                        cout << "Pacman : " << m_pacman->get_tile() << endl;
                        cout << "Blinky : " << m_blinky->get_tile() << endl;
                        break;
                    case SDLK_s:
                        if(m_pacman->get_speed() != 0)
                            m_pacman->set_speed(0);
                        else 
                            m_pacman->set_speed(0.8);
                        break;
                }
                break;
            default: break;
            }
        }

        // Gestion du clavier        
        int nbk;
        const Uint8* keys = SDL_GetKeyboardState(&nbk);
        if (keys[SDL_SCANCODE_ESCAPE]) {
            quit = true;
        }
        if (keys[SDL_SCANCODE_LEFT]) {
            // m_pacman->move(-1,0);
            m_pacman->set_direction(Direction::Left);
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            // m_pacman->move(1,0);
            m_pacman->set_direction(Direction::Right);
        }
        if (keys[SDL_SCANCODE_UP]) {
            // m_pacman->move(0,-1);
            m_pacman->set_direction(Direction::Up);
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            // m_pacman->move(0,1);
            m_pacman->set_direction(Direction::Down);
        }
        
        // FRAME
        if(!pause) {
            m_logic->do_frame();
            // cout << m_pacman->get_pos() << endl;
        }
        

        // AFFICHAGE
        m_view->draw(
            m_pacman.get(),
            m_blinky.get(),
            m_pinky.get(),
            m_inky.get(),
            m_clyde.get());
        SDL_UpdateWindowSurface(m_view->getWindow());
        // LIMITE A 60 FPS
        SDL_Delay(16); // utiliser SDL_GetTicks64() pour plus de precisions
    }
}
