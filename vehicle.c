#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "allocation.h"
#include "vehicle.h"
#include "parking.h"
#include "billing.h"

struct Vehicle vehicles[MAX_VEHICLES];

int vehicleCount = 0;

void initializeVehicles()
{
    vehicleCount = 0;
}

int validTime(int hour, int minute)
{
    return hour >= 0 && hour <= 23 &&
           minute >= 0 && minute <= 59;
}

void addVehicle()
{
    if (vehicleCount >= MAX_VEHICLES)
    {
        printf("\nVehicle storage is full!\n");
        return;
    }

    printf("\n========================================\n");
    printf("            VEHICLE ENTRY\n");
    printf("========================================\n");

    /* Vehicle Number */
    int duplicate;

do
{
    duplicate = 0;

    printf("\nEnter vehicle number: ");
    scanf("%19s", vehicles[vehicleCount].vehicleNumber);

    for (int i = 0; i < vehicleCount; i++)
    {
        if (strcmp(vehicles[i].vehicleNumber,
                   vehicles[vehicleCount].vehicleNumber) == 0)
        {
            duplicate = 1;
            printf("\nVehicle is already parked!\n");
            printf("Please enter a different vehicle number.\n");
            break;
        }
    }

} while (duplicate);

    /* Vehicle Type Validation */
    do
{
    printf("Enter vehicle type (Car/Bike): ");
    scanf("%19s", vehicles[vehicleCount].vehicleType);

    /* Convert input to lowercase */
    for (int i = 0; vehicles[vehicleCount].vehicleType[i] != '\0'; i++)
    {
        vehicles[vehicleCount].vehicleType[i] =
            tolower(vehicles[vehicleCount].vehicleType[i]);
    }

    if (strcmp(vehicles[vehicleCount].vehicleType, "car") != 0 &&
        strcmp(vehicles[vehicleCount].vehicleType, "bike") != 0)
    {
        printf("\nInvalid type! Enter Car or Bike.\n");
    }

} while (strcmp(vehicles[vehicleCount].vehicleType, "car") != 0 &&
         strcmp(vehicles[vehicleCount].vehicleType, "bike") != 0);

/* Store standard format */
if (strcmp(vehicles[vehicleCount].vehicleType, "car") == 0)
{
    strcpy(vehicles[vehicleCount].vehicleType, "Car");
}
else
{
    strcpy(vehicles[vehicleCount].vehicleType, "Bike");
}

    /* Find nearest suitable slot */
    int slot = findNearestSlotByType(
        vehicles[vehicleCount].vehicleType
    );

    if (slot == -1)
    {
        printf("\nSorry! No suitable parking slot is available.\n");
        return;
    }

    vehicles[vehicleCount].slotNumber = slot;

    /* Entry Time Validation */
    do
    {
        printf("Enter Entry Hour (0-23): ");
        scanf("%d", &vehicles[vehicleCount].entryHour);

        printf("Enter Entry Minute (0-59): ");
        scanf("%d", &vehicles[vehicleCount].entryMinute);

        if (!validTime(
                vehicles[vehicleCount].entryHour,
                vehicles[vehicleCount].entryMinute))
        {
            printf("\nInvalid time! Please enter again.\n");
        }

    } while (!validTime(
        vehicles[vehicleCount].entryHour,
        vehicles[vehicleCount].entryMinute));

    /* Occupy slot */
    updateSlotStatus(slot, 1);

    vehicleCount++;

    printf("\n========================================\n");
    printf("       VEHICLE PARKED SUCCESSFULLY\n");
    printf("========================================\n");

    printf("Vehicle Number : %s\n",
           vehicles[vehicleCount - 1].vehicleNumber);

    printf("Vehicle Type   : %s\n",
           vehicles[vehicleCount - 1].vehicleType);

    printf("Assigned Slot  : %d\n", slot);

    printf("Entry Time     : %02d:%02d\n",
           vehicles[vehicleCount - 1].entryHour,
           vehicles[vehicleCount - 1].entryMinute);
}

