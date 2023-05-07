#include "Logic.hpp"

#include <iostream>
// #include <cmath>
#include <random>
#include <vector>
#include <iterator>

#include "Pacman.hpp"
#include "Ghost.hpp"
#include "Pinky.hpp"
#include "Blinky.hpp"
#include "Inky.hpp"
#include "Clyde.hpp"
#include "Terrain.hpp"

using std::cout, std::endl;
// pour pouvoir utiliser << sur des pair
template<typename T,typename U>
std::ostream& operator << ( std::ostream& out, 
                const std::pair< T, U >& p )
{
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}

// template<typename T,typename U>
// bool operator== ( const std::pair< T, U >& p, 
//                 const std::pair< T, U >& q )
// {
//     return p.first == q.first && p.second == q.second;
// }

// distance entre 2 points
double distance( std::pair<int,int> a, std::pair<int,int> b ) {
    return sqrt( pow(a.first*8-b.first*8,2) 
               + pow(a.second*8-b.second*8,2) );
}

Logic::Logic(
    std::shared_ptr<Pacman>  &pacman,
    std::shared_ptr<Blinky>  &blinky,
    std::shared_ptr<Clyde>   &clyde,
    std::shared_ptr<Pinky>   &pinky,
    std::shared_ptr<Inky>    &inky,
    std::shared_ptr<Terrain> &terrain
):  m_pacman(pacman),
    m_blinky(blinky),
    m_clyde(clyde),
    m_pinky(pinky),
    m_inky(inky),
    m_terrain(terrain),
    m_score(0),
    m_frame(0),
    m_level(0),
    m_fright(false),
    m_nb_dot_eaten(0),
    m_rng(42),
    m_current_ghost_score(200),
    m_life(4),
    m_compteur_reset(0),
    m_frame_last_eaten(0),
    m_paused(false),
    m_finished(false)
{
    // m_pacman->print_position();
    this->init_new_level();
}

int Logic::get_score() {
    return m_score;
}
int Logic::add_score(int add) {
    m_score += add;
    return m_score;
}
void Logic::set_score(int score) {
    m_score = score;
}

void Logic::do_frame() {
    if(m_compteur_reset != 0) {
        m_compteur_reset -= 1;
        // if(m_compteur_reset == 80) {
        // }
        if(m_compteur_reset == 80) {
            m_pacman->set_is_dead(false);
            place_entities();
            m_frame=0;
            if(m_nb_dot_eaten == 244) {
                init_new_level();
            }
        }
        if(m_compteur_reset == 0 && m_life == -1) {
            m_finished = true;
        }

        return;
    }
    if(m_paused) return;

    if(m_nb_dot_eaten == 244) {
        cout << "FIN DU NIVEAU BRAVO" << endl;
        m_compteur_reset = 120;
        return;
    }

    do_events();

    move_pacman();
    int eaten = m_terrain->add_eaten(m_pacman->get_tile());
    if(eaten == 1) {
        // std::cout << "DOT" << std::endl;
        m_score += 10;
        m_pacman->add_stopped(1);
        m_nb_dot_eaten += 1;
        m_frame_last_eaten = 0;
    }
    else if(eaten == 2) {
        m_frame_last_eaten = 0;
        std::cout << "SUPERDOT" << std::endl;
        m_score += 50;
        m_nb_dot_eaten += 1;
        m_pacman->add_stopped(3);
        if(!m_pacman->is_energized()) {
            m_blinky->set_logic_state(Ghost::L_FRIGHT);
            m_pinky->set_logic_state(Ghost::L_FRIGHT);
            m_inky->set_logic_state(Ghost::L_FRIGHT);
            m_clyde->set_logic_state(Ghost::L_FRIGHT);
        }
        else {
            m_blinky->set_state(Ghost::FRIGHT);
            m_pinky->set_state(Ghost::FRIGHT);
            m_inky->set_state(Ghost::FRIGHT);
            m_clyde->set_state(Ghost::FRIGHT);
        }
        m_pacman->set_energy(60*m_settings.fright_time);
    }
    else {
        m_frame_last_eaten += 1;
        if(m_frame_last_eaten == 180) {
            if(m_inky->is_idling()) {
                m_inky->set_is_starting(true);
            }
            else if(m_clyde->is_idling()) {
                m_clyde->set_is_starting(true);
            }
            m_frame_last_eaten=0;
        }
    }
    
    // les fantomes utilisent la nouvelle position de pacman
    move_ghost(m_blinky);
    move_ghost(m_clyde);
    move_ghost(m_pinky);
    move_ghost(m_inky); // inky utilisera la nouvelle position de blinky

    int energy_left = m_pacman->is_energized();
    if( energy_left == 0 ){
        m_frame += 1;
    }
    else if( energy_left == 1 || m_current_ghost_score == 3200) {
        m_current_ghost_score = 200; // reset
        m_pacman->set_energy(1);
    }
    check_collision_ghost(m_blinky);
    check_collision_ghost(m_pinky);
    check_collision_ghost(m_inky);
    check_collision_ghost(m_clyde);


    m_pacman->decrease_energy();
}

