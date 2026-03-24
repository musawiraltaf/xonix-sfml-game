#pragma once

struct Enemy
{
    int x, y, dx, dy;
    float angle;

    Enemy();
    void move();
    float visualRotation() const;
};
