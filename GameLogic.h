#pragma once

// Export grid + constants so other modules can access them
extern const int M;
extern const int N;

extern int grid[25][40];
extern int ts;

// Flood-fill region closure
void drop(int y, int x);
