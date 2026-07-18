#pragma once

#include <memory>
#include <string>
#include "components.h"


class Entity
{
    size_t m_id = 0;
    std::string m_tag = "default";
    bool m_alive = true;

    // pvt constructor
    Entity () {}
    Entity (const size_t id,const std::string &tag)
        :   m_id(id)
        ,   m_tag(tag)
    {

    }
    friend class EntityManager;

public:
    // pointers to the component
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CShape> cShape;
    // std::shared_ptr<CScore> cScore;
    std::shared_ptr<CLifespan> cLifespan;
    std::shared_ptr<CInput> cInput;



    // functions to provide pvt member access
    size_t id () const;
    bool alive () const;
    const std::string& tag () const;
    void destroy ();

};