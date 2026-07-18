#include "Vec2.h"
#include "entity.h"
#include <iostream>


int main() {

    Entity e1(1,"bharat");

    e1.cLifespan = std::make_shared<CLifespan>(10);

    auto it = std::make_shared<Entity> (2,"hello");

    it->cLifespan = std::make_shared<CLifespan>(20);
    
    std::cout << e1.cLifespan->total + it->cLifespan->total << std::endl;
    

}