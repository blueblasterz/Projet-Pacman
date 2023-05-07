#pragma once

#include <memory>

#include "Entity.hpp"
#include "settings.hpp"

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

    // utilisé pour l'affichage
    enum State {
        CHASE,    // chasse pacman
        SCATTER,  // vise le coin
        FRIGHT,   // appeuré
        FRIGHT_END, // appeuré et clignotant
        SCORE,    // ces quelques frame où le fantome disparait et affiche le score gagné
        EATEN,    // quand le fantome a été mangé et doit revenir au centre
        IDLE,     // bloqué au centre
        STARTING  // en train de sortir du centre
    };

    // utilisé pour la logique
    enum LogicState {
        LCHASE,
        LSCATTER,
        LFRIGHT,
        RECOVER // état spécial pour récupérer l'état enregistrer
    };

    virtual void compute_target() =0;
    
    std::pair<int,int> get_target();

    double get_tunnel_speed();
    void set_tunnel_speed(double speed);

    void toggle_locked_direction();
    void set_locked_direction(bool locked);
    bool is_locked_direction();


    int get_counter_anim();
    void inc_counter_anim();
    void reset_counter_anim();

    State get_state();
    // from_timer doit être vrai si le changement est un changement
    // du au temps qui passe (ie les changements scatter/chase,
    // mais pas les animations, et les superGum)
    void set_state(State state, bool from_timer=false);

    LogicState get_logic_state();
    void set_logic_state(LogicState state);

    bool force_reverse_pending();

    void load_param(Setting::level &  param);

protected:
    // contient la case visée par le fantome actuellement
    std::pair<int,int> m_target;
    
    // contient la case que le fantome vise quand il est en mode "scatter"
    std::pair<int,int> m_scatter_target;
    
    // pour savoir où est le pacman
    std::shared_ptr<Pacman> m_pacman;

    // vitesse dans le tunnel
    double m_tunnel_speed;
    // vitesse de fright
    double m_fright_speed;

    // est-ce que la direction du fantome est bloquée ?
    bool m_locked_direction;

    // compteur de dot mangés
    // permet la logique de sortie des fantomes
    bool m_dot_counter;

    // variable qui donne l'état actuel du fantôme (visuel)
    State m_state;

    // vairable qui donne l'état acutel du fantome (logique)
    LogicState m_logic_state;

    LogicState m_stored_logic_state;

    // variable pour suivre l'évolution d'une animation
    // (IDLE, STARTING)
    int m_counter_anim;
    
    // stocke l'état que prend le fantome après être sorti de la cage
    // peut être soit CHASE, soit SCATTER
    State m_state_buffer;

    // stocke un changement de direction forcé
    // ceci arrive quand un fantome change depuis CHASE ou SCATTER vers FRIGHT
    // on le stocke car si le fantome est encore dans la cage, il devra
    // changer de direction une frame après être sorti
    bool m_force_reverse_dir_pending;

    bool m_state_change_pending;


};