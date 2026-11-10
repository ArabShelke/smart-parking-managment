#ifndef PARKING_H
#define PARKING_H

#define TOTAL_SLOTS 10

struct ParkingSlot
{
    int slotNumber;
    char slotType[20];
    int isOccupied;
};

void initializeSlots();
void displaySlots();
int findAvailableSlot();
int findAvailableSlotByType(const char *slotType);
int isSlotAvailable(int slotNumber);
void updateSlotStatus(int slotNumber, int status);

#endif