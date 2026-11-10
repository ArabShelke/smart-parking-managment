#include <stdio.h>

#include "parking.h"
#include "vehicle.h"
#include "allocation.h"
#include "billing.h"

int main()
{
    initializeSlots();
    initializeVehicles();
    initializeGraph();
    int choice;

    printf("\n");
    printf("========================================\n");
    printf("     SMART PARKING MANAGEMENT SYSTEM\n");
    printf("========================================\n");

    do
    {
        printf("\n----------- MAIN MENU -----------\n");
        printf("1. Park Vehicle\n");
        printf("2. Vehicle Exit\n");
        printf("3. Display Parking Slots\n");
        printf("4. Find Smart Parking Slot\n");
        printf("5. Search Vehicle\n");
        printf("6. Parking History\n");
        printf("7. Calculate Parking Fee\n");
        printf("8. Dashboard\n");
        printf("9. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addVehicle();
            break;

            case 2:
                removeVehicle();
                break;

            case 3:
                displaySlots();
                break;

            case 4:
                displayGraph();
                   
                break;

            case 5:
                searchVehicle();
                break;

            case 6:
                displayHistory();
                break;

            case 7:
                printf("\nParking fee is calculated during vehicle exit.\n");
                break;

            case 8:
                dashboard();
                break;

            case 9:
                printf("\nThank you for using Smart Parking System!\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 9);

    return 0;
}