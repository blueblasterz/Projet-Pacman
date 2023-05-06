#ifndef VIEW_HPP
#define VIEW_HPP

#include "Entity.hpp"
#include "Terrain.hpp"

class View {
public:
    View(
        Entity* pacman,
        Entity* blinky,
        Entity* pinky,
        Entity* inky,
        Entity* clyde,
        Terrain* terrain );
    ~View();
protected:
    // pointeurs vers les modèles (Pacman, ghost, labyrinthe..)
    // les pointeurs vers les entitées peuvent etre des entitées
};

#endif