#include "Clyde.hpp"
#include "Pacman.hpp"
#include <iostream>

Clyde::Clyde(
    double x,
    double y,
    std::shared_ptr<Pacman> &pacman)
:   Ghost(x,y,pacman, {0,35})
{

}


Clyde::~Clyde() {
    
}


void Clyde::compute_target() {
    /*
    Pour Clyde (le jaune orange),
    s'il est à plus de 8 cases de PACMAN, alors il le vise directement
    sinon, il vise son scatter_target 
    */
    // std::cout << "Clyde : compute_target" << std::endl;
    // std::cout << "distance : " << this->dist(m_pacman) << "px  /8 = " << this->dist(m_pacman)/8. << std::endl;
    if(m_state==Ghost::SCATTER || this->dist(m_pacman)/8. <= 8.) {
        m_target = m_scatter_target;
    }
    else {
        m_target = m_pacman->get_tile();
    }
}