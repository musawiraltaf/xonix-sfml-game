#include "Enemy.h"
#include "GameLogic.h"
#include <cstdlib>
#include <cmath>

Enemy::Enemy()
{
    int gx = 1 + std::rand() % (N - 2);
    int gy = 1 + std::rand() % (M - 2);

    x = gx * ts + ts / 2;
    y = gy * ts + ts / 2;

    int vx = (std::rand() % 7) - 3;
    int vy = (std::rand() % 7) - 3;
    if (vx == 0) vx = (std::rand() % 2) ? 2 : -2;
    if (vy == 0) vy = (std::rand() % 2) ? 2 : -2;

    dx = vx * 2;
    dy = vy * 2;

    float facing = std::atan2(static_cast<float>(dy), static_cast<float>(dx));
    angle = facing * 180.0f / 3.14159265f + 90.0f;
}

void Enemy::move()
{
    x += dx;
    int gx = x / ts;
    int gy = y / ts;
    if (gx < 0) gx = 0;
    if (gx >= N) gx = N - 1;
    if (gy < 0) gy = 0;
    if (gy >= M) gy = M - 1;
    if (grid[gy][gx] == 1)
    {
        dx = -dx;
        x += dx;
    }

    y += dy;
    gx = x / ts;
    gy = y / ts;
    if (gx < 0) gx = 0;
    if (gx >= N) gx = N - 1;
    if (gy < 0) gy = 0;
    if (gy >= M) gy = M - 1;
    if (grid[gy][gx] == 1)
    {
        dy = -dy;
        y += dy;
    }

    const float speed = std::sqrt(static_cast<float>(dx * dx + dy * dy));
    float spinDirection = (dx >= 0) ? 1.0f : -1.0f;
    if (dy < 0)
        spinDirection *= -1.0f;
    angle += spinDirection * (speed * 1.75f);
    while (angle >= 360.0f) angle -= 360.0f;
    while (angle < 0.0f) angle += 360.0f;
}

float Enemy::visualRotation() const
{
    return angle;
}
