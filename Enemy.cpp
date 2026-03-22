#include "Enemy.h"
#include "GameLogic.h"   // for M, N, ts
#include <cstdlib>       // for std::rand

// grid and ts are declared extern in GameLogic.h, so no need to redeclare them here.

Enemy::Enemy()
{
    // Random starting cell inside the borders [1 .. N-2], [1 .. M-2]
    int gx = 1 + rand() % (N - 2);
    int gy = 1 + rand() % (M - 2);

    // Place enemy roughly in the centre of that tile (pixel coords)
    x = gx * ts + ts / 2;
    y = gy * ts + ts / 2;

    // Random base speed in range -3..3 (not zero)
    int vx = (rand() % 7) - 3;  // -3..3
    int vy = (rand() % 7) - 3;  // -3..3

    if (vx == 0) vx = (rand() % 2) ? 2 : -2;
    if (vy == 0) vy = (rand() % 2) ? 2 : -2;

    // Make them faster by scaling
    dx = vx * 2;   // tweak 2 -> 3 if you want crazy fast
    dy = vy * 2;
}
void Enemy::move()
{
    x += dx;
    if (grid[y / ts][x / ts] == 1)
    {
        dx = -dx;
        x += dx;
    }

    y += dy;
    if (grid[y / ts][x / ts] == 1)
    {
        dy = -dy;
        y += dy;
    }
}