void Logic::init_new_level() {
    m_rng.seed(42); // même seed pour tout les niveaux, comme dans l'original

    m_level += 1;
    std::cout << "loading level " << m_level << std::endl;
    if(m_level == 1) {
        m_settings = Setting::level1;
    }
    else if(m_level <= 4) {
        m_settings = Setting::level2_4;
    }
    else if(m_level <= 20) {
        m_settings = Setting::level5_20;
    }
    else {
        m_settings = Setting::level21;
    }

    m_terrain->load_tilemap();
    m_pacman->set_speed( m_settings.pm_speed );
    m_blinky->load_param( m_settings );
    m_pinky->load_param( m_settings );
    m_inky->load_param( m_settings );
    m_clyde->load_param( m_settings );

    place_entities();

    m_current_ghost_score = 200;
    m_nb_dot_eaten = 0;
    m_frame = 0;
}

void Logic::place_entities() {

    m_pacman->set_direction(Direction::Left);
    m_pacman->set_pos(112,212);
    m_pacman->set_energy(0);

    m_blinky->set_pos(112,116);
    m_blinky->set_direction(Direction::Left);
    m_blinky->set_logic_state(Ghost::L_SCATTER);
    m_blinky->set_state(Ghost::NORMAL);
    m_blinky->reset_counter_anim();
    m_blinky->set_is_idling(false);
    m_blinky->set_is_starting(false);
    m_blinky->is_force_reverse_pending(); // pour le virer
    m_blinky->set_is_eaten(false);

    m_pinky->set_pos(112,140);
    m_pinky->set_direction(Direction::Left);
    m_pinky->set_logic_state(Ghost::L_SCATTER);
    m_pinky->set_state(Ghost::NORMAL);
    m_pinky->reset_counter_anim();
    m_pinky->set_is_starting(true);
    m_pinky->is_force_reverse_pending(); // pour le virer
    m_pinky->set_is_eaten(false);
    
    m_inky->set_pos(96,140);
    m_inky->set_direction(Direction::Left);
    m_inky->set_logic_state(Ghost::L_SCATTER);
    m_inky->set_state(Ghost::NORMAL);
    m_inky->reset_counter_anim();
    m_inky->set_is_idling(true);
    m_inky->is_force_reverse_pending(); // pour le virer
    m_inky->set_is_eaten(false);

    m_clyde->set_pos(128,140);
    m_clyde->set_direction(Direction::Left);
    m_clyde->set_logic_state(Ghost::L_SCATTER);
    m_clyde->set_state(Ghost::NORMAL);
    m_clyde->reset_counter_anim();
    m_clyde->set_is_idling(true);
    m_clyde->is_force_reverse_pending(); // pour le virer
    m_clyde->set_is_eaten(false);
}

