#include "GameLogic.h"

// IMPORTANT: real definitions here.
const int M = 25;
const int N = 40;

int grid[25][40] = { 0 };
int ts = 18;

// Safe flood-fill: never goes out of bounds
void drop(int y, int x)
{
    // 1) Bounds check first – if outside grid, stop
    if (y < 0 || y >= M || x < 0 || x >= N)
        return;

    // 2) Only flood empty cells (0). Anything else (1, 2, 3, -1) stops.
    if (grid[y][x] != 0)
        return;

    // 3) Mark as visited
    grid[y][x] = -1;

    // 4) Recurse to 4 neighbours
    drop(y - 1, x);
    drop(y + 1, x);
    drop(y, x - 1);
    drop(y, x + 1);
}
