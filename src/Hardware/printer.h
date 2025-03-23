//
// Created by keqiu on 25-3-17.
//

#ifndef PRINTER_H
#define PRINTER_H

void PrinterInit();
void PrinterStbTest();
void PrinterPowerOff();
void PrinterPowerInit();

void StartPrintingByOneStb(uint8_t StbNum,uint8_t* data,uint32_t size);
void StartPrintingByAllStb(uint8_t* data,uint32_t size);

#endif //PRINTER_H