void Logic::do_events() {
    if( m_frame == m_settings.scatter.at(0)*60) { // fin premier scatter
        cout << "debut chase 1" << endl;
        m_blinky->set_logic_state(Ghost::L_CHASE);
        m_pinky->set_logic_state( Ghost::L_CHASE);
        m_inky->set_logic_state(  Ghost::L_CHASE);
        m_clyde->set_logic_state( Ghost::L_CHASE);
    }
    if( m_frame == m_settings.chase.at(0)*60) { // fin premier chase
        cout << "debut scatter 2" << endl;
        m_blinky->set_logic_state(Ghost::L_SCATTER);
        m_pinky->set_logic_state( Ghost::L_SCATTER);
        m_inky->set_logic_state(  Ghost::L_SCATTER);
        m_clyde->set_logic_state( Ghost::L_SCATTER);
    }
    if( m_frame == m_settings.scatter.at(1)*60) { // fin 2e scatter
        cout << "debut chase 2 " << endl;
        m_blinky->set_logic_state(Ghost::L_CHASE);
        m_pinky->set_logic_state( Ghost::L_CHASE);
        m_inky->set_logic_state(  Ghost::L_CHASE);
        m_clyde->set_logic_state( Ghost::L_CHASE);
    }
    if( m_frame == m_settings.chase.at(1)*60) { // fin 2e chase
        cout << "debut scatter 3" << endl;
        m_blinky->set_logic_state(Ghost::L_SCATTER);
        m_pinky->set_logic_state( Ghost::L_SCATTER);
        m_inky->set_logic_state(  Ghost::L_SCATTER);
        m_clyde->set_logic_state( Ghost::L_SCATTER);
    }
    if( m_frame == m_settings.scatter.at(2)*60) { // fin 3e scatter
        cout << "debut chase 3" << endl;
        m_blinky->set_logic_state(Ghost::L_CHASE);
        m_pinky->set_logic_state( Ghost::L_CHASE);
        m_inky->set_logic_state(  Ghost::L_CHASE);
        m_clyde->set_logic_state( Ghost::L_CHASE);
    }
    if( m_frame == m_settings.chase.at(2)*60) { // fin 3e chase
        cout << "debut scatter 4" << endl;
        m_blinky->set_logic_state(Ghost::L_SCATTER);
        m_pinky->set_logic_state( Ghost::L_SCATTER);
        m_inky->set_logic_state(  Ghost::L_SCATTER);
        m_clyde->set_logic_state( Ghost::L_SCATTER);
    }
    if( m_frame == m_settings.scatter.at(3)*60) { // fin dernier scatter
        cout << "debut chase infinie" << endl;
        m_blinky->set_logic_state(Ghost::L_CHASE);
        m_pinky->set_logic_state( Ghost::L_CHASE);
        m_inky->set_logic_state(  Ghost::L_CHASE);
        m_clyde->set_logic_state( Ghost::L_CHASE);
    }
    if(m_level==1) {
        if(m_nb_dot_eaten >= 30 && m_inky->is_idling() && m_frame > 120) {
            m_inky->set_is_starting(true);
        }
        if(m_nb_dot_eaten>=90 && m_clyde->is_idling() && m_frame > 240) {
            m_clyde->set_is_starting(true);
        }
    }
    else if(m_level == 2) {
        if(m_frame == 0) {
            
            m_inky->set_is_starting(true);
        }
        if(m_nb_dot_eaten==30 && m_clyde->is_idling()) {
            m_clyde->set_is_starting(true);
        }
    }
}

double Logic::compute_dx(double speed, Direction::Direction dir) {
    if(dir == Direction::Left)
        return 0-speed;
    if(dir == Direction::Right)
        return speed;
    return 0;
}

double Logic::compute_dy(double speed, Direction::Direction dir) { 
    if(dir == Direction::Up)
        return 0-speed;
    if(dir == Direction::Down)
        return speed;
    return 0;
}

