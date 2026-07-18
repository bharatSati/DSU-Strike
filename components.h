#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>


class CTransform
{
public:
    // position , velocity , angle
    Vec2 position = { 0.0 , 0.0 };
    Vec2 velocity = { 0.0 , 0.0 };
    float angle = 0.0;

    CTransform () {}
    CTransform (const Vec2 &pos,const Vec2 &vel,float ang)
        : position(pos)
        , velocity(vel)
        , angle(ang)
    {
    }
};



class CShape
{
public:
    // radious , inside color , no of sides , outline color , thickness 
    sf::CircleShape shape;
    CShape (float r, const sf::Color &bg, int sides, const sf::Color &outline,float thickness)
       :   shape(r, sides)
    {
        shape.setFillColor(bg);
        shape.setOutlineColor(outline);
        shape.setOutlineThickness(thickness);
        shape.setOrigin({ r , r });
    }
};


class CCollision
{
public:
    // sets the collision radious of entity
    float collisionR;
    CCollision () {}
    CCollision (float r)
        : collisionR(r)
    {

    }
};



class CLifespan
{
public:
    // will be used for enemies
    // how long a enemy will survive
    int total = 0; 
    int remaining = 0;
    CLifespan () {}
    CLifespan (int life)
        : total(life)
        , remaining(life)
    {

    }
};



class CInput
{
public:
    // which key is clicked
    // will be used to move our player
    bool up = 0;
    bool down = 0;
    bool right = 0;
    bool left = 0;
    CInput () {}
};