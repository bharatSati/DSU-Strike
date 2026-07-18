#include "entityManager.h"

void EntityManager::update ()
{
    // delayin addition to next frame 
    // to counter iteration invalidation
    for(auto &it : m_toadd){
        m_entities.push_back(it);
        m_entityMap[it -> tag()].push_back(it);
    }
    m_toadd.clear();

    
    
    // handling removal of entitites
    // removal from EntityVector
    size_t i = 0;
    while(i < m_entities.size())
    {
        if(!(m_entities[i] -> alive()))
        {
            m_entities[i] = m_entities.back();
            m_entities.pop_back();
        }
        else
        {
            i++;
        }
    }

    // removal from EntityMap
    for(auto &it : m_entityMap)
    {
        size_t i = 0;
        while(i < it.second.size())
        {
            if(!(it.second[i] -> alive()))
            {
                it.second[i] = it.second.back();
                it.second.pop_back();
            }
            else
            {
                i++;
            }
        }

    }
}

std::shared_ptr<Entity> EntityManager::addEntity (const std::string &tag)
{
    m_total += 1; 
    auto it = std::shared_ptr<Entity> (new Entity(m_total,tag));
    m_toadd.push_back(it);
    return it;
}

size_t EntityManager::totalCreated ()
{
    return m_total;
}


const EntityVector& EntityManager::getEntities () const
{
    return m_entities;
}

const EntityMap& EntityManager::getEntityMap () const
{
    return m_entityMap;
}