#pragma once

#include <array>
#include <string>

#include "Player.hpp"

// on aurait pu en faire une class, mais ça parraissait un peu inutile
namespace Tile {
    enum Tile : int;
    std::string to_str(Tile t);
}

// meme remarque
namespace Move {
    enum Move : int;
}

class Terrain {
public:
    Terrain();
    ~Terrain();

    void print_terrain();

protected:
    Player * m_player;

    std::array<std::array<Tile::Tile,36>,28> m_tiles;
    
    
};