#include "Collision2d.h"

Collision2d* Collision2d::GetIns()
{
    static Collision2d ins;
    return &ins;
}

bool Collision2d::CircleAndCircle(Sprite* obj1, Sprite* obj2, float radius1, float radius2)
{
    float x1, x2;
    float y1, y2;
    x1 = obj1->GetPosition().x;
    x2 = obj2->GetPosition().x;
    y1 = obj1->GetPosition().y;
    y2 = obj2->GetPosition().y;
    return ((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)) <= radius1 + radius2;
}

bool Collision2d::CircleAndCircle(Vector2 pos1, Vector2 pos2, float radius1, float radius2)
{
    float x1, x2;
    float y1, y2;
    x1 = pos1.x;
    x2 = pos2.x;
    y1 = pos1.y;
    y2 = pos2.y;
    return ((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)) <= ((radius1 + radius2) * (radius1 + radius2));
}
