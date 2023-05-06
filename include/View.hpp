#include "Entity.hpp"
#include "Terrain.hpp"

class View {
public:
    View(
        Entity* pacman,
        Entity* ghost_red,
        Entity* ghost_cyan,
        Entity* ghost_pink,
        Entity* ghost_yellow,
        Terrain* terrain );
    ~View();
protected:
    // pointeurs vers les modèles (Pacman, ghost, labyrinthe..)
    // les pointeurs vers les entitées peuvent etre des entitées
};