#ifndef BILLING_H
#define BILLING_H

float calculateFee(int entryHour, int entryMinute,
                   int exitHour, int exitMinute);

void saveHistory(const char *vehicleNumber,
                 const char *vehicleType,
                 int slotNumber,
                 int entryHour,
                 int entryMinute,
                 int exitHour,
                 int exitMinute,
                 float fee);

void displayHistory();
void dashboard();

#endif