void Logic::move_pacman() {
    if(m_pacman->is_stopped()) return;
    
    Direction::Direction last_direction=m_pacman->get_direction();
    static bool second_try = false;
    double speed = m_pacman->get_speed();
    // std::cout << m_pacman->get_direction() << std::endl;
    Direction::Direction dir = m_pacman->get_next_dir();
    double dx = compute_dx(speed, dir);
    double dy = compute_dy(speed, dir);
    bool cancel_change(false);
    
    double cur_x = m_pacman->get_x();
    double cur_y = m_pacman->get_y();
    std::pair<int,int> tile = m_pacman->get_tile();
    //cas particulier : tunnel
    if( int(cur_y) == 140 
    && (dir == Direction::Right || dir == Direction::Left)) {
        if(cur_x + dx > 219
        && dir == Direction::Right) {
            if(cur_x + dx >= 224) {
                m_pacman->set_x(cur_x+dx-224);
            }
            else {
                m_pacman->move(dx,dy);
            }
            return;
        }
        if(cur_x + dx < 5
        && dir == Direction::Left) {
            if(cur_x + dx < 0) {
                m_pacman->set_x(222-cur_x-dx);
            }
            else {
                m_pacman->move(dx,dy);
            }
            return;
        }
    }

    if (dy != 0) { // déplacement vertical
        if( int(cur_y)%8 == 4 ) {
            if(( int(cur_x)%8 != 4 && false )
            || (dy > 0 && m_terrain->is_wall(tile.first,tile.second+1) )
            || (dy < 0 && m_terrain->is_wall(tile.first,tile.second-1) ))
            {
                cancel_change = true;
            }
        }
        if(int(cur_x)%8 > 4) {
            dx -= speed;
        }
        else if(int(cur_x)%8 < 4) {
            dx += speed;
        }
    }
    else if (dx != 0) { // déplacement horizontal
        if( int(cur_x)%8 == 4 ) {
            if(( int(cur_y)%8 != 4 && false)
            || (dx > 0 && m_terrain->is_wall(tile.first+1,tile.second) )
            || (dx < 0 && m_terrain->is_wall(tile.first-1,tile.second) ))
            {
                cancel_change = true;
            }
        }
        if(int(cur_y)%8 > 4) {
            dy -= speed;
        }
        else if(int(cur_y)%8 < 4) {
            dy += speed;
        }
    }

    if (cancel_change) {
        if(!second_try) { // on relance la fonction avec la direction précédente
        // utile quand on tente de faire un mouvement interdit
        // une seule fois, parce que sinon on tombe dans une boucle infinie
        // lorsque pacman rentre dans un mur
            m_pacman->set_next_dir(last_direction);
            second_try = true;
            move_pacman();
            second_try=false;
        }
    }
    else {
        m_pacman->move(dx,dy);
        m_pacman->set_direction(dir);
    }
}

