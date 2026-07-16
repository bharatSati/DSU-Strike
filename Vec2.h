#pragma once

class Vec2
{
public:
    float x = 0;
    float y = 0;

    Vec2 ();
    Vec2 (float xVal, float yVal);

    bool operator == (const Vec2 &rhs) const;
    bool operator != (const Vec2 &rhs) const;

    Vec2 operator + (const Vec2 &rhs) const;
    Vec2 operator - (const Vec2 &rhs) const;
    Vec2 operator * (const Vec2 &rhs) const;
    Vec2 operator / (const Vec2 &rhs) const;

    Vec2& operator += (const Vec2 &rhs);
    Vec2& operator -= (const Vec2 &rhs);
    Vec2& operator *= (const Vec2 &rhs);
    Vec2& operator /= (const Vec2 &rhs);

    float distance (const Vec2 &rhs) const;
    
    Vec2& normalize ();


     

};