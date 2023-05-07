#include "Inky.hpp"

#include "Pacman.hpp"
#include "Blinky.hpp"

Inky::Inky(
    double x,
    double y,
    std::shared_ptr<Pacman> &pacman,
    std::shared_ptr<Blinky> &blinky)
:   Ghost(x,y,pacman, {35,27}),
    m_blinky(blinky) {

}

Inky::~Inky() {
    
}

void Inky::compute_target() {
    /*
    Pour Inky (le cyan) :
    prend la position de blinky (rouge) -> pos_blinky

    prend la position 2 cases devant pacman (ou 2 gauche 2 haut si direction=Up) 
    -> pos_pacman_decal

    vise 2*( pos_pacman_decal - pos_blinky) 
    */

    if(m_state==Ghost::SCATTER) {
        m_target = m_scatter_target;
    }
    else {
        std::pair<int,int> pos_blinky = m_blinky->get_tile();
        std::pair<int,int> pos_pacman_decal = m_pacman->get_tile(2);
        m_target = std::make_pair(
            2*pos_pacman_decal.first - pos_blinky.first,
            2*pos_pacman_decal.second - pos_blinky.second
        );
    }
}