void Logic::move_ghost(std::shared_ptr<Ghost> ghost) {
    if(ghost->is_stopped()) return;
    if(ghost->is_idling()) {
        this->do_idle_ghost(ghost);
        return;
    }
    if(ghost->is_starting()) {
        this->do_get_out(ghost);
        return;
    }
    if(ghost->get_state() == Ghost::SCORE) {
        ghost->inc_counter_anim();
        if (ghost->get_counter_anim() > 100) {
            ghost->set_state(Ghost::EATEN);
            ghost->update_dir_retour();
        }
        return;
    }
    if(ghost->is_eaten()) {
        // rentrer en ligne droite au centre
        std::pair<double,double> delta = ghost->get_dir_retour();
        ghost->move(delta.first,delta.second);
        // cout << ghost->get_pos() << " = " << int(ghost->get_x()) << " " << int(ghost->get_y()) << endl;
        int x = int(ghost->get_x());
        int y = int(ghost->get_y());
        if( 96 < x && x < 127
        &&  136 < y && y < 143 ) {
            // cout << "ARRIVE" << endl;
            ghost->set_pos(112,140);
            ghost->set_is_eaten(false);
            ghost->set_state(Ghost::NORMAL);
            ghost->set_logic_state(Ghost::L_RECOVER);
            ghost->set_is_starting(true);
        }
        return;
    }
    static bool second_try = false;

    double speed;
    Direction::Direction dir = ghost->get_direction();
    Ghost::LogicState state = ghost->get_logic_state();
    bool cancel_change(false);
    
    double cur_x = ghost->get_x();
    double cur_y = ghost->get_y();
    std::pair<int,int> tile = ghost->get_tile();
    // std::cout << tile.first << " " << tile.second << std::endl;

    if( m_terrain->is_tunnel(tile) ) {
        speed = m_settings.g_speed_tunnel;
    }
    else if(state == Ghost::L_FRIGHT) {
        speed = m_settings.g_speed_fright;
    }
    else if(ghost->is_eaten()) {
        speed = 1.5; // vite vite retour au centre
    }
    else {
        speed = m_settings.g_speed;
    }
    double dx = compute_dx(speed, dir);
    double dy = compute_dy(speed, dir);
    //cas particulier : tunnel
    if( int(cur_y) == 140 
    && (dir == Direction::Right || dir == Direction::Left)) {
        if(cur_x + dx > 219
        && dir == Direction::Right) {
            if(cur_x + dx >= 224) {
                ghost->set_x(cur_x+dx-224);
            }
            else {
                ghost->move(dx,dy);
            }
            return;
        }
        if(cur_x + dx < 5
        && dir == Direction::Left) {
            if(cur_x + dx < 0) {
                ghost->set_x(222-cur_x-dx);
            }
            else {
                ghost->move(dx,dy);
            }
            return;
        }
    }
    
    if(!ghost->is_locked_direction()
    && int(cur_x)%8 == 4 
    && int(cur_y)%8 == 4 
    && m_terrain->is_intersection(tile) ) {
        // cout << "en " << tile << endl;
        if(state != Ghost::L_FRIGHT) {
            ghost->compute_target();
            auto possib = m_terrain->get_possib(ghost);
            std::pair<int,int> target = ghost->get_target();
            double best_dist = 10000;
            Direction::Direction best_dir;
            for(Direction::Direction test_dir : possib) {
                std::pair<int,int> test_tile = Direction::tile_in_dir(tile, test_dir);

                double test_dist = distance(target,test_tile);
                // cout << "  test de " << test_tile 
                // << "( " << test_dir 
                // << " dist : " << test_dist << endl;
                if(test_dist < best_dist) {
                    best_dist = test_dist;
                    best_dir = test_dir;
                }
            }
            if(best_dist != 10000) {
                ghost->set_direction(best_dir);
                dir = best_dir;
                dx = compute_dx(speed, dir);
                dy = compute_dy(speed, dir);
            }
            // std::cout << "best dir : " << best_dir << " " << best_dist << std::endl;
            // std::cout << "target : " << target.first << " " << target.second << std::endl;
            ghost->set_locked_direction(true);
        }
        else if(state == Ghost::L_FRIGHT) {
            auto possib = m_terrain->get_possib(ghost);
            std::uniform_int_distribution<int> dis(0, 3);
            int rd = dis(m_rng);
            Direction::Direction test_dir = static_cast<Direction::Direction>(rd);
            std::pair<int,int> test_tile = tile;
            switch(test_dir) {
                case Direction::Up: test_tile.second -= 1; break;
                case Direction::Left: test_tile.first -= 1; break;
                case Direction::Down: test_tile.second += 1; break;
                case Direction::Right: test_tile.first += 1; break;
            } 
            if( (! m_terrain->is_wall(test_tile)) && test_dir != dir ) {
                ghost->set_direction(test_dir);
            }
            else if( test_dir != Direction::Up
                  || ! m_terrain->is_wall( Direction::tile_in_dir(tile,Direction::Up) )) {
                ghost->set_direction(Direction::Up);
            }
            else if( test_dir != Direction::Left
                  || ! m_terrain->is_wall( Direction::tile_in_dir(tile,Direction::Left) )) {
                ghost->set_direction(Direction::Left);
            }
            else if( test_dir != Direction::Down
                  || ! m_terrain->is_wall( Direction::tile_in_dir(tile,Direction::Down) )) {
                ghost->set_direction(Direction::Down);
            }
            else if( test_dir != Direction::Right
                  || ! m_terrain->is_wall( Direction::tile_in_dir(tile,Direction::Right) )) {
                ghost->set_direction(Direction::Right);
            }
            ghost->set_locked_direction(true);
            dir = ghost->get_direction();
            dx = compute_dx(speed, dir);
            dy = compute_dy(speed, dir);
        }
    }
    else {
        ghost->set_locked_direction(false);
    }

    if (dy != 0) { // déplacement vertical
        if( int(cur_y)%8 == 4 ) {
            if(( int(cur_x)%8 != 4 )
            || (dy > 0 && m_terrain->is_wall(tile.first,tile.second+1) )
            || (dy < 0 && m_terrain->is_wall(tile.first,tile.second-1, true) ))
            {
                cancel_change = true;
            }
        }
    }
    if (dx != 0) { // déplacement horizontal
        if( int(cur_x)%8 == 4 ) {
            if(( int(cur_y)%8 != 4 )
            || (dx > 0 && m_terrain->is_wall(tile.first+1,tile.second) )
            || (dx < 0 && m_terrain->is_wall(tile.first-1,tile.second) ))
            {
                cancel_change = true;
            }
        }
    }
    if(!cancel_change) {
        ghost->move(dx,dy);
    }
    else if(!second_try) {
        auto possib = m_terrain->get_possib(ghost);
        ghost->set_direction( possib.at(0) );
        // std::cout << "bonk " << possib.at(0) << std::endl;
        second_try = true;
        move_ghost(ghost);
        second_try=false;
    }
    if(ghost->is_force_reverse_pending()) {
        ghost->set_direction( Direction::reverse(ghost->get_direction()) );
    }
}

