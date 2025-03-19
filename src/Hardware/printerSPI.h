//
// Created by keqiu on 25-3-17.
//

#ifndef PRINTERSPI_H
#define PRINTERSPI_H


#include <Arduino.h>

void SPIInit();


void SPICommand(uint8_t* data,uint8_t size);


#endif //PRINTERSPI_H
