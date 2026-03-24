#include "GameLogic.h"

const int M = 25;
const int N = 40;

int grid[25][40] = { 0 };
int ts = 18;

void drop(int y, int x)
{
    struct Cell
    {
        int y;
        int x;
    };

    if (y < 0 || y >= M || x < 0 || x >= N)
        return;
    if (grid[y][x] != 0)
        return;

    Cell stack[M * N * 4];
    int top = 0;
    stack[top++] = Cell{ y, x };

    while (top > 0)
    {
        Cell cell = stack[--top];
        if (cell.y < 0 || cell.y >= M || cell.x < 0 || cell.x >= N)
            continue;
        if (grid[cell.y][cell.x] != 0)
            continue;

        grid[cell.y][cell.x] = -1;

        stack[top++] = Cell{ cell.y - 1, cell.x };
        stack[top++] = Cell{ cell.y + 1, cell.x };
        stack[top++] = Cell{ cell.y, cell.x - 1 };
        stack[top++] = Cell{ cell.y, cell.x + 1 };
    }
}
