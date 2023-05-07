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


Game::Game() :
    m_pacman(std::make_shared<Pacman>()),
    m_pinky(std::make_shared<Pinky>(0,0,m_pacman)),
    m_blinky(std::make_shared<Blinky>(0,0,m_pacman)),
    m_inky(std::make_shared<Inky>(0,0,m_pacman,m_blinky)),
    m_clyde(std::make_shared<Clyde>(0,0,m_pacman)),

    m_terrain(std::make_shared<Terrain>()),
    m_logic(std::make_shared<Logic>(
        m_pacman,
        m_blinky,
        m_clyde,
        m_pinky,
        m_inky,
        m_terrain
    )),
    m_view(std::make_unique<View>(
        m_pacman,
        m_blinky,
        m_pinky,
        m_inky,
        m_clyde,
        m_terrain
    ))
    {
    // m_pacman = std::make_shared<Pacman>();
    // m_pinky = std::make_shared<Pinky>(0,0,m_pacman);
    // m_blinky = std::make_shared<Blinky>(0,0,m_pacman);
    // m_inky =   std::make_shared<Inky>(0,0,m_pacman,m_blinky);
    // m_clyde = std::make_shared<Clyde>(0,0,m_pacman);

    // m_terrain = std::make_shared<Terrain>();
    // m_logic = std::make_unique<Logic>(
    //     m_pacman,
    //     m_blinky,
    //     m_clyde,
    //     m_pinky,
    //     m_inky,
    //     m_terrain
    // );
    // m_view = std::make_unique<View>(
    //     m_pacman,
    //     m_blinky,
    //     m_pinky,
    //     m_inky,
    //     m_clyde,
    //     m_terrain
    // );
    cout << "Fin de l'initialisation" << endl;
}

Game::~Game() {
    m_view.release();
}

void Game::launch() {

    // m_blinky->set_is_idling(true);

    
    bool quit = false;
    bool pause = true;
    int frame(0); // compte le nombre de frames qui se sont écoulées
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
                        // cout << m_pacman->get_pos() << endl;
                        break;
                    case SDLK_p:
                        pause = !pause;
                        // m_view->toggle_pause()
                        break;
                    case SDLK_d:
                        cout << "Pacman : " << m_pacman->get_tile() << m_pacman->get_pos() << endl;
                        cout << "Blinky : " << m_blinky->get_tile() << endl;
                        break;
                    case SDLK_s:
                        if(m_pacman->get_speed() != 0)
                            m_pacman->set_speed(0);
                        else 
                            m_pacman->set_speed(Setting::level1.pm_speed);
                        break;
                    case SDLK_e:
                        cout << "Eaten tiles : " << endl;
                        for(auto tile : m_terrain->get_eaten()) {
                            cout << tile << "\n";
                        }
                        cout << "--" << endl;
                        break;
                    case SDLK_c: // lancer clyde
                        m_clyde->set_is_starting(true);
                        break;
                    case SDLK_i: // lancer inky
                        m_inky->set_is_starting(true);
                        break;
                    case SDLK_o: // scatter
                        if(m_blinky->get_logic_state() == Ghost::L_CHASE) {
                            m_blinky->set_logic_state(Ghost::L_SCATTER);
                            m_inky  ->set_logic_state(Ghost::L_SCATTER);
                            m_pinky ->set_logic_state(Ghost::L_SCATTER);
                            m_clyde ->set_logic_state(Ghost::L_SCATTER);
                        }
                        else if (m_blinky->get_logic_state() == Ghost::L_SCATTER) {
                            m_blinky->set_logic_state(Ghost::L_CHASE);
                            m_inky  ->set_logic_state(Ghost::L_CHASE);
                            m_pinky ->set_logic_state(Ghost::L_CHASE);
                            m_clyde ->set_logic_state(Ghost::L_CHASE);
                        }
                        // cout << m_blinky->get_state() << endl;
                        // cout << m_inky->get_state() << endl;
                        // cout << m_pinky->get_state() << endl;
                        // cout << m_clyde->get_state() << endl;
                        break;
                    case SDLK_RETURN:
                        pause = true;
                        m_logic->init_new_level();
                        break;
                    default: break;
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
            m_pacman->set_next_dir(Direction::Left);
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            // m_pacman->move(1,0);
            m_pacman->set_next_dir(Direction::Right);
        }
        if (keys[SDL_SCANCODE_UP]) {
            // m_pacman->move(0,-1);
            m_pacman->set_next_dir(Direction::Up);
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            // m_pacman->move(0,1);
            m_pacman->set_next_dir(Direction::Down);
        }
        
        // FRAME
        if(!pause) {
            m_logic->do_frame();
            frame ++;
            if(frame%1 == 0) {
                // cout << "Pinky    : " << m_pinky->get_tile() << endl;
                // cout << "  dir     : " << m_pinky->get_direction() << endl;
                // cout << "  l_state : " << m_pinky->get_logic_state() << endl;
                // cout << "  state   : " << m_pinky->get_state() << endl;
                // cout << endl;
                // cout << "energy_Pacman : " << m_pacman->is_energized() << endl << endl;
            }
            // cout << m_pacman->get_pos() << endl;
            // cout << m_pinky->get_target() << endl;
        }
        

        // AFFICHAGE
        m_view->draw(
            // m_pacman.get(),
            // m_blinky.get(),
            // m_pinky.get(),
            // m_inky.get(),
            // m_clyde.get()
            );
        SDL_UpdateWindowSurface(m_view->getWindow());
        // LIMITE A 60 FPS
        SDL_Delay(16); 
        // SDL_Delay(5); 
    }
}
