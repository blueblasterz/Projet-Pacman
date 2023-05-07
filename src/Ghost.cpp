#include "Ghost.hpp"
#include <iostream>
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
    m_tunnel_speed(0.4),
    m_fright_speed(0.5),
    m_locked_direction(false),
    m_dot_counter(0),
    m_state(Ghost::IDLE),
    m_counter_anim(0),
    m_state_buffer(Ghost::IDLE),
    m_force_reverse_dir_pending(false),
    m_state_change_pending(false) {
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
void Ghost::set_state(Ghost::State state, bool from_timer) {
    if( from_timer ) { // si c'est un changement auto entre scatter et chase
        if( m_state == Ghost::IDLE || m_state == Ghost::STARTING
        || m_state == Ghost::EATEN ) {
            // si on est encore en cage (ou en train d'y revenir), 
            // on stocke l'état pour plus tard
            m_state_buffer = state;
            m_state_change_pending = true;
            m_force_reverse_dir_pending = !m_force_reverse_dir_pending;
            return;
        }
        // un évt de timer ne peux pas arriver pendant FRIGHT / END_FRIGHT
        // car le timer est en pause à ce moment
        // donc on est forcément en CHASE ou SCATTER ici
        if(m_state != state) {
            m_state = state; 
            m_force_reverse_dir_pending = !m_force_reverse_dir_pending;
        }
    }
    else { // l'événement vient du controleur logique
        // dans le cas où on sort de STARTING,
        // on prend (s'il existe) le buffer
        if(m_state == Ghost::STARTING && m_state_change_pending) {
            // std::cout << "delayed change" << std::endl;
            m_state = m_state_buffer;
            return;
        }
        // dans le cas où on entre dans FRIGHT,
        // on enregistre l'état actuel
        if(state == Ghost::FRIGHT) {
            m_state_buffer = m_state;
            m_state = state;
            return;
        }
        // dans le cas où on sort de FRIGHT_END ou de EATEN,
        // on prend le buffer
        if(m_state == Ghost::FRIGHT_END || m_state == Ghost::EATEN) {
            m_state = m_state_buffer;
            return;
        }
        // dans tout les autres cas, on ne fait rien de particulier
        m_state = state;
    }
}

Ghost::LogicState Ghost::get_logic_state() {
    return m_logic_state;
}
void Ghost::set_logic_state(Ghost::LogicState state) {
    // RECOVER est utilisé pour retrouver
    // l'état précédent à FRIGHT
    if(state == LogicState::RECOVER) {
        m_logic_state = m_stored_logic_state;
        return;
    }
    // on enregistre l'état actuel dans le cas de FRIGHT
    if(state == LogicState::LFRIGHT) {
        m_stored_logic_state = m_logic_state;
    }
    // si on part de CHASE ou de SCATTER, on force l'inversion de direction
    if(m_logic_state != LogicState::LFRIGHT){
        m_force_reverse_dir_pending = true;
    }
    m_logic_state = state;
}

bool Ghost::force_reverse_pending() {
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