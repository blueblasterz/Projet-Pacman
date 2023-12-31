#include "Blinky.hpp"

#include "Pacman.hpp"

Blinky::Blinky(
    double x,
    double y,
    std::shared_ptr<Pacman> &pacman)
:   Ghost(x,y,pacman, {25,0})
{

}

Blinky::~Blinky() {
    
}


void Blinky::compute_target() {
    /*
    Pour Blinky (le rouge), c'est très facile :
    il ne fait que viser Pacman directement.
    */
    if(m_logic_state==Ghost::L_SCATTER) {
        m_target = m_scatter_target;
    }
    else {
        m_target = m_pacman->get_tile();
    }
}