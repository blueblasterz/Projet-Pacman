#include <memory>
#include "Entity.hpp" // pour les directions
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
    int frame;
    // le niveau actuel (modifie la difficulté)
    int level;
};