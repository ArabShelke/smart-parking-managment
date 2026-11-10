#ifndef VEHICLE_H
#define VEHICLE_H

#define MAX_VEHICLES 20

struct Vehicle
{
    char vehicleNumber[20];
    char vehicleType[20];
    int slotNumber;
    int entryHour;
    int entryMinute;
};

void initializeVehicles();
void addVehicle();
void removeVehicle();
void searchVehicle();
void displayVehicles();

#endif