#include <stdio.h>
#include "billing.h"
#include "parking.h"

float calculateFee(int entryHour, int entryMinute,
                   int exitHour, int exitMinute)
{
    int entryTotal = entryHour * 60 + entryMinute;
    int exitTotal = exitHour * 60 + exitMinute;

    int duration = exitTotal - entryTotal;

    if (duration <= 0)
    {
        return 0;
    }

    float hours = duration / 60.0;

    return hours * 30;
}

void saveHistory(const char *vehicleNumber,
                 const char *vehicleType,
                 int slotNumber,
                 int entryHour,
                 int entryMinute,
                 int exitHour,
                 int exitMinute,
                 float fee)
{
    FILE *file = fopen("history.txt", "a");

    if (file == NULL)
    {
        printf("\nError opening history file!\n");
        return;
    }

    fprintf(file,
            "Vehicle: %s | Type: %s | Slot: %d | "
            "Entry: %02d:%02d | Exit: %02d:%02d | Fee: Rs. %.2f\n",
            vehicleNumber,
            vehicleType,
            slotNumber,
            entryHour,
            entryMinute,
            exitHour,
            exitMinute,
            fee);

    fclose(file);

    printf("\nParking history saved successfully!\n");
}

void displayHistory()
{
    FILE *file;
    char line[300];

    file = fopen("history.txt", "r");

    printf("\n========================================\n");
    printf("           PARKING HISTORY\n");
    printf("========================================\n");

    if (file == NULL)
    {
        printf("\nNo parking history available.\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("\n%s", line);
    }

    fclose(file);
}
void dashboard()
{
    int available = 0;
    int occupied = 0;

    printf("\n========================================\n");
    printf("              DASHBOARD\n");
    printf("========================================\n");

    for (int i = 1; i <= TOTAL_SLOTS; i++)
    {
        if (isSlotAvailable(i))
        {
            available++;
        }
        else
        {
            occupied++;
        }
    }

    printf("\nTotal Slots      : %d\n", TOTAL_SLOTS);
    printf("Available Slots  : %d\n", available);
    printf("Occupied Slots   : %d\n", occupied);

    printf("\n========================================\n");
}