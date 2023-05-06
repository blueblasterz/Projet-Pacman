#include "View.hpp"
#include "Player.hpp" // faudra mettre les bons types
#include "Terrain.hpp"
#include "Logic.hpp"

class Jeu {
public:
    //méthodes

protected:
// à instancier dans le constructeur
    Player* m_pacman;
    // ghosts...

    Terrain* m_terrain;



    View* m_vue; // changer pointeurs

    Logic* m_logique;

};