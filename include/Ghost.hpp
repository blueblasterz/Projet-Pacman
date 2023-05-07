#pragma once

#include <memory>

#include "Entity.hpp"

class Pacman;


class Ghost : public Entity{
public:
    Ghost();
    Ghost(
        double x,
        double y,
        std::shared_ptr<Pacman> &pacman,
        std::pair<int,int> scatter_pos);
    Ghost(
        std::pair<int,int> pos,
        std::shared_ptr<Pacman> &pacman,
        std::pair<int,int> scatter_pos);
    ~Ghost();

    enum State {
        CHASE,    // chasse pacman
        SCATTER,  // vise le coin
        FRIGHT,   // appeuré
        END_FRIGHT, // appeuré et clignotant
        EATEN,    // quand le fantome a été mangé et doit revenir au centre
        IDLE,     // bloqué au centre
        STARTING  // en train de sortir du centre
    };

    virtual void compute_target() =0;
    
    std::pair<int,int> get_target();

    double get_tunnel_speed();
    void set_tunnel_speed(double speed);

    void toggle_locked_direction();
    void set_locked_direction(bool locked);
    bool is_locked_direction();

    void set_is_caged(bool caged);
    bool is_caged();

    int get_counter_anim();
    void inc_counter_anim();
    void reset_counter_anim();

    State get_state();
    void set_state(State state);


protected:
    // contient la case visée par le fantome actuellement
    std::pair<int,int> m_target;
    
    // contient la case que le fantome vise quand il est en mode "scatter"
    std::pair<int,int> m_scatter_target;
    
    // pour savoir où est le pacman
    std::shared_ptr<Pacman> m_pacman;

    // true si le fantome est en "scatter", false sinon
    bool m_scatter;

    // vitesse dans le tunnel
    double m_tunnel_speed;

    // est-ce que la direction du fantome est bloquée ?
    bool m_locked_direction;

    // est-ce que le fantome est bloqué dans la cage ?
    // autrement dit est-ce qu'il a le droit de passer la porte
    bool m_is_caged;

    // compteur de dot mangés
    // permet la logique de sortie des fantomes
    bool m_dot_counter;

    // variable qui donne l'état actuel du fantôme
    State m_state;

    // variable pour suivre l'évolution d'une animation
    // (IDLE, STARTING)
    int m_counter_anim;

};