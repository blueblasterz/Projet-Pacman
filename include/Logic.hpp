#pragma once

#include <memory>
#include <random>

#include "Entity.hpp" // pour les directions
#include "settings.hpp"


// forward declaration au lieu d'include
class Pacman;
class Ghost;
class Blinky;
class Clyde;
class Pinky;
class Inky;
class Terrain;

class Logic {
public:
    Logic(
        std::shared_ptr<Pacman>  &pacman,
        std::shared_ptr<Blinky>  &blinky,
        std::shared_ptr<Clyde>   &clyde,
        std::shared_ptr<Pinky>   &pinky,
        std::shared_ptr<Inky>    &inky,
        std::shared_ptr<Terrain> &terrain
    );

    int get_score();
    int add_score(int add);
    void set_score(int score);

    void do_frame();


    double compute_dx(double speed, Direction::Direction dir);
    double compute_dy(double speed, Direction::Direction dir);

    void move_pacman();
    void move_ghost(std::shared_ptr<Ghost> ghost);

    void do_idle_ghost(std::shared_ptr<Ghost> ghost);
    void do_get_out(std::shared_ptr<Ghost> ghost);

    // charge les données du niveau suivant.
    void init_new_level();

    // met les entités à leur point de départ
    void place_entities();

    // les événements timés : changement scatter/chase
    void do_events();

    // vérifie s'il y a des fantomes mangés
    // et gère la transition de FRIGHT à NORMAL
    void check_collision_ghost(std::shared_ptr<Ghost> ghost);

    void set_compteur_reset(int a);

    int get_life();


    void set_paused(bool b);
    bool is_paused();

    bool show_ready();

    bool is_finished();

protected:
    // pointeurs vers les modèles (Pacman, ghost..)
    std::shared_ptr<Pacman> m_pacman;
    std::shared_ptr<Blinky> m_blinky;
    std::shared_ptr<Clyde> m_clyde;
    std::shared_ptr<Pinky> m_pinky;
    std::shared_ptr<Inky> m_inky;
    std::shared_ptr<Terrain> m_terrain;

    // le score de PACMAN
    int m_score;
    // nombre de frames qui se sont écoulées depuis le début du niveau
    int m_frame;
    // le niveau actuel (modifie la difficulté)
    int m_level;
    // les données du niveau actuel (vitesse, timings etc.)
    Setting::level m_settings;

    // est-ce que pacman a mangé un SUPER PAC-GUM 9000™ ???
    bool m_fright;

    // nbre de points mangés depuis le début du niveau
    int m_nb_dot_eaten;

    std::mt19937 m_rng;

    // le score gagné en mangeant un fantome
    // double à chaque fantome mangé avec le meme superdot
    int m_current_ghost_score;

    // nombre de vies
    int m_life;

    // compteur utilisé au moment du reset
    int m_compteur_reset;

    // nb de frame depuis le dernier mangeage de dot
    int m_frame_last_eaten;

    // est-ce que le jeu est en pause ?
    bool m_paused;

    // est-ce que le jeu est fini ?
    bool m_finished;
};