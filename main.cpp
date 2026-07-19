#include "Vec2.h"
#include "entity.h"
#include "game.h"
#include <iostream>
#include <ctime>

int main() {

    srand(time(nullptr));
    // EntityManager entities;
    // entities.addEntity("bharat");
    // entities.update();

    // EntityVector vec = entities.getEntities(); 
    // std::cout << vec.size() << std::endl;
    // for(auto it : vec) std::cout << it -> tag() << std::endl;

    Game g("config.txt");
    g.run();


    

}