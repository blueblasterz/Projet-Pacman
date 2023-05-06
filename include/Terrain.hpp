#pragma once

#include <array>
#include <string>
#include <memory>


#include "Entity.hpp" // inclut pour l'énumération de directions

// on aurait pu en faire une class, mais ça parraissait un peu inutile
namespace Tile {
    enum Tile : int;
    std::string to_str(Tile t);
}


class Terrain {
public:
    Terrain();
    ~Terrain();

    void print_terrain();

    bool is_intersection(std::pair<double,double> pos);

    bool is_wall(int x, int y);

    // int action_pacman(std::pair<double,double> pos);


protected:

    std::array<std::array<Tile::Tile,36>,28> m_tiles;
    
    // est-ce qu'une case est une intersection ?
    // calculé une seule fois au début de l'exécution
    // parce qu'il serait très pénible de le faire à la main
    std::array<std::array<bool,36>,28> m_intersec;
};