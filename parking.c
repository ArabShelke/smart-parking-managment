#include <stdio.h>
#include <string.h>

#include "parking.h"

struct ParkingSlot slots[TOTAL_SLOTS];

void initializeSlots()
{
    for (int i = 0; i < TOTAL_SLOTS; i++)
    {
        slots[i].slotNumber = i + 1;

        if (i < 5)
        {
            strcpy(slots[i].slotType, "Car");
        }
        else
        {
            strcpy(slots[i].slotType, "Bike");
        }

        slots[i].isOccupied = 0;
    }
}

void displaySlots()
{
    printf("\n========================================\n");
    printf("           PARKING SLOT STATUS\n");
    printf("========================================\n");

    printf("\n%-10s %-15s %-15s\n",
           "Slot", "Type", "Status");

    printf("----------------------------------------\n");

    for (int i = 0; i < TOTAL_SLOTS; i++)
    {
        printf("%-10d %-15s %-15s\n",
               slots[i].slotNumber,
               slots[i].slotType,
               slots[i].isOccupied ? "OCCUPIED" : "AVAILABLE");
    }
}

int findAvailableSlot()
{
    for (int i = 0; i < TOTAL_SLOTS; i++)
    {
        if (slots[i].isOccupied == 0)
        {
            return slots[i].slotNumber;
        }
    }

    return -1;
}

void updateSlotStatus(int slotNumber, int status)
{
    if (slotNumber < 1 || slotNumber > TOTAL_SLOTS)
    {
        printf("\nInvalid slot number!\n");
        return;
    }

    slots[slotNumber - 1].isOccupied = status;
}
int isSlotAvailable(int slotNumber)
{
    if (slotNumber < 1 || slotNumber > TOTAL_SLOTS)
    {
        return 0;
    }

    return slots[slotNumber - 1].isOccupied == 0;
}
int getTotalSlots()
{
    return TOTAL_SLOTS;
}
int getSlotOccupied(int slotNumber)
{
    if (slotNumber < 1 || slotNumber > TOTAL_SLOTS)
    {
        return -1;
    }

    return slots[slotNumber - 1].isOccupied;
}


char slotVehicleNumbers[TOTAL_SLOTS][20];

void setSlotVehicleNumber(int slotNumber, const char *vehicleNumber)
{
    if (slotNumber < 1 || slotNumber > TOTAL_SLOTS)
    {
        return;
    }

    strncpy(slotVehicleNumbers[slotNumber - 1], vehicleNumber, 19);
    slotVehicleNumbers[slotNumber - 1][19] = '\0';
}

const char *getSlotVehicleNumber(int slotNumber)
{
    if (slotNumber < 1 || slotNumber > TOTAL_SLOTS)
    {
        return "";
    }

    return slotVehicleNumbers[slotNumber - 1];
}
