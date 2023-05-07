#include "Pinky.hpp"

#include "Pacman.hpp"

Pinky::Pinky(
    double x,
    double y,
    std::shared_ptr<Pacman> &pacman)
:   Ghost(x,y,pacman, {0,2})
{

}

Pinky::~Pinky() {

}

void Pinky::compute_target() {
    /*
    Pour Pinky (le rose) :
    vise 4 cases devant Pacman

    Sauf si pacman regarde vers le haut, 
    dans ce cas vise 4 case au dessus +4 à gauche
    */
    if(m_state==Ghost::SCATTER) {
        m_target = m_scatter_target;
    }
    else {
        std::pair<double,double> pacman_pos = m_pacman->get_tile(4);
        // switch( m_pacman->get_direction() ) {
        //     case Direction::Up :
        //         pacman_pos.second -= 4;
        //         pacman_pos.first -= 4;
        //         break;
        //     case Direction::Down :
        //         pacman_pos.first += 4;
        //         break; 
        //     case Direction::Left :
        //         pacman_pos.second -= 4;
        //         break;
        //     case Direction::Right:
        //         pacman_pos.second += 4;
        //         break;
        //     default:
        //         break;
        // }
        m_target = pacman_pos;
    }
}