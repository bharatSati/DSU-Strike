#include <iostream>
#include "entity.h"

size_t Entity::id () const
{
    return m_id;
}


bool Entity::alive () const
{
    return m_alive;
}


const std::string& Entity::tag () const
{
    return m_tag;
}


void Entity::destroy ()
{
    m_alive = 0;

}