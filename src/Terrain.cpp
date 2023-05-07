#include "Terrain.hpp"

#include <iostream>
#include <cmath>
#include "tilemap.hpp"

using std::cout, std::cin, std::endl;


enum Tile::Tile : int {
    Empty = 0,
    Wall,
    Gate,
    Dot,
    SuperDot,
    Tunnel,
    Error
};

std::string Tile::to_str(Tile t) {
    switch(t) {
        case Tile::Empty:
            return " ";
        case Tile::Wall:
            return "W";
        case Tile::Gate:
            return "-";
        case Tile::Dot:
            return "·";
        case Tile::SuperDot:
            return "●";
        case Tile::Tunnel:
            return " ";
        case Tile::Error:
            return "X";
    }
    return "X";
}

Terrain::Terrain() {
    for(int y=0;y<36;y++) {
        for(int x=0;x<28;x++) {
            m_intersec.at(x).at(y) = false;
        }
    }
    this->load_tilemap();
}

Terrain::~Terrain() {}

void Terrain::load_tilemap() {
    int x = 0;
    int y = 0;
    for(char c : TILEMAP) {
        switch(c) {
            case '\n':
                x = 0;
                y++;
                break;
            case 'E':
                m_tiles.at(x).at(y) = Tile::Empty;
                x++;
                break;
            case 'F':
                m_tiles.at(x).at(y) = Tile::Empty;
                m_intersec.at(x).at(y) = true;
                x++;
                break;
            case 'W':
                m_tiles.at(x).at(y) = Tile::Wall;
                x++;
                break;
            case 'X':
                m_tiles.at(x).at(y) = Tile::Gate;
                x++;
                break;
            case 'D':
                m_tiles.at(x).at(y) = Tile::Dot;
                x++;
                break;
            case 'O':
                m_tiles.at(x).at(y) = Tile::Dot;
                m_intersec.at(x).at(y) = true;
                x++;
                break;
            case 'S':
                m_tiles.at(x).at(y) = Tile::SuperDot;
                x++;
                break;
            case 'T':
                m_tiles.at(x).at(y) = Tile::Tunnel;
                x++;
                break;
            default:
                if(y!=36) m_tiles.at(x).at(y) = Tile::Error;
                x++;
        }      
    }
    m_eaten_tiles.clear();
}

void Terrain::print_terrain() {
    for(int y=0 ; y<36 ; y++) {
        for(int x=0 ; x<28 ; x++) {
            cout << Tile::to_str(m_tiles.at(x).at(y)) << " ";
        }
        cout << endl;
    }
}

bool Terrain::is_intersection(std::pair<double,double> pos) {
    return m_intersec.at(int(pos.first/8)).at(int(pos.second/8));
}

bool Terrain::is_intersection(std::pair<int,int> pos) {
    return m_intersec.at(pos.first).at(pos.second);
}

bool Terrain::is_wall(int x, int y, bool ignore_gate) {
    if(x < 0 || x > 27 || y < 0 || y > 35) {
        return true;
    }
    return m_tiles.at(x).at(y) == Tile::Wall || (!ignore_gate && m_tiles.at(x).at(y) == Tile::Gate);
}
bool Terrain::is_wall(std::pair<int,int> pos, bool ignore_gate) {
    return this->is_wall(pos.first, pos.second, ignore_gate);
}


bool Terrain::is_tunnel(int x, int y) {
    if(x < 0 || x > 27 || y < 0 || y > 35) {
        return true;
    }
    return m_tiles.at(x).at(y) == Tile::Tunnel;
}
bool Terrain::is_tunnel(std::pair<int,int> pos) {
    return this->is_tunnel(pos.first, pos.second);
}


bool Terrain::is_gate(int x, int y) {
    if(x < 0 || x > 27 || y < 0 || y > 35) {
        return true;
    }
    return m_tiles.at(x).at(y) == Tile::Gate;
}
bool Terrain::is_gate(std::pair<int,int> pos) {
    return this->is_gate(pos.first, pos.second);
}

std::vector<Direction::Direction> Terrain::get_possib(
    std::shared_ptr<Entity> ghost) 
{
    std::vector<Direction::Direction> res;
    std::pair<int,int> pos = ghost->get_tile();
    std::pair<int,int> test_pos = pos;
    Direction::Direction rev_dir = reverse(ghost->get_direction());

    // UP
    test_pos.first = pos.first;
    test_pos.second = pos.second-1;
    if(this->is_gate(test_pos) ) { // cas particulier : libération des fantomes
        res.push_back(Direction::Up);
        return res;
    }
    if( (rev_dir != Direction::Up && !this->is_wall(test_pos))
    && !( (pos.first == 12 && pos.second == 26) // autre cas part :
        ||(pos.first == 15 && pos.second == 26) // 4 intersections où les fantomes
        ||(pos.first == 12 && pos.second == 14) // n'ont pas le droit de monter
        ||(pos.first == 15 && pos.second == 14) ) ) {
        res.push_back(Direction::Up);
    }
    // LEFT
    test_pos.first = pos.first-1;
    test_pos.second = pos.second;
    if( rev_dir != Direction::Left && !this->is_wall(test_pos) ) {
        res.push_back(Direction::Left);
    }
    // DOWN
    test_pos.first = pos.first;
    test_pos.second = pos.second+1;
    if( rev_dir != Direction::Down && !this->is_wall(test_pos) ) {
        res.push_back(Direction::Down);
    }
    // RIGHT
    test_pos.first = pos.first+1;
    test_pos.second = pos.second;
    if( rev_dir != Direction::Right && !this->is_wall(test_pos) ) {
        res.push_back(Direction::Right);
    }
    return res;
}


const std::vector<std::pair<int,int>> & Terrain::get_eaten() {
    return m_eaten_tiles;
}

int Terrain::add_eaten(std::pair<int,int> pos) {
    if(pos.first < 0  || pos.first > 27 
    || pos.second < 0 || pos.second > 35) {
        return 0;
    }
    Tile::Tile tile = m_tiles.at(pos.first).at(pos.second);
    if( tile == Tile::Dot || tile == Tile::SuperDot ) {
        m_eaten_tiles.push_back(pos);
        m_tiles.at(pos.first).at(pos.second) = Tile::Empty;
        return (tile == Tile::Dot) ? 1 : 2;
    }
    return 0;
}