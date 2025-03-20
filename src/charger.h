#ifndef CHARGER_H
#define CHARGER_H


// Пины для управления направлением
#define IN19 19   // Voltage of battary
#define IN20 20   // Voltage of charge


void init_array();
void initChargerPins();
char *getVoltageJson();

#endif