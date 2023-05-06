#include "View.hpp"
#include "Pacman.hpp" // faudra mettre les bons types
#include "Terrain.hpp"
#include "Logic.hpp"

class Game {
public:
    //méthodes

protected:
// à instancier dans le constructeur
    Pacman* m_pacman;
    // ghosts...

    Terrain* m_terrain;



    View* m_vue; // changer pointeurs

    Logic* m_logique;

};