#include "MatchmakingQueue.h"

// -------------------- Constructor --------------------

MatchmakingQueue::MatchmakingQueue()
    : size(0)
{
}

// -------------------- Basic helpers --------------------

void MatchmakingQueue::clear()
{
    size = 0;
}

bool MatchmakingQueue::isEmpty() const
{
    return (size == 0);
}

bool MatchmakingQueue::canCreateMatch() const
{
    // Need at least 2 players to form a match
    return (size >= 2);
}

// -------------------- Heap helpers (max-heap) --------------------

// Maintain max-heap: parent.priority >= children.priority
void MatchmakingQueue::heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;
        if (arr[parent].priority >= arr[index].priority)
            break;

        // Swap parent and child
        MMEntry tmp = arr[parent];
        arr[parent] = arr[index];
        arr[index] = tmp;

        index = parent;
    }
}

void MatchmakingQueue::heapifyDown(int index)
{
    while (true)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < size && arr[left].priority > arr[largest].priority)
            largest = left;

        if (right < size && arr[right].priority > arr[largest].priority)
            largest = right;

        if (largest == index)
            break;

        MMEntry tmp = arr[index];
        arr[index] = arr[largest];
        arr[largest] = tmp;

        index = largest;
    }
}

// -------------------- Queue interface --------------------

void MatchmakingQueue::push(Player* player, int priority)
{
    if (!player)
        return;

    if (size >= MAX_WAITING)
    {
        // Queue full: ignore extra players for now
        return;
    }

    MMEntry entry;
    entry.player = player;
    entry.priority = priority;
    entry.totalPoints = priority;   // keep this in sync with priority

    arr[size] = entry;
    heapifyUp(size);
    ++size;
}

bool MatchmakingQueue::popTopTwo(MMEntry& a, MMEntry& b)
{
    if (size < 2)
        return false;

    // Pop top-1 (highest priority)
    a = arr[0];
    arr[0] = arr[size - 1];
    --size;
    if (size > 0)
        heapifyDown(0);

    // Pop top-2 (now the next highest)
    b = arr[0];
    arr[0] = arr[size - 1];
    --size;
    if (size > 0)
        heapifyDown(0);

    return true;
}

// Copy current entries into outArray, sorted by descending priority
void MatchmakingQueue::getSnapshot(MMEntry outArray[], int& count) const
{
    count = size;
    if (count > MAX_WAITING)
        count = MAX_WAITING;

    // Copy existing heap
    for (int i = 0; i < count; ++i)
    {
        outArray[i] = arr[i];
    }

    // Simple selection sort in descending order of priority for display
    for (int i = 0; i < count; ++i)
    {
        int bestIdx = i;
        for (int j = i + 1; j < count; ++j)
        {
            if (outArray[j].priority > outArray[bestIdx].priority)
            {
                bestIdx = j;
            }
        }

        if (bestIdx != i)
        {
            MMEntry tmp = outArray[i];
            outArray[i] = outArray[bestIdx];
            outArray[bestIdx] = tmp;
        }
    }
}
