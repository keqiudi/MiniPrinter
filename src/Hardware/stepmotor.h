//
// Created by keqiu on 25-3-17.
//

#ifndef STEPMOTOR_H
#define STEPMOTOR_H

#include <Arduino.h>

void StepmotorInit();

void StepmotorRunStep(uint32_t steps);
void StepmotorRunOneStep();

void StepmotorStart();

void StepmotorStop();

#endif //STEPMOTOR_H
