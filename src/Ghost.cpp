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
    m_scatter(false),
    m_tunnel_speed(0.4),
    m_locked_direction(false),
    m_is_caged(true),
    m_dot_counter(0),
    m_state(Ghost::IDLE) {
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


double Ghost::get_tunnel_speed() {
    return m_tunnel_speed;
}
void Ghost::set_tunnel_speed(double speed) {
    if (speed < 0) {
        m_tunnel_speed = 0;
    }
    else if (speed > 1) {
        m_tunnel_speed = 1;
    }
    else {
        m_tunnel_speed = speed;
    }
}

void Ghost::toggle_locked_direction() {
    m_locked_direction = !m_locked_direction;
}
void Ghost::set_locked_direction(bool locked) {
    m_locked_direction = locked;
}
bool Ghost::is_locked_direction() {
    return m_locked_direction;
}

void Ghost::set_is_caged(bool caged) {
    m_is_caged = caged;
}
bool Ghost::is_caged() {
    return m_is_caged;
}

int Ghost::get_counter_anim() {
    return m_counter_anim;
}
void Ghost::inc_counter_anim() {
    m_counter_anim++;
}
void Ghost::reset_counter_anim() {
    m_counter_anim = 0;
}

Ghost::State Ghost::get_state() {
    return m_state;
}
void Ghost::set_state(Ghost::State state) {
    m_state = state;
}