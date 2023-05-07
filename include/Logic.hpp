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

    void do_frame();


    double compute_dx(double speed, Direction::Direction dir);
    double compute_dy(double speed, Direction::Direction dir);

    void move_pacman();
    void move_ghost(std::shared_ptr<Ghost> ghost);


protected:
    // pointeurs vers les modèles (Pacman, ghost..)
    std::shared_ptr<Pacman> m_pacman;
    std::shared_ptr<Blinky> m_blinky;
    std::shared_ptr<Clyde> m_clyde;
    std::shared_ptr<Pinky> m_pinky;
    std::shared_ptr<Inky> m_inky;
    std::shared_ptr<Terrain> m_terrain;
};