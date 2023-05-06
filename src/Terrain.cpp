// #include "Terrain.hpp"

// #include <iostream>
// #include "tilemap.hpp"

// using std::cout, std::cin, std::endl;


// enum Tile::Tile : int {
//     Empty = 0,
//     Wall,
//     Dot,
//     SuperDot,
//     Tunnel,
//     Error
// };

// enum Move::Move : int {
//     Left,
//     Right,
//     Up,
//     Down
// };

// std::string Tile::to_str(Tile t) {
//     switch(t) {
//         case Tile::Empty:
//             return " ";
//         case Tile::Wall:
//             return "W";
//         case Tile::Dot:
//             return "·";
//         case Tile::SuperDot:
//             return "●";
//         case Tile::Tunnel:
//             return " ";
//         case Tile::Error:
//             return "X";
//     }
//     return "X";
// }

// Terrain::Terrain() {
//     m_player = new Player();

//     int x = 0;
//     int y = 0;

//     for(char c : TILEMAP) {
//         switch(c) {
//             case '\n':
//                 x = 0;
//                 y++;
//                 break;
//             case 'E':
//                 m_tiles[x][y] = Tile::Empty;
//                 x++;
//                 break;
//             case 'W':
//                 m_tiles[x][y] = Tile::Wall;
//                 x++;
//                 break;
//             case 'D':
//                 m_tiles[x][y] = Tile::Dot;
//                 x++;
//                 break;
//             case 'S':
//                 m_tiles[x][y] = Tile::SuperDot;
//                 x++;
//                 break;
//             case 'T':
//                 m_tiles[x][y] = Tile::Tunnel;
//                 x++;
//                 break;
//             default:
//                 m_tiles[x][y] = Tile::Error;
//                 x++;
//         }      
//     }   
// }

// Terrain::~Terrain() {
//     delete m_player;
// }



// void Terrain::print_terrain() {
//     for(int y=0 ; y<36 ; y++) {
//         for(int x=0 ; x<28 ; x++) {
//             cout << Tile::to_str(m_tiles.at(x).at(y)) << " ";
//         }
//         cout << endl;
//     }
// }