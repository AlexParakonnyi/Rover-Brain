#ifndef VOLTAGE_DATA_H  // Защита от повторного включения
#define VOLTAGE_DATA_H

const int dataSize = 27;
const float voltageData[dataSize][3] = {
    {3, 2750, 2955},    {3.5, 3226, 3315},  {4, 3288, 3340},
    {4.5, 3322, 3375},  {5, 3350, 3400},    {5.5, 3370, 3420},
    {6, 3395, 3440},    {6.5, 3412, 3460},  {7, 3430, 3475},
    {7.5, 3446, 3490},  {8, 3440, 3500},    {8.5, 3433, 515},
    {9, 3475, 3530},    {9.5, 3450, 3542},  {10, 3471, 3555},
    {10.5, 3486, 3567}, {11, 3525, 3580},   {11.5, 3505, 3525},
    {12, 3524, 3600},   {12.5, 3532, 3619}, {13, 3550, 3630},
    {13.5, 3557, 3645}, {14, 3573, 3660},   {14.5, 3655, 3665},
    {15, 3662, 3676},   {16, 3680, 3703},   {16.3, 3690, 3830},
};

#endif  // VOLTAGE_DATA_H