void removeVehicle()
{
    char vehicleNumber[20];

    printf("\n========================================\n");
    printf("              VEHICLE EXIT\n");
    printf("========================================\n");

    printf("Enter Vehicle Number: ");
    scanf("%19s", vehicleNumber);

    int foundIndex = -1;

    /* Search vehicle */
    for (int i = 0; i < vehicleCount; i++)
    {
        if (strcmp(
                vehicles[i].vehicleNumber,
                vehicleNumber) == 0)
        {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1)
    {
        printf("\nVehicle not found!\n");
        return;
    }

    int exitHour;
    int exitMinute;

    /* Exit Time Validation */
    do
{
    printf("Enter Exit Hour (0-23): ");
    scanf("%d", &exitHour);

    printf("Enter Exit Minute (0-59): ");
    scanf("%d", &exitMinute);

    int entryTime =
        vehicles[foundIndex].entryHour * 60 +
        vehicles[foundIndex].entryMinute;

    int exitTime =
        exitHour * 60 + exitMinute;

    if (!validTime(exitHour, exitMinute))
    {
        printf("\nInvalid time! Please enter again.\n");
    }
    else if (exitTime <= entryTime)
    {
        printf("\nExit time must be after entry time!\n");
    }

} while (!validTime(exitHour, exitMinute) ||
         (exitHour * 60 + exitMinute) <=
         (vehicles[foundIndex].entryHour * 60 +
          vehicles[foundIndex].entryMinute));

    /* Calculate parking fee */
    float fee = calculateFee(
        vehicles[foundIndex].entryHour,
        vehicles[foundIndex].entryMinute,
        exitHour,
        exitMinute
    );

    /* Save parking history */
    saveHistory(
        vehicles[foundIndex].vehicleNumber,
        vehicles[foundIndex].vehicleType,
        vehicles[foundIndex].slotNumber,
        vehicles[foundIndex].entryHour,
        vehicles[foundIndex].entryMinute,
        exitHour,
        exitMinute,
        fee
    );

    /* Free parking slot */
    int slot = vehicles[foundIndex].slotNumber;

    updateSlotStatus(slot, 0);

    /* Remove vehicle from array */
    for (int i = foundIndex; i < vehicleCount - 1; i++)
    {
        vehicles[i] = vehicles[i + 1];
    }

    vehicleCount--;

    printf("\n========================================\n");
    printf("        VEHICLE EXIT SUCCESSFUL\n");
    printf("========================================\n");

    printf("Vehicle Number : %s\n", vehicleNumber);
    printf("Slot Number    : %d\n", slot);
    printf("Parking Fee    : Rs. %.2f\n", fee);
    printf("Slot %d is now AVAILABLE.\n", slot);
}

void searchVehicle()
{
    char vehicleNumber[20];

    printf("\nEnter vehicle number to search: ");
    scanf("%19s", vehicleNumber);

    for (int i = 0; i < vehicleCount; i++)
    {
        if (strcmp(
                vehicles[i].vehicleNumber,
                vehicleNumber) == 0)
        {
            printf("\n========================================\n");
            printf("             VEHICLE FOUND\n");
            printf("========================================\n");

            printf("Vehicle Number : %s\n",
                   vehicles[i].vehicleNumber);

            printf("Vehicle Type   : %s\n",
                   vehicles[i].vehicleType);

            printf("Slot Number    : %d\n",
                   vehicles[i].slotNumber);

            printf("Entry Time     : %02d:%02d\n",
                   vehicles[i].entryHour,
                   vehicles[i].entryMinute);

            return;
        }
    }

    printf("\nVehicle not found!\n");
}

void displayVehicles()
{
    if (vehicleCount == 0)
    {
        printf("\nNo vehicles are currently parked.\n");
        return;
    }

    printf("\n========================================\n");
    printf("          PARKED VEHICLES\n");
    printf("========================================\n");

    printf("\n%-15s %-10s %-10s %-10s\n",
           "Vehicle",
           "Type",
           "Slot",
           "Entry");

    printf("----------------------------------------\n");

    for (int i = 0; i < vehicleCount; i++)
    {
        printf("%-15s %-10s %-10d %02d:%02d\n",
               vehicles[i].vehicleNumber,
               vehicles[i].vehicleType,
               vehicles[i].slotNumber,
               vehicles[i].entryHour,
               vehicles[i].entryMinute);
    }
}