void Logic::do_idle_ghost(std::shared_ptr<Ghost> ghost) {
    int frame = ghost->get_counter_anim();
    
    if(frame%2 == 0) {
        if( (frame+10)%40 < 20) {
            ghost->move(0,-1);
            ghost->set_direction(Direction::Up);
        }
        else {
            ghost->move(0,1);
            ghost->set_direction(Direction::Down);

        }
    }
    ghost->inc_counter_anim();
}

void Logic::do_get_out(std::shared_ptr<Ghost> ghost) {
    std::pair<int,int> pos = ghost->get_pos();
    // std::cout << pos.first << " " << pos.second << std::endl;
    int frame = ghost->get_counter_anim();
    if( pos.first != 112 && frame%2==0 ) {
        if(pos.first > 112) {
            ghost->move(-1,0);
            ghost->set_direction(Direction::Left);

        }
        else {
            ghost->move(1,0);
            ghost->set_direction(Direction::Right);
        }
        if( ghost->get_x() == 112 ) {
            ghost->reset_counter_anim();
        }
        else {
            ghost->inc_counter_anim();
        }
    }
    else {
        if(pos.second != 116 and frame%2==0) {
            ghost->move(0,-1);
            ghost->set_direction(Direction::Up);
        }
        ghost->inc_counter_anim();
        if(ghost->get_y() == 116) {
            ghost->set_direction(Direction::Left);
            ghost->reset_counter_anim();
            ghost->set_is_starting(false);
        }
    }
}

void Logic::check_collision_ghost(std::shared_ptr<Ghost> ghost) {
    if( ghost->is_eaten()) return;
    int energy_left = m_pacman->is_energized();
    if(energy_left == 1) {
        // fin du fright
        // cout <<"gethere" <<endl;
        ghost->set_logic_state(Ghost::L_RECOVER);
        ghost->set_state(Ghost::NORMAL);
    }
    else if(energy_left == 120) {
        // cout <<"fright_end almost" <<endl;
        ghost->set_state(Ghost::FRIGHT_END);   
    }
    if( m_pacman->get_tile() == ghost->get_tile() ) {
        if(m_pacman->is_energized() && ghost->get_logic_state() == Ghost::L_FRIGHT) {
            // cout << "MANGER" << endl;
            ghost->set_score(m_current_ghost_score);
            m_score += m_current_ghost_score;
            m_current_ghost_score *= 2;
            ghost->set_state(Ghost::SCORE);
            ghost->set_is_eaten(true);
            ghost->reset_counter_anim(); // pour compter le nbre de frame
            // depuis qu'il s'est fait mangerr
        }
        else { // perte d'une vie
            m_pacman->set_is_dead(true);
            m_compteur_reset = 88+60+20;
            m_life -= 1;
        }
    }
}

int Logic::get_life() {
    return m_life;
}

void Logic::set_compteur_reset(int a) {
    m_compteur_reset = a;
}

bool Logic::is_paused() {
    return m_paused;
}
void Logic::set_paused(bool b) {
    m_paused = b;
}

bool Logic::show_ready() {
    return m_compteur_reset > 0 && m_compteur_reset < 80 && m_life!=-1;
}

bool Logic::is_finished() {
    return m_finished;
}