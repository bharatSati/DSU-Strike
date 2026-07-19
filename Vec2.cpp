#include <math.h>
#include <Vec2.h>

Vec2::Vec2() 
{

}


Vec2::Vec2(float xVal, float yVal) 
    : x(xVal)
    , y(yVal)
{

}


bool Vec2::operator == (const Vec2 &rhs) const
{
    return ((rhs.x == x) && (rhs.y == y));
}


bool Vec2::operator != (const Vec2 &rhs) const
{
    return ((rhs.x != x) || (rhs.y != y));
}


Vec2 Vec2::operator + (const Vec2 &rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}


Vec2 Vec2::operator - (const Vec2 &rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}


Vec2 Vec2::operator * (const Vec2 &rhs) const
{
    return Vec2(x * rhs.x, y * rhs.y);
}


Vec2 Vec2::operator / (const Vec2 &rhs) const
{
    return Vec2(x / rhs.x, y / rhs.y);
}


Vec2& Vec2::operator += (const Vec2 &rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}


Vec2& Vec2::operator -= (const Vec2 &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}


Vec2& Vec2::operator *= (const Vec2 &rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    return *this;
}


Vec2& Vec2::operator /= (const Vec2 &rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    return *this;
}


float Vec2::distance (const Vec2 &rhs) const
{
    float dist = sqrt( (x - rhs.x)*(x - rhs.x) + (y - rhs.y)*(y - rhs.y));
    return dist;
}


Vec2& Vec2::normalize () 
{
    float dist = sqrt( x*x + y*y );
    if(dist)
    {
        x = x / dist;
        y = y / dist;
    }
    return *this;
}


Vec2& Vec2::scale (float scalar)
{
    x = 1.0f * x * scalar;
    y = 1.0f * y * scalar;
    return *this;
}