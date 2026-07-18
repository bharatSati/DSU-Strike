#pragma once

#include <map>
#include "entity.h"


typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::map<std::string, EntityVector> EntityMap;

class EntityManager
{
    EntityVector m_entities; // to store ptr to all entity
    EntityMap m_entityMap; // tag wise access
    EntityVector m_toadd;  // for delayed effect
    size_t m_total = 0; // total entity constructed count to distinguish entity uniquely

public:
    EntityManager () {}
    
    void update ();

    std::shared_ptr<Entity> addEntity (const std::string &tag);

    size_t totalCreated ();

    const EntityVector& getEntities () const;

    const EntityMap& getEntityMap () const;

};