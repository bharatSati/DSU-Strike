#include "Vec2.h"
#include "entity.h"
#include "entityManager.h"
#include <iostream>


int main() {

    
    EntityManager entities;
    entities.addEntity("bharat");
    entities.update();

    EntityVector vec = entities.getEntities(); 
    std::cout << vec.size() << std::endl;
    for(auto it : vec) std::cout << it -> tag() << std::endl;


    

}