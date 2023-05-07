#include "Ghost.hpp"
#include <iostream>
#include "Pacman.hpp"



std::ostream& operator<<(std::ostream& os, const Ghost::LogicState& st) {
    switch(st) {
        case Ghost::L_CHASE: os << "L_CHASE"; break;
        case Ghost::L_FRIGHT: os << "L_FRIGHT"; break;
        case Ghost::L_SCATTER: os << "L_SCATTER"; break;
        case Ghost::L_RECOVER: os << "L_RECOVER"; break;
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, const Ghost::State& st) {
    switch(st) {
        case Ghost::NORMAL: os << "NORMAL"; break;
        case Ghost::FRIGHT: os << "FRIGHT"; break;
        case Ghost::FRIGHT_END: os << "FRIGHT_END"; break;
        case Ghost::SCORE: os << "SCORE"; break;
        case Ghost::EATEN: os << "EATEN"; break;
    }
    return os;
}


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
    m_tunnel_speed(0.4),
    m_fright_speed(0.5),
    m_locked_direction(false),
    m_dot_counter(0),
    m_state(Ghost::NORMAL),
    m_logic_state(Ghost::L_SCATTER),
    m_stored_logic_state(Ghost::L_SCATTER),
    m_counter_anim(0),
    m_force_reverse_dir_pending(false),
    m_is_idling(false),
    m_is_starting(false),
    m_is_eaten(false),
    m_score(200),
    m_dir_retour({0,0}) {
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

Ghost::LogicState Ghost::get_logic_state() {
    return m_logic_state;
}
void Ghost::set_logic_state(Ghost::LogicState state) {
    // RECOVER est utilisé pour retrouver
    // l'état précédent à FRIGHT
    if(state == LogicState::L_RECOVER) {
        state = m_stored_logic_state;
        // std::cout << "recovered state " << state << m_stored_logic_state << std::endl;
    }
    // on enregistre l'état actuel dans le cas de FRIGHT
    if(state == LogicState::L_FRIGHT && m_logic_state != LogicState::L_FRIGHT) {
        m_stored_logic_state = m_logic_state;
    }
    // si on part de CHASE ou de SCATTER, on force l'inversion de direction
    if(m_logic_state != LogicState::L_FRIGHT
    && m_logic_state != state){
        m_force_reverse_dir_pending = true;
    }
    m_logic_state = state;

    if(m_logic_state == LogicState::L_FRIGHT) {
        this->set_state(Ghost::FRIGHT);
    }
    else {
        this->set_state(Ghost::NORMAL);
    }
}

bool Ghost::is_force_reverse_pending() {
    if(m_force_reverse_dir_pending) {
        m_force_reverse_dir_pending = false;
        return true;
    }
    return false;
}

void Ghost::load_param(Setting::level & param) {
    m_speed = param.g_speed;
    m_tunnel_speed = param.g_speed_tunnel;
    m_fright_speed = param.g_speed_fright;
}

void Ghost::set_is_idling(bool b) {
    m_is_idling = b;
    if(b) {
        m_is_starting = false;
    }
}
bool Ghost::is_idling() {
    return m_is_idling;
}
void Ghost::set_is_starting(bool b) {
    m_is_starting = b;
    if(b) {
        m_is_idling = false;
    }
}
bool Ghost::is_starting() {
    return m_is_starting;
}

void Ghost::set_is_eaten(bool b) {
    m_is_eaten = b;
}
bool Ghost::is_eaten() {
    return m_is_eaten;
}

void Ghost::set_score(int score) {
    m_score = score;
}
int Ghost::get_score() {
    return m_score;
}

void Ghost::update_dir_retour() {
    m_dir_retour = { (112.-m_x)/120., (140.-m_y)/120.};
}
std::pair<double,double> Ghost::get_dir_retour() {
    return m_dir_retour;
}