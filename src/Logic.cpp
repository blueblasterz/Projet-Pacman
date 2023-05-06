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
    // move_ghost(m_blinky);
    // move_ghost(m_clyde);
    // move_ghost(m_pinky);
    // move_ghost(m_inky); // inky utilisera la nouvelle position de blinky
}

void Logic::move_pacman() {
    static Direction::Direction last_direction=Direction::Left;
    static bool second_try = false;
    double speed = m_pacman->get_speed();
    // std::cout << m_pacman->get_direction() << std::endl;
    double dx(0);
    double dy(0);
    bool cancel_change(false);
    switch(m_pacman->get_direction()) {
        case Direction::Up:
            dy -= speed;
            break;
        case Direction::Down:
            dy += speed;
            break;
        case Direction::Left:
            dx -= speed;
            break;
        case Direction::Right:
            dx += speed;
            break;
        default:
            std::cerr << "Problème de direction pour pacman" << std::endl;
    }
    double cur_x = m_pacman->get_x();
    double cur_y = m_pacman->get_y();
    std::pair<int,int> tile = m_pacman->get_tile();
    Direction::Direction dir = m_pacman->get_direction();
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

}