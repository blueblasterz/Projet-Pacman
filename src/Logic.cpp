#include "Logic.hpp"

#include <iostream>
#include <cmath>

#include "Pacman.hpp"
#include "Ghost.hpp"
#include "Pinky.hpp"
#include "Blinky.hpp"
#include "Inky.hpp"
#include "Clyde.hpp"
#include "Terrain.hpp"

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
    m_terrain(terrain)
{
    // m_pacman->print_position();
}

void Logic::do_frame() {
    move_pacman();
    int eaten = m_terrain->add_eaten(m_pacman->get_tile());
    if(eaten == 1) {
        std::cout << "DOT" << std::endl;
    }
    else if(eaten == 2) {
        std::cout << "SUPERDOT" << std::endl;
    }
    // les fantomes utilisent la nouvelle position de pacman
    move_ghost(m_blinky);
    move_ghost(m_clyde);
    move_ghost(m_pinky);
    move_ghost(m_inky); // inky utilisera la nouvelle position de blinky
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
    static Direction::Direction last_direction=Direction::Left;
    static bool second_try = false;
    double speed = m_pacman->get_speed();
    // std::cout << m_pacman->get_direction() << std::endl;
    Direction::Direction dir = m_pacman->get_direction();
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
            if(( int(cur_x)%8 != 4 )
            || (dy > 0 && m_terrain->is_wall(tile.first,tile.second+1) )
            || (dy < 0 && m_terrain->is_wall(tile.first,tile.second-1) ))
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

    if (cancel_change) {
        if(!second_try) { // on relance la fonction avec la direction précédente
        // utile quand on tente de faire un mouvement interdit
        // une seule fois, parce que sinon on tombe dans une boucle infinie
        // lorsque pacman rentre dans un mur
            m_pacman->set_direction(last_direction);
            second_try = true;
            move_pacman();
            second_try=false;
        }
    }
    else {
        m_pacman->move(dx,dy);
        last_direction = dir;
    }
}

void Logic::move_ghost(std::shared_ptr<Ghost> ghost) {
    static bool second_try = false;

    double speed = ghost->get_speed();
    Direction::Direction dir = ghost->get_direction();
    bool cancel_change(false);
    
    double cur_x = ghost->get_x();
    double cur_y = ghost->get_y();
    std::pair<int,int> tile = ghost->get_tile();
    // std::cout << tile.first << " " << tile.second << std::endl;

    if( m_terrain->is_tunnel(tile) ) {
        speed = ghost->get_tunnel_speed();
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
        ghost->compute_target();
        auto possib = m_terrain->get_possib(ghost);
        std::pair<int,int> target = ghost->get_target();
        double best_dist = 10000;
        Direction::Direction best_dir;
        for(Direction::Direction test_dir : possib) {
            std::pair<int,int> test_tile = tile;
            switch(test_dir) {
                case Direction::Up: test_tile.second -= 1; break;
                case Direction::Left: test_tile.first -= 1; break;
                case Direction::Down: test_tile.second += 1; break;
                case Direction::Right: test_tile.first += 1; break;
            } 
            double test_dist = distance(target,test_tile);
            // std::cout << m_terrain->is_gate(tile) << "dir : " << test_dir << " = " << test_dist << std::endl;
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
}