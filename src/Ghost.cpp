#include "Ghost.hpp"

#include "Pacman.hpp"


Ghost::Ghost() {

}

Ghost::Ghost(
    double x,
    double y,
    std::shared_ptr<Pacman> &pacman,
    std::pair<int,int> scatter_pos
):  Entity(x,y),
    m_scatter_target(scatter_pos),
    m_pacman(pacman),
    m_scatter(false) {
    
}
Ghost::Ghost(
    std::pair<int,int> pos,
    std::shared_ptr<Pacman> &pacman,
    std::pair<int,int> scatter_pos
):  Ghost(pos.first, pos.second, pacman,scatter_pos)
{
    
}

Ghost::~Ghost() {}

std::pair<int,int> Ghost::get_target() {
    return m_target;
}