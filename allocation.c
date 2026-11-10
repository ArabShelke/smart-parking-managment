#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "allocation.h"
#include "parking.h"

#define NODES 11
#define INF 9999

int graph[NODES][NODES];

void initializeGraph()
{
    for (int i = 0; i < NODES; i++)
    {
        for (int j = 0; j < NODES; j++)
        {
            graph[i][j] = 0;
        }
    }

    // Entrance = Node 0

    graph[0][1] = 5;
    graph[1][0] = 5;

    graph[0][2] = 8;
    graph[2][0] = 8;

    graph[0][3] = 12;
    graph[3][0] = 12;

    graph[1][4] = 4;
    graph[4][1] = 4;

    graph[2][5] = 3;
    graph[5][2] = 3;

    graph[3][6] = 4;
    graph[6][3] = 4;

    graph[4][7] = 5;
    graph[7][4] = 5;

    graph[5][8] = 4;
    graph[8][5] = 4;

    graph[6][9] = 3;
    graph[9][6] = 3;

    graph[7][10] = 4;
    graph[10][7] = 4;
}

void displayGraph()
{
    printf("\n========================================\n");
    printf("          PARKING GRAPH\n");
    printf("========================================\n");

    printf("\nParking graph initialized successfully.\n");
    printf("Node 0 = Entrance\n");
    printf("Nodes 1-10 = Parking Slots\n");
}

int findNearestSlot()
{
    int distance[NODES];
    int visited[NODES] = {0};

    for (int i = 0; i < NODES; i++)
    {
        distance[i] = INF;
    }

    distance[0] = 0;

    for (int count = 0; count < NODES - 1; count++)
    {
        int minDistance = INF;
        int current = -1;

        for (int i = 0; i < NODES; i++)
        {
            if (!visited[i] && distance[i] < minDistance)
            {
                minDistance = distance[i];
                current = i;
            }
        }

        if (current == -1)
        {
            break;
        }

        visited[current] = 1;

        for (int i = 0; i < NODES; i++)
        {
            if (graph[current][i] != 0 &&
                !visited[i] &&
                distance[current] + graph[current][i] < distance[i])
            {
                distance[i] =
                    distance[current] + graph[current][i];
            }
        }
    }

    int nearestSlot = -1;
    int shortestDistance = INF;

    for (int slot = 1; slot <= 10; slot++)
    {
        if (isSlotAvailable(slot) &&
            distance[slot] < shortestDistance)
        {
            shortestDistance = distance[slot];
            nearestSlot = slot;
        }
    }

    return nearestSlot;
}

void allocateSlot()
{
    int slot = findNearestSlot();

    if (slot == -1)
    {
        printf("\nNo parking slot is available.\n");
        return;
    }

    updateSlotStatus(slot, 1);

    printf("\nSmart Allocation Successful!\n");
    printf("Nearest Available Slot: %d\n", slot);
}
int findNearestSlotByType(const char *vehicleType)
{
    int distance[NODES];
    int visited[NODES] = {0};

    for (int i = 0; i < NODES; i++)
    {
        distance[i] = INF;
    }

    distance[0] = 0;

    for (int count = 0; count < NODES - 1; count++)
    {
        int minDistance = INF;
        int current = -1;

        for (int i = 0; i < NODES; i++)
        {
            if (!visited[i] && distance[i] < minDistance)
            {
                minDistance = distance[i];
                current = i;
            }
        }

        if (current == -1)
            break;

        visited[current] = 1;

        for (int i = 0; i < NODES; i++)
        {
            if (graph[current][i] != 0 &&
                !visited[i] &&
                distance[current] + graph[current][i] < distance[i])
            {
                distance[i] =
                    distance[current] + graph[current][i];
            }
        }
    }

    int nearestSlot = -1;
    int shortestDistance = INF;

    for (int slot = 1; slot <= 10; slot++)
    {
        if (isSlotAvailable(slot) &&
            distance[slot] < shortestDistance)
        {
            if ((strcmp(vehicleType, "Car") == 0 && slot <= 5) ||
                (strcmp(vehicleType, "Bike") == 0 && slot >= 6))
            {
                shortestDistance = distance[slot];
                nearestSlot = slot;
            }
        }
    }

    return nearestSlot;
}